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
    void LLeg2RLeg(double *q,double *LL,double *T);
    void LLeg2LHand(double *q,double *LL,double *T);
    void LLeg2RHand(double *q,double *LL,double *T);
    void LLeg2Head(double *q,double *LL,double *T);
    void LLeg2CoM(double *q,double *LL,double *A);
    void RLeg2LLeg(double *q,double *RL,double *T);
    void RLeg2LHand(double *q,double *RL,double *T);
    void RLeg2RHand(double *q,double *RL,double *T);
    void RLeg2Head(double *q,double *RL,double *T);
    void RLeg2CoM(double *q,double *RL,double *A);


    void from_LLeg_3(double *q,double *LL,double *RL,double *CoM,double *A);
    void from_RLeg_3(double *q,double *RL,double *LL,double *CoM,double *A);
}

#endif //MAPLE_FUNCTIONS_H

