/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:18:26 MSK
 */


#ifndef LEG2JOINTS_H
#define LEG2JOINTS_H

/****************************************
 * PROTOTYPES 
 ****************************************/

extern "C" {
  void LLeg2Joints(double *q, double *A);
  void RLeg2Joints(double *q, double *A);
}

#endif //LEG2JOINTS_H

