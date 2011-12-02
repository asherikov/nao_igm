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

extern "C" {
    void JacCoM(double *q, double *J);
}

int main(int argc, char** argv)
{
    nao_igm nao;

    struct timeval start, end;
    double cTime;
    int test_N = 1000;

    const int nJ = 24; // number of joints
    const int nR = 9;  // using a rotation matrix

    // set initial configuration
    double *q0 = new double[nao.state_var_num];

    nao.InitJointAngles();
    nao.q[24] = 0.0;
    nao.q[25] = 0.05;
    nao.q[26] = 0.0;

    nao.q[27] = 1.0;
    nao.q[30] = 0.0;
    nao.q[33] = 0.0;
    nao.q[28] = 0.0;
    nao.q[31] = 1.0;
    nao.q[34] = 0.0;
    nao.q[29] = 0.0;
    nao.q[32] = 0.0;
    nao.q[35] = 1.0;

    for (int i=0; i<nao.state_var_num; i++)
        q0[i] = nao.q[i];

    double CoM[3];
    double RotTorso[3*3];
    double LegT[4*4];
    double Tc[4*4];
    int iter;
    igmSupportFoot support_foot = IGM_SUPPORT_RIGHT;

    if (support_foot)
    {
        LLeg2RLeg(nao.q,Tc);
        nao.PostureOffset(Tc, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset

        LLeg2Torso(nao.q,Tc);
        nao.T2Rot(Tc, RotTorso);
        nao.RotationOffset(RotTorso, RotTorso, 0.1,-0.1,0.1); // some offset

        LLeg2CoM(nao.q,CoM);
        CoM[0] += 0.03;
        CoM[1] += 0.02;
        CoM[2] -= 0.01;
    }
    else
    {
        RLeg2LLeg(nao.q,Tc);

        MatrixPrint(4,4,Tc,"Tc (p)");
        nao.PostureOffset(Tc, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset
        MatrixPrint(4,4,LegT,"Tc (a)");

        RLeg2Torso(nao.q,Tc);
        nao.T2Rot(Tc, RotTorso);
        MatrixPrint(3,3,RotTorso,"Rc (p)");
        nao.RotationOffset(RotTorso, RotTorso, 0.1,-0.1,0.1); // some offset
        MatrixPrint(3,3,RotTorso,"Rc (a)");

        RLeg2CoM(nao.q,CoM);
        CoM[0] += 0.03;
        CoM[1] += 0.02;
        CoM[2] -= 0.01;
    }

    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
    {
        for (int j=0; j<nJ+3+nR; j++) // every time start from q0
            nao.q[j] = q0[j];

        iter = nao.igm_3(support_foot, LegT, CoM, RotTorso);
    }
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (igm_3) = % f\n", cTime/test_N);

    cout << "iter = " << iter << endl;

    MatrixPrint(1,12,nao.q,"q");

    /*
    double J[3*24];
    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
      JacCoM(q,J);
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (Jacobian of CoM) = % f\n", cTime/test_N);
    */

    //MatrixPrint(3,12,J,"J CoM");
    delete q0;

    return 0;
}

