/**
 * @file
 * @author Alexander Sherikov
 */


#include "joint_bounds.h"


jointBounds joint_bounds;


/**
 * @brief Constructor
 */
jointBounds::jointBounds()
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
void jointBounds::setBounds (const jointSensorIDs id, const double lower_bound, const double upper_bound)
{
    q_lower_bound[id] = lower_bound;
    q_upper_bound[id] = upper_bound;
}



/**
 * @brief Check that all joint angles lie within bounds.
 *
 * @param[in] q joint angles.
 *
 * @return -1 if all values are corrent, id of the first joint violating the
 * bounds.
 *
 * @attention No collision checks!
 */
int jointBounds::check(const double *q)
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
