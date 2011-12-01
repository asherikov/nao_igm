#define REF(A) &A(0)

#include "joints_sensors_id.h"

extern "C" {
  void from_LLeg_1(double *q, double *LetT, double *TorsoT, double *out);
  void from_RLeg_1(double *q, double *LetT, double *TorsoT, double *out);
  void from_LLeg_2(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_2(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_LLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_3(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_LLeg_4(double *q, double *LetT, double *CoM, double *RotTorso, double *out);
  void from_RLeg_4(double *q, double *LetT, double *CoM, double *RotTorso, double *out);

  void LLeg2Joints(double *q, double *A);
  void RLeg2Joints(double *q, double *A);

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

  void Euler2T(double x, double y, double z, double X, double Y, double Z, double *T);
  void Euler2Rot(double X, double Y, double Z, double *Rot);
}

void PostureOffset(double *Tc, double *Td, double x, double y, double z, double alpha, double beta, double gamma);
void RotationOffset(double *Tc, double *Td, double alpha, double beta, double gamma);
void T2Rot(double * T, double *Rot);
void SetBasePose(double *q, double x, double y, double z, double alpha, double beta, double gamma);

void InitJointAngles(double *q);
void MatrixPrint(unsigned int m, unsigned int n, double * A, const char * description);

int igm_1(int case_flag, double *LegT, double *TorsoT, double *q);
int igm_2(int case_flag, double *LegT, double *CoM, double *RotTorso, double *q);
int igm_3(int case_flag, double *LegT, double *CoM, double *RotTorso, double *q);
int igm_4(int case_flag, double *LegT, double *CoM, double *RotTorso, double *q, double *q0, double mu);
