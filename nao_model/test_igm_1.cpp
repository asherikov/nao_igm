// test_igm_1.cpp
//
// Inverse Geometric Model imposing constraints on
// (x, y, z, X(alpha), Y(beta), Z(gamma)) for the foot not in support and
// (x, y, z, X(alpha), Y(beta)) for the torso
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
    //const int nR = 3; // when using Euler angles
    //const int nR = 4; // when using unit quaternion
    const int nR = 9; // when using rotation matrix

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

    double LegT[4*4];
    double TorsoT[4*4];
    double Tc[4*4];
    int iter;
    igmSupportFoot support_foot = IGM_SUPPORT_RIGHT;

    if (support_foot) // LLeg is the base
    {
        LLeg2RLeg(q,Tc);
        PostureOffset(Tc, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset

        LLeg2Torso(q,Tc);
        PostureOffset(Tc, TorsoT, 0.01,0.01,-0.02,0.1,0.1,-0.1); // some offset

        //MatrixPrint(4,4,LegT,"LegT");
        //MatrixPrint(4,4,TorsoT,"TorsoT");

        iter = igm_1(support_foot, LegT, TorsoT, q);
    }
    else // RLeg is the base
    {
        RLeg2LLeg(q,Tc);
        PostureOffset(Tc, LegT, 0.01,0.01,0.01,0.1,0.1,0.1); // some offset

        RLeg2Torso(q,Tc);
        PostureOffset(Tc, TorsoT, -0.02,0.02,0.01,0.1,-0.1,0.1); // some offset

        //MatrixPrint(4,4,LegT,"LegT");
        //MatrixPrint(4,4,TorsoT,"TorsoT");

        iter = igm_1(support_foot, LegT, TorsoT, q);
    }

    MatrixPrint(1,12,q,"q");
    cout << "iter = " << iter << endl;

    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
    {
        for (int j=0; j<nJ+3+nR; j++) // every time start from q0
            q[j] = q0[j];

        iter = igm_1(support_foot, LegT, TorsoT, q);
    }
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (igm_1) = % f\n", cTime/test_N);

    return 0;
}

