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



    nao.q[0]  = -2.7676870406486114e-05;
    nao.q[1]  = 0.032806022566075822;
    nao.q[2]  = -0.81030421414459242;
    nao.q[3]  = 0.56882095159942125;
    nao.q[4]  = 0.15419009646993775;
    nao.q[5]  = -0.03268290396557879;
    nao.q[6]  = -2.7676870986320282e-05;
    nao.q[7]  = 0.032359010861538688;
    nao.q[8]  = -0.64372298049772814;
    nao.q[9]  = 0.53301994524106533;
    nao.q[10] = 0.023411144269111651;
    nao.q[11] = -0.032234183341070845;
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

    nao.q[24] = 0.14000000000000001;
    nao.q[25] = -0.050000000000000003;
    nao.q[26] = 0;

    nao.q[27] = 1;
    nao.q[28] = 0;
    nao.q[29] = 0;
    nao.q[30] = -0;
    nao.q[31] = 1;
    nao.q[32] = 0;
    nao.q[33] = 0;
    nao.q[34] = -0;
    nao.q[35] = 1;
                                                         

    nao.swing_foot_posture[0]  = 1;
    nao.swing_foot_posture[1]  = 0;
    nao.swing_foot_posture[2]  = 0;
    nao.swing_foot_posture[3]  = 0;
    nao.swing_foot_posture[4]  = -0;
    nao.swing_foot_posture[5]  = 1;
    nao.swing_foot_posture[6]  = 0;
    nao.swing_foot_posture[7]  = 0;
    nao.swing_foot_posture[8]  = 0;
    nao.swing_foot_posture[9]  = -0;
    nao.swing_foot_posture[10] = 1;
    nao.swing_foot_posture[11] = 0;

    nao.swing_foot_posture[12] = 0.17150000000000001;
    nao.swing_foot_posture[13] = 0.050000000000000003;
    nao.swing_foot_posture[14] = 0.0025650000000000091;
    nao.swing_foot_posture[15] = 1;

    
    nao.torso_orientation[0] = 0.99618568308522459;
    nao.torso_orientation[1] = -0.0042475465289972536;
    nao.torso_orientation[2] = -0.087155281908261634;
    nao.torso_orientation[3] = 0.0042637712941319544;
    nao.torso_orientation[4] = 0.99999091008586261;
    nao.torso_orientation[5] = 5.5511151231257827e-17;
    nao.torso_orientation[6] = 0.087154489674232435;
    nao.torso_orientation[7] = -0.00037161018913245414;
    nao.torso_orientation[8] = 0.99619473840986128;
                                                          
    nao.CoM_position[0] = 0.12117274733661604;
    nao.CoM_position[1] = -0.0045745286535708507;
    nao.CoM_position[2] = 0.2618665245671406;
   

    nao.support_foot = IGM_SUPPORT_RIGHT;


    int iter = nao.igm_3(nao.swing_foot_posture, nao.CoM_position, nao.torso_orientation);
    cout << "iter = " << iter << endl;

    return 0;
}

