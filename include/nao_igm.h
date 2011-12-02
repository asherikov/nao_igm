/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:17:52 MSK
@todo Add CheckJointLimits()
@todo Remove different variants of igm.
 */


#ifndef NAO_IGM_H
#define NAO_IGM_H

/****************************************
 * INCLUDES 
 ****************************************/



/****************************************
 * DEFINES
 ****************************************/


/****************************************
 * TYPEDEFS 
 ****************************************/

enum igmSupportFoot {
    IGM_SUPPORT_RIGHT = 0,
    IGM_SUPPORT_LEFT = 1
};


enum supportFootPos {
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

        void PostureOffset(double *Tc, double *Td, double x, double y, double z, double alpha, double beta, double gamma);
        void RotationOffset(double *Tc, double *Td, double alpha, double beta, double gamma);
        void T2Rot(double * T, double *Rot);
        void SetBasePose(double x, double y, double z, double alpha, double beta, double gamma);

        void InitJointAngles();


        int igm_1(igmSupportFoot support_foot, double *LegT, double *TorsoT);
        int igm_2(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso);
        int igm_3(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso);
        int igm_4(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso, double *q0, double mu);


        double *q;
        int state_var_num;
};



/****************************************
 * PROTOTYPES 
 ****************************************/

#endif // NAO_IGM_H

