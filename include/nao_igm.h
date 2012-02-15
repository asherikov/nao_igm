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


#define ORIENTATION_MATRIX_SIZE 3*3
#define POSITION_VECTOR_SIZE 3



class jointState
{
    public:
        jointState();

        void initJointAngles();
        int checkJointBounds();

        double q[JOINTS_NUM];


    private:
        void initBounds();
        void setBounds (const jointSensorIDs, const double, const double);

        static double q_lower_bound[JOINTS_NUM];
        static double q_upper_bound[JOINTS_NUM];
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
        void getSwingFootPosition (jointState&, double *);

        void getCoM (jointState&, double *);
        void getSwingFootPosture (jointState&, Transform<double,3>&);

        int igm ();
        igmSupportFoot support_foot;


        jointState state_model;
        jointState state_sensor;

        Transform<double,3> support_foot_posture;
        Transform<double,3> swing_foot_posture;

        double torso_orientation[ORIENTATION_MATRIX_SIZE];
        double CoM_position[POSITION_VECTOR_SIZE];
};

#endif // NAO_IGM_H
