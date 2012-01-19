/**
 * @file
 * @author Alexander Sherikov
 * @date 19.01.2012 17:56:36 MSK
 */


#ifndef POSTURE_ORIENTATION_H
#define POSTURE_ORIENTATION_H

/****************************************
 * INCLUDES 
 ****************************************/



/****************************************
 * DEFINES
 ****************************************/



/****************************************
 * TYPEDEFS 
 ****************************************/



/****************************************
 * PROTOTYPES 
 ****************************************/

void initPosture (double *, const double *, const double, const double, const double);
void PostureOffset(
        const double *,
        double *,
        const double,
        const double,
        const double,
        const double,
        const double,
        const double);


void rpy2R(const double, const double, const double, double *);
void rpy2R_hom(const double, const double, const double, double *);


void RotationOffset(
        const double *,
        double *,
        const double,
        const double,
        const double);
void T2Rot(const double *, double *);


#endif // POSTURE_ORIENTATION_H

