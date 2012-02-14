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
    jointState q0;

    nao.init (IGM_SUPPORT_RIGHT, 
              0.0, 0.05, 0.0, 
              0.0, 0.0, 0.0);
    q0 = nao.state_model;


    nao.swing_foot_posture = 
        nao.swing_foot_posture *
        Translation<double,3>(-0.02,0.01,0.02) *
        AngleAxisd(0.1, Vector3d::UnitX()) *
        AngleAxisd(0.1, Vector3d::UnitY()) *
        AngleAxisd(0.1, Vector3d::UnitZ());

    Matrix3d::Map(nao.torso_orientation) = Matrix3d::Map(nao.torso_orientation)
                      * (AngleAxisd(0.1, Vector3d::UnitX())
                      *  AngleAxisd(-0.1, Vector3d::UnitY())
                      *  AngleAxisd(0.1, Vector3d::UnitZ())).toRotationMatrix();

    nao.CoM_position[0] += 0.03;
    nao.CoM_position[1] += 0.02;
    nao.CoM_position[2] -= 0.01;


    int iter;
    gettimeofday(&start,0);
    for (int i=0 ; i<test_N ; i++)
    {
        nao.state_model = q0;
        iter = nao.igm(nao.state_model);
    }
    gettimeofday(&end,0);
    cTime = end.tv_sec - start.tv_sec + 0.000001 * (end.tv_usec - start.tv_usec);
    printf(" time (igm) = % f\n", cTime/test_N);

    cout << "iter = " << iter << endl;

    MatrixPrint(1,12,nao.state_model.q,"q");

    int check_bounds = nao.state_model.checkJointBounds();
    if (check_bounds >= 0)
    {
        cout << "Bounds are violated! ID:" << check_bounds << endl;
    }
    else
    {
        cout << "Bounds are ok." << endl;
    }

    return 0;
}

