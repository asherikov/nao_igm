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

#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT

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
                const double, const double, const double,
                const double, const double, const double);

        void switchSupportFoot ();
        void setCoM (const double, const double, const double);

        void getFeetPositions (double *, double *, double *, double *);

        void getCoM (jointState&, double *);
        void getSwingFootPosture (jointState&, double *);

        int igm (const double*, const double, const double, const int);



        igmSupportFoot support_foot;

        jointState state_model;
        jointState state_sensor;

        Transform<double,3> left_foot_posture;
        Transform<double,3> right_foot_posture;

        double CoM_position[POSITION_VECTOR_SIZE];
};

#endif // NAO_IGM_H
