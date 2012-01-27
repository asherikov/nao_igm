/**
 * @file
 * @author Alexander Sherikov
 */


#include "nao_igm.h"
#include "maple_functions.h"


/**
 * @brief Constructor
 */
modelState::modelState()
{
    initJointAngles();
}



/**
 * @brief Compute position of the CoM from joint angles.
 *
 * @param[in,out] CoM_pos 3x1 vector of coordinates.
 */
void modelState::getCoM (double *CoM_pos)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2CoM(q, CoM_pos);
    }
    else
    {
        RLeg2CoM(q, CoM_pos);
    }
}



/**
 * @brief Compute position of the swing foot from joint angles.
 *
 * @param[in,out] swing_foot_position  3x1 vector of coordinates.
 */
void modelState::getSwingFootPosition (double * swing_foot_position)
{
    posture swing_foot_posture;

    getSwingFootPosture(swing_foot_posture.data);
    swing_foot_posture.getPosition (swing_foot_position);
}



/**
 * @brief Compute position of the swing foot from joint angles.
 *
 * @param[in,out] swing_foot_posture  homogeneous 4x4 matrix
 */
void modelState::getSwingFootPosture (double * swing_foot_posture)
{
    if (support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(q, swing_foot_posture);
    }
    else
    {
        RLeg2LLeg(q, swing_foot_posture);
    }
}


/** @brief Sets the initial configuration of nao (lets call it the standard initial configuration)
    \note Only q[0]...q[23] are set. The posture of the base is not set.
*/
void modelState::initJointAngles()
{
    // LEFT LEG
    q[L_HIP_YAW_PITCH] =  0.0;
    q[L_HIP_ROLL]      =  0.0;
    q[L_HIP_PITCH]     = -0.436332;
    q[L_KNEE_PITCH]    =  0.698132;
    q[L_ANKLE_PITCH]   = -0.349066;
    q[L_ANKLE_ROLL]    =  0.0;

    // RIGHT LEG
    q[R_HIP_YAW_PITCH] =  0.0;
    q[R_HIP_ROLL]      =  0.0;
    q[R_HIP_PITCH]     = -0.436332;
    q[R_KNEE_PITCH]    =  0.698132;
    q[R_ANKLE_PITCH]   = -0.349066;
    q[R_ANKLE_ROLL]    =  0.0;

    // LEFT ARM
    q[L_SHOULDER_PITCH] =  1.396263;
    q[L_SHOULDER_ROLL]  =  0.349066;
    q[L_ELBOW_YAW]      = -1.396263;
    q[L_ELBOW_ROLL]     = -1.047198;
    q[L_WRIST_YAW]      =  0.0;

    // RIGHT ARM
    q[R_SHOULDER_PITCH] =  1.396263;
    q[R_SHOULDER_ROLL]  = -0.349066;
    q[R_ELBOW_YAW]      =  1.396263;
    q[R_ELBOW_ROLL]     =  1.047198;
    q[R_WRIST_YAW]      =  0.0;

    // HEAD
    q[HEAD_PITCH] =  0.0;
    q[HEAD_YAW]   =  0.0;
}
