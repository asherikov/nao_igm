#ifndef JOINTS_SENSORS_ID_H
#define JOINTS_SENSORS_ID_H


/****************************************
 * TYPEDEFS 
 ****************************************/

enum jointSensorIDs {
    // LEFT LEG
    L_HIP_YAW_PITCH     = 0 ,
    L_HIP_ROLL          = 1 ,
    L_HIP_PITCH         = 2 ,
    L_KNEE_PITCH        = 3 ,
    L_ANKLE_PITCH       = 4 ,
    L_ANKLE_ROLL        = 5 ,

    // RIGHT LEG
    R_HIP_YAW_PITCH     = 6 ,
    R_HIP_ROLL          = 7 ,
    R_HIP_PITCH         = 8 ,
    R_KNEE_PITCH        = 9 ,
    R_ANKLE_PITCH       = 10,
    R_ANKLE_ROLL        = 11,

    // The number of joints in the lower body.
    LOWER_JOINTS_NUM    = 12,

    // LEFT ARM
    L_SHOULDER_PITCH    = 12,
    L_SHOULDER_ROLL     = 13,
    L_ELBOW_YAW         = 14,
    L_ELBOW_ROLL        = 15,
    L_WRIST_YAW         = 16,

    // RIGHT ARM
    R_SHOULDER_PITCH    = 17,
    R_SHOULDER_ROLL     = 18,
    R_ELBOW_YAW         = 19,
    R_ELBOW_ROLL        = 20,
    R_WRIST_YAW         = 21,

    // HEAD
    HEAD_PITCH          = 22,
    HEAD_YAW            = 23,

    JOINTS_NUM          = 24
};

#endif // JOINTS_SENSORS_ID_H

