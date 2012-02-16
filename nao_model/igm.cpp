#include <Eigen/Core>       // Cholesky decomposition + solving
#include <Eigen/Cholesky>   // of system of linear equations.

#include "nao_igm.h"
#include "maple_functions.h"



/**
 * @brief Get feet positions.
 *
 * @param[out] left_foot_expected 3x1 expected position vector
 * @param[out] right_foot_expected 3x1 expected position vector
 * @param[out] left_foot_computed 3x1 position vector determined using sensor data
 * @param[out] right_foot_computed 3x1 position vector determined using sensor data
 *
 * @note Support foot position is assumed to be correct and there is no difference
 * between the expected and 'real' positions.
 */
void nao_igm::getFeetPositions (
        double *left_foot_expected,
        double *right_foot_expected,
        double *left_foot_computed,
        double *right_foot_computed)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        Vector3d::Map(left_foot_expected) = Vector3d::Map(left_foot_computed) = left_foot_posture.translation();
        Vector3d::Map(right_foot_expected) = right_foot_posture.translation();
        getSwingFootPosition (state_sensor, right_foot_computed);
    }
    else
    {
        Vector3d::Map(right_foot_expected) = Vector3d::Map(right_foot_computed) = right_foot_posture.translation();
        Vector3d::Map(left_foot_expected) = right_foot_posture.translation();
        getSwingFootPosition (state_sensor, left_foot_computed);
    }
}



/**
 * @brief Set coordinates of center of mass.
 *
 * @param[in] x coordinate
 * @param[in] y coordinate
 * @param[in] z coordinate
 */
void nao_igm::setCoM (const double x, const double y, const double z)
{
    CoM_position[0] = x;
    CoM_position[1] = y;
    CoM_position[2] = z;
}



/** @brief Initialize model.

    @param[in] support_foot_ current support foot.
    @param[in] x x-position
    @param[in] y y-position
    @param[in] z z-position
    @param[in] roll x-rotation
    @param[in] pitch y-rotation
    @param[in] yaw z-rotation

    @attention Joint angles in state_sensor must be set.
*/
void nao_igm::init(
        const igmSupportFoot support_foot_, 
        const double x, 
        const double y, 
        const double z, 
        const double roll, 
        const double pitch, 
        const double yaw)
{
    support_foot = support_foot_;
    state_model = state_sensor;


    Transform<double,3> torso_posture;
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        left_foot_posture = 
            Translation<double,3>(x, y, z) *
            AngleAxisd(roll, Vector3d::UnitX()) *
            AngleAxisd(pitch, Vector3d::UnitY()) *
            AngleAxisd(yaw, Vector3d::UnitZ());
        LLeg2Torso(state_sensor.q, left_foot_posture.data(), torso_posture.data());
        getSwingFootPosture(state_sensor, right_foot_posture);
    }
    else
    {
        right_foot_posture = 
            Translation<double,3>(x, y, z) *
            AngleAxisd(roll, Vector3d::UnitX()) *
            AngleAxisd(pitch, Vector3d::UnitY()) *
            AngleAxisd(yaw, Vector3d::UnitZ());
        RLeg2Torso(state_sensor.q, right_foot_posture.data(), torso_posture.data());
        getSwingFootPosture(state_sensor, left_foot_posture);
    }
    Matrix3d::Map (torso_orientation) = torso_posture.matrix().corner(TopLeft,3,3);

    getCoM (state_sensor, CoM_position);
}



/**
 * @brief Switch support foot.
 *
 * @return A 4x4 homogeneous matrix, which contains position and orientation 
 * (computed using sensor data) of the new support foot.
 */
double* nao_igm::switchSupportFoot()
{
    state_model = state_sensor;

    if (support_foot == IGM_SUPPORT_LEFT)
    {
        getSwingFootPosture (state_sensor, right_foot_posture);
        support_foot = IGM_SUPPORT_RIGHT;
        return (right_foot_posture.data());
    }
    else
    {
        getSwingFootPosture (state_sensor, left_foot_posture);
        support_foot = IGM_SUPPORT_LEFT;
        return (left_foot_posture.data());
    }
}



/**
 * @brief Compute position of the CoM from joint angles.
 *
 * @param[in] joints state of the joints.
 * @param[in,out] CoM_pos 3x1 vector of coordinates.
 */
void nao_igm::getCoM (jointState& joints, double *CoM_pos)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2CoM(joints.q, left_foot_posture.data(), CoM_pos);
    }
    else
    {
        RLeg2CoM(joints.q, right_foot_posture.data(), CoM_pos);
    }
}



/**
 * @brief Compute position of the swing foot from joint angles.
 *
 * @param[in] joints state of the joints.
 * @param[in,out] swing_foot_position  3x1 vector of coordinates.
 */
void nao_igm::getSwingFootPosition (jointState& joints, double * swing_foot_position)
{
    Transform<double,3> swing_foot_posture_tmp;
    getSwingFootPosture(joints, swing_foot_posture_tmp);
    Vector3d::Map(swing_foot_position) = swing_foot_posture_tmp.translation();
}



/**
 * @brief Compute position of the swing foot from joint angles.
 *
 * @param[in] joints state of the joints.
 * @param[in,out] swing_foot_posture  homogeneous 4x4 matrix
 */
void nao_igm::getSwingFootPosture (jointState& joints, Transform<double,3>& swing_foot_posture)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(joints.q, left_foot_posture.data(), swing_foot_posture.data());
    }
    else
    {
        RLeg2LLeg(joints.q, right_foot_posture.data(), swing_foot_posture.data());
    }
}



/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm()
{
    const int num_constraints = LOWER_JOINTS_NUM-1; // one is skipped

    Matrix<double, num_constraints, num_constraints> AAT;
    Matrix<double, LOWER_JOINTS_NUM, 1> dq;

    double out[num_constraints*LOWER_JOINTS_NUM + num_constraints];
    Map<MatrixXd> A(out,num_constraints,LOWER_JOINTS_NUM);
    Map<VectorXd> err(out+num_constraints*LOWER_JOINTS_NUM,num_constraints);

    double tol = 0.0005;
    int max_iter = 20;
    int iter;
    double norm_dq = 1.0;

    for (iter = 0; (norm_dq > tol) && (iter <= max_iter); ++iter)
    {
        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
        {
            from_LLeg_3 (
                    state_model.q, 
                    left_foot_posture.data(), 
                    right_foot_posture.data(), 
                    CoM_position, 
                    torso_orientation, 
                    out);
        }
        else
        {
            from_RLeg_3 ( 
                    state_model.q, 
                    right_foot_posture.data(), 
                    left_foot_posture.data(), 
                    CoM_position, 
                    torso_orientation, 
                    out);
        }

        // Solve KKT system
        AAT = A*A.transpose();
        AAT.llt().solveInPlace(err);
        dq = A.transpose()*err;

        // Update angles (of legs)
        VectorXd::Map (state_model.q, LOWER_JOINTS_NUM) += dq;

        // Compute the norm of dq
        norm_dq = dq.norm();
    }

    if (iter > max_iter)
    {
        iter = -1;
    }

    return(iter);
}
