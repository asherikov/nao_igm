// test_igm_2.cpp
//
// Inverse Geometric Model imposing constraints on
// (x, y, z, X(alpha), Y(beta), Z(gamma)) for the foot not in support,
// (x, y, z) for CoM position and
// X(alpha), Y(beta) for the torso orientation
//
// [2011/11/26]

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <sys/time.h>
#include <time.h>

#include "../include/nao.h"

using namespace std;

int main(int argc, char** argv)
{

    struct timeval start, end;
    double cTime;
    int test_N = 1000;

    const int nJ = 24; // number of joints
    const int nR = 9;  // using a rotation matrix

    // set initial configuration
    double q[nJ+3+nR];
    double q0[nJ+3+nR];

    InitJointAngles(q0);

    q0[24] = 0.0;
    q0[25] = 0.05;
    q0[26] = 0.0;

    q0[27] = 1.0;
    q0[30] = 0.0;
    q0[33] = 0.0;
    q0[28] = 0.0;
    q0[31] = 1.0;
    q0[34] = 0.0;
    q0[29] = 0.0;
    q0[32] = 0.0;
    q0[35] = 1.0;

    for (int i=0; i<nJ+3+nR; i++)
        q[i] = q0[i];

    double CoM[3];
    double RotTorso[3*3];
    double LegT[4*4];
    double Tc[4*4];
    int iter;
    int case_flag = 0;

    if (case_flag)
    {
        LLeg2RLeg(q,Tc);
        PostureOffset(Tc, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset

        LLeg2Torso(q,Tc);
        T2Rot(Tc, RotTorso);
        RotationOffset(RotTorso, RotTorso, 0.1,-0.1,0.1); // some offset

        LLeg2CoM(q,CoM);
        CoM[0] += 0.03;
        CoM[1] += 0.02;
        CoM[2] -= 0.01;
    }
    else
    {
        RLeg2LLeg(q,Tc);
        PostureOffset(Tc, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset

        RLeg2Torso(q,Tc);
        T2Rot(Tc, RotTorso);
        RotationOffset(RotTorso, RotTorso, 0.1,-0.1,0.1); // some offset

        RLeg2CoM(q,CoM);
        CoM[0] += 0.03;
        CoM[1] += 0.02;
        CoM[2] -= 0.01;
    }

    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
    {
        for (int j=0; j<nJ+3+nR; j++) // every time start from q0
            q[j] = q0[j];

        iter = igm_2(case_flag, LegT, CoM, RotTorso, q);
    }
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (igm_2) = % f\n", cTime/test_N);

    cout << "iter = " << iter << endl;

    return 0;
}

