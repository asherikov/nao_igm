// test_igm_3.cpp
//
// Testing the Jacobian of the CoM. To include an igm function based on it.
//
// [2011/11/26]

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <sys/time.h>
#include <time.h>

#include "nao_igm.h"
#include "util.cpp"
#include "maple_functions.h"

using namespace std;


int main(int argc, char** argv)
{
    nao_igm nao;

    double init_joint_angles[JOINTS_NUM];

    init_joint_angles[L_HIP_YAW_PITCH]  =  0.0;
    init_joint_angles[R_HIP_YAW_PITCH]  =  0.0;

    init_joint_angles[L_HIP_ROLL]       = -0.000384;
    init_joint_angles[L_HIP_PITCH]      = -0.598291;
    init_joint_angles[L_KNEE_PITCH]     =  1.009413;
    init_joint_angles[L_ANKLE_PITCH]    = -0.492352;
    init_joint_angles[L_ANKLE_ROLL]     =  0.000469;

    init_joint_angles[R_HIP_ROLL]       = -0.000384;
    init_joint_angles[R_HIP_PITCH]      = -0.598219;
    init_joint_angles[R_KNEE_PITCH]     =  1.009237;
    init_joint_angles[R_ANKLE_PITCH]    = -0.492248;
    init_joint_angles[R_ANKLE_ROLL]     =  0.000469;

    init_joint_angles[L_SHOULDER_PITCH] =  1.418908;
    init_joint_angles[L_SHOULDER_ROLL]  =  0.332836;
    init_joint_angles[L_ELBOW_YAW]      = -1.379108;
    init_joint_angles[L_ELBOW_ROLL]     = -1.021602;
    init_joint_angles[L_WRIST_YAW]      = -0.013848;

    init_joint_angles[R_SHOULDER_PITCH] =  1.425128;
    init_joint_angles[R_SHOULDER_ROLL]  = -0.331386;
    init_joint_angles[R_ELBOW_YAW]      =  1.383626;
    init_joint_angles[R_ELBOW_ROLL]     =  1.029356;
    init_joint_angles[R_WRIST_YAW]      = -0.01078;

    init_joint_angles[HEAD_PITCH]       =  0.0;
    init_joint_angles[HEAD_YAW]         =  0.0;

    for (int i = 0; i < JOINTS_NUM; i++)
    {
        nao.q[i] = init_joint_angles[i];
    }


    double foot_position[POSITION_VECTOR_SIZE] = {0.0, -0.05, 0.0};
    double foot_orientation[ORIENTATION_MATRIX_SIZE] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0};

    nao.init (
            IGM_SUPPORT_RIGHT,
            foot_position,
            foot_orientation);
   

    int iter = nao.igm_3(nao.swing_foot_posture, nao.CoM_position, nao.torso_orientation, init_joint_angles, 1.0);
    cout << "iter = " << iter << endl;

    return 0;
}

