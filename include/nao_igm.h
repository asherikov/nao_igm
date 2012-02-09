/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:17:52 MSK
 */


#ifndef NAO_IGM_H
#define NAO_IGM_H

/****************************************
 * INCLUDES 
 ****************************************/

#include <Eigen/Geometry>
#include "joints_sensors_id.h"



/****************************************
 * TYPEDEFS 
 ****************************************/

using namespace Eigen;


enum igmSupportFoot {
    IGM_SUPPORT_RIGHT = 0,
    IGM_SUPPORT_LEFT = 1
};


enum supportFootPos {
    SUPPORT_FOOT_POS_START = 24,
    SUPPORT_FOOT_X = 24,
    SUPPORT_FOOT_Y = 25,
    SUPPORT_FOOT_Z = 26,
    SUPPORT_FOOT_POS_NUM = 3
};


enum supportFootOrientation {
    SUPPORT_FOOT_ORIENTATION_START = 27,
    SUPPORT_FOOT_ORIENTATION_NUM = 9
};


#define STATE_VAR_NUM  (JOINTS_NUM + SUPPORT_FOOT_POS_NUM + SUPPORT_FOOT_ORIENTATION_NUM)
#define ORIENTATION_MATRIX_SIZE 3*3
#define POSITION_VECTOR_SIZE 3



class modelState
{
    public:
        modelState();

        void getCoM (double *);
        void getSwingFootPosture (double *);
        void getSwingFootPosition (double *);
        void initJointAngles();
        int checkJointBounds();
        void initSupportPosture (
                const double,
                const double,
                const double,
                const double,
                const double,
                const double);
        void initSupportPosture (
                const double,
                const double,
                const double,
                const double);
        void copySupportPosture (const modelState &);


        double q[STATE_VAR_NUM];
        igmSupportFoot support_foot;
};



class nao_igm 
{
    public:
        void init (
                const igmSupportFoot,
                const double, 
                const double, 
                const double, 
                const double, 
                const double, 
                const double);


        void switchSupportFoot (double *);
        void setCoM (const double, const double, const double);
        void setFeetPostures (const double *, const double *);
        void getFeetPositions (double *, double *, double *, double *);

        int igm (modelState &);


        modelState state_model;
        modelState state_sensor;

        Transform<double,3> swing_foot_posture;
        double torso_orientation[ORIENTATION_MATRIX_SIZE];
        double CoM_position[POSITION_VECTOR_SIZE];
};

#endif // NAO_IGM_H
