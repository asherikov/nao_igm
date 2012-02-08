#include <Eigen/Core>       // Cholesky decomposition + solving
#include <Eigen/Cholesky>   // of system of linear equations.

#include "nao_igm.h"
#include "maple_functions.h"
#include "joint_bounds.h"



/**
 * @brief Check that all joint angles lie within bounds.
 *
 * @return -1 if all values are corrent, id of the first joint violating the
 * bounds.
 *
 * @attention No collision checks!
 */
int nao_igm::checkJointBounds()
{
    return(joint_bounds.check(state_model.q));
}



/**
 * @brief Set feet postures.
 *
 * @param[in] left_foot_position 3x1 vector of coordinates + yaw angle.
 * @param[in] right_foot_position 3x1 vector of coordinates + yaw angle.
 *
 * @note Roll and pitch angles are assumed to be 0.
 */
void nao_igm::setFeetPostures (
        const double *left_foot_position,
        const double *right_foot_position)
{
    if (state_model.support_foot == IGM_SUPPORT_LEFT)
    {
        state_sensor.initSupportPosture(
                left_foot_position[0],
                left_foot_position[1],
                left_foot_position[2],
                left_foot_position[3]);
        swing_foot_posture = 
            Translation<double,3>(right_foot_position[0], right_foot_position[1], right_foot_position[2]) *
            AngleAxisd(right_foot_position[3], Vector3d::UnitZ());
    }
    else
    {
        state_sensor.initSupportPosture(
                right_foot_position[0],
                right_foot_position[1],
                right_foot_position[2],
                right_foot_position[3]);
        swing_foot_posture = 
            Translation<double,3>(left_foot_position[0], left_foot_position[1], left_foot_position[2]) *
            AngleAxisd(left_foot_position[3], Vector3d::UnitZ());
    }

    state_sensor.copySupportPosture(state_model);
}


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
    if (state_model.support_foot == IGM_SUPPORT_LEFT)
    {
        left_foot_expected[0] = left_foot_computed[0] = state_model.q[SUPPORT_FOOT_POS_START]    ;
        left_foot_expected[1] = left_foot_computed[1] = state_model.q[SUPPORT_FOOT_POS_START + 1];
        left_foot_expected[2] = left_foot_computed[2] = state_model.q[SUPPORT_FOOT_POS_START + 2];
        Vector3d::Map(right_foot_expected) = swing_foot_posture.translation();
        state_sensor.getSwingFootPosition (right_foot_computed);
    }
    else
    {
        right_foot_expected[0] = right_foot_computed[0] = state_model.q[SUPPORT_FOOT_POS_START]    ;
        right_foot_expected[1] = right_foot_computed[1] = state_model.q[SUPPORT_FOOT_POS_START + 1];
        right_foot_expected[2] = right_foot_computed[2] = state_model.q[SUPPORT_FOOT_POS_START + 2];
        Vector3d::Map(left_foot_expected) = swing_foot_posture.translation();
        state_sensor.getSwingFootPosition (left_foot_computed);
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
    state_sensor.initSupportPosture(x, y, z, roll, pitch, yaw);
    state_sensor.support_foot = support_foot_;
    state_model = state_sensor;


    Transform<double,3> torso_posture;
    if (state_sensor.support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(state_sensor.q, swing_foot_posture.data());
        LLeg2Torso(state_sensor.q, torso_posture.data());
        LLeg2CoM(state_sensor.q, CoM_position);
    }
    else
    {
        RLeg2LLeg(state_sensor.q, swing_foot_posture.data());
        RLeg2Torso(state_sensor.q, torso_posture.data());
        RLeg2CoM(state_sensor.q, CoM_position);
    }
    Matrix3d::Map (torso_orientation) = torso_posture.matrix().corner(TopLeft,3,3);
}



/**
 * @brief Switch support foot.
 *
 * @param[in] position_error 3x1 position error vector
 *
 * @attention The error along z axis is ignored.
 */
void nao_igm::switchSupportFoot(double *position_error)
{
    Transform<double,3> swing_foot_posture_sensor;
    state_sensor.getSwingFootPosture (swing_foot_posture_sensor.data());
    Vector3d::Map(position_error) = swing_foot_posture.translation() - swing_foot_posture_sensor.translation();


    Vector3d::Map(&state_sensor.q[SUPPORT_FOOT_POS_START]) = swing_foot_posture_sensor.translation();
    // reset z position
    state_sensor.q[SUPPORT_FOOT_POS_START + 2] = 0.0; 
    Matrix3d::Map (&state_sensor.q[SUPPORT_FOOT_ORIENTATION_START]) = 
        swing_foot_posture_sensor.matrix().corner(TopLeft,3,3);


    if (state_model.support_foot == IGM_SUPPORT_LEFT)
    {
        state_sensor.support_foot = IGM_SUPPORT_RIGHT;
    }
    else
    {
        state_sensor.support_foot = IGM_SUPPORT_LEFT;
    }

    state_model = state_sensor;
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
    int N = LOWER_JOINTS_NUM;
    int num_constraints = N-1; // one is skipped

    double out[N*N+N];
    VectorXd dq(N);
    MatrixXd A(num_constraints, N);
    VectorXd err(num_constraints);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 20;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {
        // Form data
        if (state_model.support_foot == IGM_SUPPORT_LEFT)
        {
            from_LLeg_3(state_model.q, swing_foot_posture.data(), CoM_position, torso_orientation, out);
        }
        else
        {
            from_RLeg_3(state_model.q, swing_foot_posture.data(), CoM_position, torso_orientation, out);
        }

        
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0, k = 0; j < N; ++j)
            {
                if (j != 9) // ignore the 9th line, which corresponds to the constraint
                {           // on the torso orientation (rotation around x axis)
                    A(k,i) = out[i*N+j];
                    ++k;
                }
            }
        }
        for (int i = 0, j = 0; i < N; ++i)
        {
            if (i != 9) // ignore the 9th element, which corresponds to the constraint
            {           // on the torso orientation (rotation around x axis)
                err(j) = out[N*N+i];
                ++j;
            }
        }


        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
        {
            state_model.q[i] += dq[i];
        }

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i < N; i++)
        {
            norm_dq += dq[i]*dq[i];
        }
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}
