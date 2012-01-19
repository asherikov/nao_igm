/**
 * @file
 * @author Alexander Sherikov
 */


#ifndef JOINT_BOUNDS_H
#define JOINT_BOUNDS_H

/****************************************
 * INCLUDES 
 ****************************************/

#include "joints_sensors_id.h"


/****************************************
 * TYPEDEFS 
 ****************************************/


class jointBounds
{
    public:
        jointBounds ();
        int check (const double *);

    private:
        double q_lower_bound[JOINTS_NUM];
        double q_upper_bound[JOINTS_NUM];
        void setBounds (const jointSensorIDs, const double, const double);
};

extern jointBounds joint_bounds;

#endif // JOINT_BOUNDS_H

