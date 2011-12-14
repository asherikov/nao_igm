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


    nao.q[0]  = 0.0014265491477135045;
    nao.q[1]  = -0.41732929046562739;
    nao.q[2]  = 28.746025296929801;
    nao.q[3]  = -46.548684804152458;
    nao.q[4]  = 17.706190733695742;
    nao.q[5]  = 0.4156953589200087;
    nao.q[6]  = 0.0014265491492418785;
    nao.q[7]  = -0.39761540860406758;
    nao.q[8]  = 38.839406285499955;
    nao.q[9]  = -70.521411965286276;
    nao.q[10] = 31.58631023642333;
    nao.q[11] = 0.39589342309323805;
    nao.q[12] = 1.396263;
    nao.q[13] = 0.34906599999999999;
    nao.q[14] = -1.396263;
    nao.q[15] = -1.0471980000000001;
    nao.q[16] = 0;
    nao.q[17] = 1.396263;
    nao.q[18] = -0.34906599999999999;
    nao.q[19] = 1.396263;
    nao.q[20] = 1.0471980000000001;
    nao.q[21] = 0;
    nao.q[22] = 0;
    nao.q[23] = 0;

    nao.q[24] = 0;
    nao.q[25] = -0.050000000000000003;
    nao.q[26] = 0;

    nao.q[27] = 1;
    nao.q[28] = 0;
    nao.q[29] = 0;
    nao.q[30] = 0;
    nao.q[31] = 1;
    nao.q[32] = 0;
    nao.q[33] = 0;
    nao.q[34] = 0;
    nao.q[35] = 1;
                                                                       

    nao.swing_foot_posture[0]  = 1;
    nao.swing_foot_posture[1]  = 0;
    nao.swing_foot_posture[2]  = 0;
    nao.swing_foot_posture[3]  = 0;
    nao.swing_foot_posture[4]  = -0;
    nao.swing_foot_posture[5]  = 1;
    nao.swing_foot_posture[6]  = 0;
    nao.swing_foot_posture[7]  = 0 ;
    nao.swing_foot_posture[8]  = 0;
    nao.swing_foot_posture[9]  = -0;
    nao.swing_foot_posture[10] = 1;
    nao.swing_foot_posture[11] = 0;

    nao.swing_foot_posture[12] = 0;
    nao.swing_foot_posture[13] = 0.050000000000000003;
    nao.swing_foot_posture[14] = 0;
    nao.swing_foot_posture[15] = 1;
    
    
    nao.torso_orientation[0] = 0.99619473840986084;
    nao.torso_orientation[1] = -0;
    nao.torso_orientation[2] = -0.087155281908264007;
    nao.torso_orientation[3] = 0;
    nao.torso_orientation[4] = 1.0000000000000002;
    nao.torso_orientation[5] = 0;
    nao.torso_orientation[6] = 0.087155281908263924;
    nao.torso_orientation[7] = 0;
    nao.torso_orientation[8] = 0.99619473840986117;  
     
    nao.CoM_position[0] = -0.0109689996033003770;
    nao.CoM_position[1] = 0.058099459507841178;
    nao.CoM_position[2] = 0.2618665245671406;
   

    nao.support_foot = IGM_SUPPORT_RIGHT;


    int iter = nao.igm_3(nao.swing_foot_posture, nao.CoM_position, nao.torso_orientation);
    cout << "iter = " << iter << endl;

    return 0;
}

