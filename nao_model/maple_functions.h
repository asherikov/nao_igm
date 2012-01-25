/**
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:18:26 MSK
 */


#ifndef MAPLE_FUNCTIONS_H
#define MAPLE_FUNCTIONS_H


/****************************************
 * PROTOTYPES 
 ****************************************/

extern "C" {
  void from_LLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);

  void LLeg2RLeg(double *q,double *T);
  void LLeg2LHand(double *q,double *T);
  void LLeg2RHand(double *q,double *T);
  void LLeg2Head(double *q,double *T);
  void LLeg2Torso(double *q,double *T);
  void LLeg2CoM(double *q,double *A);
  void RLeg2LLeg(double *q,double *T);
  void RLeg2LHand(double *q,double *T);
  void RLeg2RHand(double *q,double *T);
  void RLeg2Head(double *q,double *T);
  void RLeg2Torso(double *q,double *T);
  void RLeg2CoM(double *q,double *A);
}

#endif //MAPLE_FUNCTIONS_H

