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
  void from_LLeg_1(double *q, double *LetT, double *TorsoT, double *out);
  void from_RLeg_1(double *q, double *LetT, double *TorsoT, double *out);
  void from_LLeg_2(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_2(double *q, double *LetT, double *CoM, double *RotTorso, double *out);

  void from_LLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_LLeg_4(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_4(double *q, double *LetT, double *CoM, double *RotTorso, double *out);

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



extern "C" {
  void LLeg2Joints(double *q, double *A);
  void RLeg2Joints(double *q, double *A);
}


extern "C" {
  void Euler2T(double x, double y, double z, double X, double Y, double Z, double *T);
  void Euler2Rot(double X, double Y, double Z, double *Rot);
}


#endif //MAPLE_FUNCTIONS_H
