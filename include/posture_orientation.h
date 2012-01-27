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

class posture
{
    public:
        void init (
                const double *, 
                const double, 
                const double, 
                const double);

        void init (
                const double, 
                const double, 
                const double, 
                const double, 
                const double, 
                const double);



        void getPosition (double *);
        void getPositionDiff (const posture&, double *);
        void setOrientation (const double *);
        void getOrientation (double *);


        double data[POSTURE_MATRIX_SIZE];
};


void rpy2R (const double, const double, const double, double *);

#endif // POSTURE_ORIENTATION_H

