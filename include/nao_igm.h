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


class nao_igm 
{
    public:
        nao_igm();

        void PostureOffset(
                const double *, 
                double *, 
                const double, 
                const double, 
                const double, 
                const double, 
                const double, 
                const double);
        void RotationOffset(
                const double *, 
                double *, 
                const double, 
                const double, 
                const double);
        void T2Rot(const double *, double *);
        void SetBasePose(
                const double, 
                const double, 
                const double, 
                const double, 
                const double, 
                const double);

        void init(const igmSupportFoot, const double *, const double *);
        void switchSupportFoot();
        void initJointAngles();
        int checkJointBounds();
        void initPosture (double *, const double *, const double, const double, const double);
        void rpy2R(const double, const double, const double, double *);
        void rpy2R_hom(const double, const double, const double, double *);
        void setCoM (const double, const double, const double);
        void getUpdatedCoM (double *);
        void getUpdatedSwingFoot (double *);


        int igm_1(double *, double *);
        int igm_2(double *, double *, double *);
        int igm_3(double *, double *, double *);
        int igm_4(double *, double *, double *, const double *, const double);


        double q[JOINTS_NUM + SUPPORT_FOOT_POS_NUM + SUPPORT_FOOT_ORIENTATION_NUM];
        double q_lower_bound[JOINTS_NUM];
        double q_upper_bound[JOINTS_NUM];

        int state_var_num;
        double swing_foot_posture[POSTURE_MATRIX_SIZE];
        double torso_orientation[ORIENTATION_MATRIX_SIZE];
        double CoM_position[POSITION_VECTOR_SIZE];
        igmSupportFoot support_foot;

    private:
        void setBounds (const jointSensorIDs, const double, const double);
};



/****************************************
 * PROTOTYPES 
 ****************************************/

#endif // NAO_IGM_H

