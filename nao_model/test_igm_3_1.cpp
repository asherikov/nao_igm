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

    struct timeval start, end;
    double cTime;
    int test_N = 1000;


    // set initial configuration
    double *q0 = new double[nao.state_var_num];
    double foot_position[3] = {0.0, 0.05, 0.0};
    double foot_orientation[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0};

    nao.init (IGM_SUPPORT_RIGHT, foot_position, foot_orientation);

    for (int i=0; i<nao.state_var_num; i++)
        q0[i] = nao.q[i];


    double LegT[POSTURE_MATRIX_SIZE];
    nao.PostureOffset(nao.swing_foot_posture, LegT, -0.02,0.01,0.02,0.1,0.1,0.1); // some offset
    nao.RotationOffset(nao.torso_orientation, nao.torso_orientation, 0.1,-0.1,0.1); // some offset
    nao.CoM_position[0] += 0.03;
    nao.CoM_position[1] += 0.02;
    nao.CoM_position[2] -= 0.01;


    int iter;
    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
    {
        for (int j=0; j<nao.state_var_num; j++) // every time start from q0
            nao.q[j] = q0[j];

        iter = nao.igm_3(nao.support_foot, LegT, nao.CoM_position, nao.torso_orientation);
    }
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (igm_3) = % f\n", cTime/test_N);

    cout << "iter = " << iter << endl;

    MatrixPrint(1,12,nao.q,"q");

    delete q0;

    return 0;
}

