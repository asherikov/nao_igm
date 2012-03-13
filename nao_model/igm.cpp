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
    Transform<double, 3> swing_foot_posture;
    getSwingFootPosture (state_sensor, swing_foot_posture.data());


    Vector3d::Map(left_foot_expected) = left_foot_posture.translation();
    Vector3d::Map(right_foot_expected) = right_foot_posture.translation();


    if (support_foot == IGM_SUPPORT_LEFT)
    {
        Vector3d::Map(left_foot_computed) = left_foot_posture.translation();
        Vector3d::Map(right_foot_computed) = swing_foot_posture.translation();
    }
    else
    {
        Vector3d::Map(right_foot_computed) = right_foot_posture.translation();
        Vector3d::Map(left_foot_computed) = swing_foot_posture.translation();
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



/** 
 * @brief Initialize model.
 *
 * @param[in] support_foot_ current support foot.
 * @param[in] x x coordinate of the support foot.
 * @param[in] y y coordinate of the support foot.
 * @param[in] z z coordinate of the support foot.
 * @param[in] roll roll angle of the support foot.
 * @param[in] pitch pitch angle of the support foot.
 * @param[in] yaw yaw angle of the support foot.
 *
 * @attention Joint angles in state_sensor must be set.
 */
void nao_igm::init (
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

    if (support_foot == IGM_SUPPORT_LEFT)
    {
        left_foot_posture = 
                Translation<double,3>(x, y, z) *
                AngleAxisd(roll, Vector3d::UnitX()) *
                AngleAxisd(pitch, Vector3d::UnitY()) *
                AngleAxisd(yaw, Vector3d::UnitZ());
    }
    else
    {
        right_foot_posture = 
                Translation<double,3>(x, y, z) *
                AngleAxisd(roll, Vector3d::UnitX()) *
                AngleAxisd(pitch, Vector3d::UnitY()) *
                AngleAxisd(yaw, Vector3d::UnitZ());
    }
}



/**
 * @brief Switch support foot.
 */
void nao_igm::switchSupportFoot()
{
    state_model = state_sensor;

    if (support_foot == IGM_SUPPORT_LEFT)
    {
        support_foot = IGM_SUPPORT_RIGHT;
    }
    else
    {
        support_foot = IGM_SUPPORT_LEFT;
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
 * @param[in,out] swing_foot_posture 4x4 homogeneous matrix
 */
void nao_igm::getSwingFootPosture (jointState& joints, double *swing_foot_posture)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(joints.q, left_foot_posture.data(), swing_foot_posture);
    }
    else
    {
        RLeg2LLeg(joints.q, right_foot_posture.data(), swing_foot_posture);
    }
}



/** 
 * @brief Solves the Inverse Geometric Problem (IGM).
 *
 * @param[in] ref_angles LOWER_JOINTS_NUM reference joint angles
 * @param[in] mu penalty for difference between solution and reference angles
 * @param[in] tol tolerance
 * @param[in] max_iter maximal number of iterations
 *
 * @return the number of iterations performed until convergence, or a negative number 
 * if the algorithm did not converge within max_iter number of iterations.
 *
 * @note It is assumed that the leading matrix of the constraints is nonsingular.
 *
 * @note On input, model_state.q is taken as an initial guess for the joint angles,
 * on output it contains a solution of the inverse kinematics problem (if iter >= 0). 
 * Only the joints angles in the lower part of the body are altered.
*/
int nao_igm::igm(
        const double *ref_angles,
        const double mu,
        const double tol,
        const int max_iter)
{
    /*
     * Constraints:
     *  - 3 on the position of the swing foot
     *  - 3 on the orientation of the swing foot
     *  - 3 on the position of the CoM
     *  - 1 to take into account coupled joint L_HIP_YAW_PITCH / R_HIP_YAW_PITCH
     */
    const int num_constraints = 10;


    Map< Matrix<double, LOWER_JOINTS_NUM, 1> > q(state_model.q);
    Map< Matrix<double, LOWER_JOINTS_NUM, 1> > q0(ref_angles);
    Matrix<double, LOWER_JOINTS_NUM, 1> dq;
    Matrix<double, LOWER_JOINTS_NUM, 1> iH; // inverted Hessian

    iH.setConstant(1.0);
    // joints can be penalized here:
    //iH(L_HIP_ROLL) = 0.5;
    //iH(R_HIP_ROLL) = 0.5;


    double out[num_constraints*LOWER_JOINTS_NUM + num_constraints];
    Map< Matrix<double, num_constraints, LOWER_JOINTS_NUM> > A(out);
    Map< Matrix<double, num_constraints, 1> > err(out + num_constraints*LOWER_JOINTS_NUM);


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
                    out);
        }
        else
        {
            from_RLeg_3 ( 
                    state_model.q, 
                    right_foot_posture.data(), 
                    left_foot_posture.data(), 
                    CoM_position, 
                    out);
        }

        // Solve KKT system
        dq = mu*(q - q0);
        err = -err - A*dq;
        /// @todo FP underflow occurs in solveInPlace().
        (A*iH.asDiagonal()*A.transpose()).llt().solveInPlace(err);
        dq = -dq - iH.asDiagonal()*A.transpose() * err;
        // Update angles (of legs)
        q += dq;

        // Compute the infinity norm of dq
        // (we want the absolute value of angle change to be detectable by 
        // the corresponding sensor, see 'tol' variable above)
        norm_dq = dq.lpNorm<Eigen::Infinity>();
    }

    if (iter > max_iter)
    {
        iter = -1;
    }

    return(iter);
}
