/**
 * @file
 * @author Alexander Sherikov
 */


#include "nao_igm.h"
#include "maple_functions.h"


double jointState::q_lower_bound[JOINTS_NUM];
double jointState::q_upper_bound[JOINTS_NUM];



/**
 * @brief Constructor
 */
jointState::jointState()
{
    initJointAngles();
    initBounds();
}



/**
 * @brief Check that all joint angles lie within bounds.
 *
 * @return -1 if all values are corrent, id of the first joint violating the
 * bounds.
 *
 * @attention No collision checks!
 */
int jointState::checkJointBounds()
{
    for (int i = 0; i < JOINTS_NUM; i++)
    {
        if ((q_lower_bound[i] > q[i]) || (q_upper_bound[i] < q[i]))
        {
            return (i);
        }
    }
    return (-1);
}



/** @brief Sets the initial configuration of nao (lets call it the standard initial configuration)
    \note Only q[0]...q[23] are set. The posture of the base is not set.
*/
void jointState::initJointAngles()
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



/**
 * @brief Initialize bounds.
 */
void jointState::initBounds()
{
    // LEFT LEG
    setBounds(L_HIP_YAW_PITCH , -1.145303,  0.740810);
    setBounds(L_HIP_ROLL      , -0.379472,  0.790477);
    setBounds(L_HIP_PITCH     , -1.773912,  0.484090);
    setBounds(L_KNEE_PITCH    , -0.092346,  2.112528);
    setBounds(L_ANKLE_PITCH   , -1.189516,  0.922747);
    setBounds(L_ANKLE_ROLL    , -0.769001,  0.397880);
                              
    // RIGHT LEG
    setBounds(R_HIP_YAW_PITCH , -1.145303,  0.740810);
    setBounds(R_HIP_ROLL      , -0.738321,  0.414754);
    setBounds(R_HIP_PITCH     , -1.772308,  0.485624);
    setBounds(R_KNEE_PITCH    , -0.103083,  2.120198);
    setBounds(R_ANKLE_PITCH   , -1.186448,  0.932056);
    setBounds(R_ANKLE_ROLL    , -0.388676,  0.785875);
                              
    // LEFT ARM
    setBounds(L_SHOULDER_PITCH, -2.085600,  2.085600);
    setBounds(L_SHOULDER_ROLL ,  0.008700,  1.649400);
    setBounds(L_ELBOW_YAW     , -2.085600,  2.085600);
    setBounds(L_ELBOW_ROLL    , -1.562100, -0.008700);
    setBounds(L_WRIST_YAW     , -1.823800,  1.823800);
                              
    // RIGHT ARM
    setBounds(R_SHOULDER_PITCH, -2.085600,  2.085600);
    setBounds(R_SHOULDER_ROLL , -1.649400, -0.008700);
    setBounds(R_ELBOW_YAW     , -2.085600,  2.085600);
    setBounds(R_ELBOW_ROLL    ,  0.008700,  1.562100);
    setBounds(R_WRIST_YAW     , -1.823800,  1.823800);
                              
    // HEAD                   
    setBounds(HEAD_PITCH      , -2.085700,  2.085700);
    setBounds(HEAD_YAW        , -0.672000,  0.514900);
}



/**
 * @brief Set bounds for a joint.
 *
 * @param[in] id id of the joint.
 * @param[in] lower_bound lower bound.
 * @param[in] upper_bound upper_bound.
 */
void jointState::setBounds (const jointSensorIDs id, const double lower_bound, const double upper_bound)
{
    q_lower_bound[id] = lower_bound;
    q_upper_bound[id] = upper_bound;
}
