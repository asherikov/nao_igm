#include <Eigen/Core>       // Cholesky decomposition + solving
#include <Eigen/Cholesky>   // of system of linear equations.

#include "nao_igm.h"
#include "maple_functions.h"
#include "joint_bounds.h"
#include "posture_orientation.h"



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
    return(joint_bounds.check(state.q));
}



/**
 * @brief Set position in a 4x4 homogeneous matrix.
 *
 * @param[in,out] Tc 4x4 homogeneous matrix.
 * @param[in] position 3x1 vector of coordinates.
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 */
void nao_igm::setSwingFootPosture (
        const double *position,
        const double roll,
        const double pitch,
        const double yaw)
{
    initPosture (swing_foot_posture, position, roll, pitch, yaw);
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
 * @param[in] x x-position
 * @param[in] y y-position
 * @param[in] z z-position
 * @param[in] sup_orientation orientation of the supprt foot.
 */
void nao_igm::init(
        const igmSupportFoot support_foot_, 
        const double x, 
        const double y, 
        const double z, 
        const double *sup_orientation)
{
    state.q[SUPPORT_FOOT_POS_START]     = state_sensor.q[SUPPORT_FOOT_POS_START]     = x;
    state.q[SUPPORT_FOOT_POS_START + 1] = state_sensor.q[SUPPORT_FOOT_POS_START + 1] = y;
    state.q[SUPPORT_FOOT_POS_START + 2] = state_sensor.q[SUPPORT_FOOT_POS_START + 2] = z;


    for (int i = SUPPORT_FOOT_ORIENTATION_START; 
            i < SUPPORT_FOOT_ORIENTATION_START + SUPPORT_FOOT_ORIENTATION_NUM; 
            i++)
    {
        state.q[i] = state_sensor.q[i] = sup_orientation[i - SUPPORT_FOOT_ORIENTATION_START];
    }

    for (int i = 0; i < JOINTS_NUM; i++)
    {
        state_sensor.q[i] = state.q[i];
    }

    state.support_foot = support_foot_;
    double torso_posture[POSTURE_MATRIX_SIZE];
    if (state.support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(state.q, swing_foot_posture);
        LLeg2Torso(state.q, torso_posture);
        LLeg2CoM(state.q, CoM_position);
    }
    else
    {
        RLeg2LLeg(state.q, swing_foot_posture);
        RLeg2Torso(state.q, torso_posture);
        RLeg2CoM(state.q, CoM_position);
    }
    T2Rot(torso_posture, torso_orientation);
}



/** @brief Initialize model.

    @param[in] support_foot_ current support foot.
    @param[in] x x-position
    @param[in] y y-position
    @param[in] z z-position
    @param[in] alpha x-rotation
    @param[in] beta y-rotation
    @param[in] gamma z-rotation
*/
void nao_igm::init(
        const igmSupportFoot support_foot_, 
        const double x, 
        const double y, 
        const double z, 
        const double alpha, 
        const double beta, 
        const double gamma)
{
    double Rot[ORIENTATION_MATRIX_SIZE];
    Euler2Rot(alpha, beta, gamma, Rot);
    init(support_foot_, x, y, z, Rot);
}



/**
 * @brief Switch support foot.
 */
void nao_igm::switchSupportFoot()
{
    if (state.support_foot == IGM_SUPPORT_LEFT)
    {
        state.support_foot = IGM_SUPPORT_RIGHT;
    }
    else
    {
        if (state.support_foot == IGM_SUPPORT_RIGHT)
        {
            state.support_foot = IGM_SUPPORT_LEFT;
        }
    }

    for (int i = SUPPORT_FOOT_POS_START; 
            i < SUPPORT_FOOT_POS_START + SUPPORT_FOOT_POS_NUM; 
            i++)
    {
        state.q[i] = state_sensor.q[i] = swing_foot_posture[12 + i - SUPPORT_FOOT_POS_START];
    }

    T2Rot (swing_foot_posture, &state.q[SUPPORT_FOOT_ORIENTATION_START]);
    T2Rot (swing_foot_posture, &state_sensor.q[SUPPORT_FOOT_ORIENTATION_START]);
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
        if (state.support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_3(state.q, swing_foot_posture, CoM_position, torso_orientation, out);
        else
            from_RLeg_3(state.q, swing_foot_posture, CoM_position, torso_orientation, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
            state.q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}
