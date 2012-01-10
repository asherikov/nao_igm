/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:18:26 MSK
 */


#ifndef PUBLIC_MAPLE_FUNCTIONS_H
#define PUBLIC_MAPLE_FUNCTIONS_H

/****************************************
 * PROTOTYPES 
 ****************************************/

extern "C" {
  void Euler2T(double x, double y, double z, double X, double Y, double Z, double *T);
  void Euler2Rot(double X, double Y, double Z, double *Rot);
}

#endif //PUBLIC_MAPLE_FUNCTIONS_H

