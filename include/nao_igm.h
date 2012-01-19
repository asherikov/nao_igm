/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:17:52 MSK
@todo Remove different variants of igm.
 */


#ifndef NAO_IGM_H
#define NAO_IGM_H

/****************************************
 * INCLUDES 
 ****************************************/

#include "joints_sensors_id.h"

/****************************************
 * DEFINES
 ****************************************/

#define POSTURE_MATRIX_SIZE 4*4
#define ORIENTATION_MATRIX_SIZE 3*3
#define POSITION_VECTOR_SIZE 3


/****************************************
 * TYPEDEFS 
 ****************************************/

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


class modelState
{
    public:
        modelState();

        void getCoM (double *);
        void getSwingFoot (double *);
        void initJointAngles();


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
        void init (
                const igmSupportFoot,
                const double, 
                const double, 
                const double, 
                const double*);


        void switchSupportFoot();
        int checkJointBounds();
        void setCoM (const double, const double, const double);
        void setSwingFootPosture (const double *, const double, const double, const double);

#ifdef ENABLE_ALL_IGM_VERSIONS
        int igm_1(double *, double *);
        int igm_2(double *, double *, double *);
#endif

        int igm_3(double *, double *, double *);
        int igm_4(double *, double *, double *, const double *, const double);


        modelState state;


        double swing_foot_posture[POSTURE_MATRIX_SIZE];
        double torso_orientation[ORIENTATION_MATRIX_SIZE];
        double CoM_position[POSITION_VECTOR_SIZE];
};

#endif // NAO_IGM_H

