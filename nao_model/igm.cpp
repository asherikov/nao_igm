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
        setSupportPosture (left_foot_position, 0.0, 0.0, left_foot_position[3]);
        swing_foot_posture.init (right_foot_position, 0.0, 0.0, right_foot_position[3]);
    }
    else
    {
        setSupportPosture (right_foot_position, 0.0, 0.0, right_foot_position[3]);
        swing_foot_posture.init (left_foot_position, 0.0, 0.0, left_foot_position[3]);
    }
}


/**
 * @brief Set support foot posture.
 *
 * @param[in] support_foot_position 3x1 vector of coordinates
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 */
void nao_igm::setSupportPosture (
        const double *support_foot_position, 
        const double roll,
        const double pitch,
        const double yaw)
{
    state_model.q[SUPPORT_FOOT_POS_START]     = support_foot_position[0];
    state_model.q[SUPPORT_FOOT_POS_START + 1] = support_foot_position[1];
    state_model.q[SUPPORT_FOOT_POS_START + 2] = support_foot_position[2];
    rpy2R (roll, pitch, yaw, &state_model.q[SUPPORT_FOOT_ORIENTATION_START]);

    for (int i = SUPPORT_FOOT_POS_START; i < STATE_VAR_NUM; i++)
    {
        state_sensor.q[i] = state_model.q[i];
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
    state_sensor.q[SUPPORT_FOOT_POS_START]     = x;
    state_sensor.q[SUPPORT_FOOT_POS_START + 1] = y;
    state_sensor.q[SUPPORT_FOOT_POS_START + 2] = z;
    rpy2R (roll, pitch, yaw, &state_sensor.q[SUPPORT_FOOT_ORIENTATION_START]);


    for (int i = 0; i < STATE_VAR_NUM; i++)
    {
        state_model.q[i] = state_sensor.q[i];
    }
    state_model.support_foot = state_sensor.support_foot = support_foot_;


    posture torso_posture;
    if (state_model.support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(state_model.q, swing_foot_posture.data);
        LLeg2Torso(state_model.q, torso_posture.data);
        LLeg2CoM(state_model.q, CoM_position);
    }
    else
    {
        RLeg2LLeg(state_model.q, swing_foot_posture.data);
        RLeg2Torso(state_model.q, torso_posture.data);
        RLeg2CoM(state_model.q, CoM_position);
    }
    torso_posture.getOrientation(torso_orientation);
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
    posture swing_foot_posture_sensor;
    state_sensor.getSwingFootPosture (swing_foot_posture_sensor.data);
    swing_foot_posture.getPositionDiff (swing_foot_posture_sensor, position_error);


    swing_foot_posture_sensor.getPosition (&state_sensor.q[SUPPORT_FOOT_POS_START]);
    // reset z position
    state_sensor.q[SUPPORT_FOOT_POS_START + 2] = 0.0; 
    swing_foot_posture_sensor.getOrientation (&state_sensor.q[SUPPORT_FOOT_ORIENTATION_START]);


    if (state_model.support_foot == IGM_SUPPORT_LEFT)
    {
        state_model.support_foot = state_sensor.support_foot = IGM_SUPPORT_RIGHT;
    }
    else
    {
        state_model.support_foot = state_sensor.support_foot = IGM_SUPPORT_LEFT;
    }

    for (int i = 0; i < STATE_VAR_NUM; i++)
    {
        state_model.q[i] = state_sensor.q[i];
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
    const int N = LOWER_JOINTS_NUM;

    double out[N*N+N];
    Eigen::VectorXd dq(N);
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
            from_LLeg_3(state_model.q, swing_foot_posture.data, CoM_position, torso_orientation, out);
        }
        else
        {
            from_RLeg_3(state_model.q, swing_foot_posture.data, CoM_position, torso_orientation, out);
        }

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

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
        for (int i=0; i<11; i++)
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
