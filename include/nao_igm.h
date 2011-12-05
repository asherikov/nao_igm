/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:17:52 MSK
@todo Add CheckJointLimits()
@todo Remove different variants of igm.
@todo Documentation.
@todo Add const qualifiers.
 */


#ifndef NAO_IGM_H
#define NAO_IGM_H

/****************************************
 * INCLUDES 
 ****************************************/



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
        ~nao_igm();

        void PostureOffset(double *, double *, double, double, double, double, double, double);
        void RotationOffset(double *, double *, double, double, double);
        void T2Rot(double *, double *);
        void SetBasePose(double, double, double, double, double, double);

        void init(igmSupportFoot, double *, double *);
        void switchSupportFoot();
        void initJointAngles();


        int igm_1(double *, double *);
        int igm_2(double *, double *, double *);
        int igm_3(double *, double *, double *);
        int igm_4(double *, double *, double *, double *, double);


        double *q;
        int state_var_num;
        double swing_foot_posture[POSTURE_MATRIX_SIZE];
        double torso_orientation[ORIENTATION_MATRIX_SIZE];
        double CoM_position[POSITION_VECTOR_SIZE];
        igmSupportFoot support_foot;
};



/****************************************
 * PROTOTYPES 
 ****************************************/

#endif // NAO_IGM_H

