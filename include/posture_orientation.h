/**
 * @file
 * @author Alexander Sherikov
 * @date 19.01.2012 17:56:36 MSK
 */


#ifndef POSTURE_ORIENTATION_H
#define POSTURE_ORIENTATION_H


/****************************************
 * DEFINES
 ****************************************/

#define POSTURE_MATRIX_SIZE 4*4
#define ORIENTATION_MATRIX_SIZE 3*3
#define POSITION_VECTOR_SIZE 3



/****************************************
 * PROTOTYPES 
 ****************************************/

void initPosture (
        const double *, 
        const double, 
        const double, 
        const double, 
        double *);

void initPosture (
        const double, 
        const double, 
        const double, 
        const double, 
        const double, 
        const double, 
        double *);

void PostureOffset(
        const double *,
        const double,
        const double,
        const double,
        const double,
        const double,
        const double,
        double *);


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

