#include <iostream>

#include <stdio.h>
#include <math.h>

#include "../include/nao.h"

#include <Eigen/Core>
#include <Eigen/Cholesky>


/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z, X(alpha), Y(beta)) of the torso can be
    imposed.

    \verbatim
    The following problem is solver repeatedly:
    minimize    0.5*dq'*dq
    subject to  A(q)*dq = [err(q);0].

    Where:
          A(q) = [J1(q); J2(q); 1 0 0 0 0 0 -1 0 0 0 0 0] is a 12x12 matrix
                   J1(q) is the 6x12 Jacobian of (x, y, z, X(alpha), Y(beta), Z(gamma)) of the leg not in support w.r.t the motion of q[0]...q[11].
                  J2(q) is the 5x12 Jacobian of (x, y, z, X(alpha), Y(beta)) of the Torso w.r.t the motion of q[0]...q[11].
                  [1 0 0 0 0 0 -1 0 0 0 0 0] is used to define a constraint q[0] = q[6].

          err(q) is a 11-vector representing an error from the current and desired postures of the foot not in support and Torso.
          err(q) = [err_LegNotInSupport_xPosition;
                    err_LegNotInSupport_yPosition;
                    err_LegNotInSupport_zPosition;
                    err_LegNotInSupport_xRotation;
                    err_LegNotInSupport_yRotation;
                    err_LegNotInSupport_zRotation;
                    err_Torso_xPosition;
                    err_Torso_yPosition;
                    err_Torso_zPosition;
                    err_Torso_xRotation;
                    err_Torso_yRotation]
    \endverbatim

    \param[in] support_foot the support leg.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] TorsoT A 4x4 homogeneous matrix specifying the desired posture of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \param[in,out] q On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.

    \note It is assumed that the leading matrix of the constraints (A) is nonsingular.

    \par

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

*/
int igm_1(igmSupportFoot support_foot, double *LegT, double *TorsoT, double *q)
{

    const int N = 12;

    double out[N*N+N];
    Eigen::VectorXd dq(N);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 20;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {
        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_1(q, LegT, TorsoT, out);
        else
            from_RLeg_1(q, LegT, TorsoT, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<12; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            printf("The IK routine did not converge in %i iterations. \n", max_iter);
            iter = -1;
        }
    }

    return(iter);
}



/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \param[in] support_foot the support leg.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] CoM Desired position of the Center of Mass in the world frame.

    \param[in] RotTorso A 3x3 rotation matrix specifying the desired orientation of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \param[in,out] q On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    \par

    \note J2(1:3,:) is the Jacobian of the Torso position (not the CoM position) w.r.t. joints 0..11.
          Even though this is incorrect, the results seem to be reasonable
          (the real Jacobian of the CoM is used in igm_3(...)).

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note For more details see igm_1

*/
int igm_2(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso, double *q)
{

    const int N = 12;

    double out[N*N+N];
    Eigen::VectorXd dq(N);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 20;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {
        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_2(q, LegT, CoM, RotTorso, out);
        else
            from_RLeg_2(q, LegT, CoM, RotTorso, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<12; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            printf("The IK routine did not converge in %i iterations. \n", max_iter);
            iter = -1;
        }
    }

    return(iter);
}



/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \param[in] support_foot the support leg.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] CoM Desired position of the Center of Mass in the world frame.

    \param[in] RotTorso A 3x3 rotation matrix specifying the desired orientation of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \param[in,out] q On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    \par

    \note The difference with igm_3(...) is that here the real Jacobian of the CoM is used

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note For more details see igm_1

*/
int igm_3(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso, double *q)
{

    const int N = 12;

    double out[N*N+N];
    Eigen::VectorXd dq(N);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 20;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {
        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_3(q, LegT, CoM, RotTorso, out);
        else
            from_RLeg_3(q, LegT, CoM, RotTorso, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<12; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            printf("The IK routine did not converge in %i iterations. \n", max_iter);
            iter = -1;
        }
    }

    return(iter);
}



int igm_4(igmSupportFoot support_foot, double *LegT, double *CoM, double *RotTorso, double *q, double *q0, double mu)
{

    const int M = 12;
    const int N = 24;

    double out[M*N+M];
    Eigen::VectorXd dq(N);
    Eigen::VectorXd z(N);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 100;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {

        for (int i=0; i<N; i++)
            z(i) = mu*(q[i] - q0[i]);

        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_4(q, LegT, CoM, RotTorso, out);
        else
            from_RLeg_4(q, LegT, CoM, RotTorso, out);

        Eigen::Map<Eigen::MatrixXd> A(out,M,N);
        Eigen::Map<Eigen::VectorXd> err(out+M*N,M);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err-A*z, &dq);
        dq = -z-A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<N; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<N; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            printf("The IK routine did not converge in %i iterations. \n", max_iter);
            iter = -1;
        }
        std::cout << "iter = " << iter << " norm_dq = " << norm_dq << std::endl;
    }

    return(iter);
}



// ===================================================================================================
// Utility functions
// ===================================================================================================

/** \brief Sets the pose of the base (of NAO)

    \param[out] q joint angles (and base parameters) vector (double q[24+3+9] is assumed)

    \param[in] x x-position
    \param[in] y y-position
    \param[in] z z-position
    \param[in] alpha x-rotation
    \param[in] beta y-rotation
    \param[in] gamma z-rotation

    \return void

*/
void SetBasePose(double *q, double x, double y, double z, double alpha, double beta, double gamma)
{
    double Rot[3*3];

    Euler2Rot(alpha, beta, gamma, Rot);
    q[24] = x;
    q[25] = y;
    q[26] = z;

    q[27] = Rot[0];
    q[30] = Rot[3];
    q[33] = Rot[6];
    q[28] = Rot[1];
    q[31] = Rot[4];
    q[34] = Rot[7];
    q[29] = Rot[2];
    q[32] = Rot[5];
    q[35] = Rot[8];
}



/** \brief Given a posture of a frame (specified using a 4x4 homogeneous matrix Tc) and an offset
    (x, y, z, X(alpha), Y(beta), Z(gamma)) returns a posture Td that includes the offset.

    \param[in] Tc 4x4 homogeneous matrix
    \param[out] Td 4x4 homogeneous matrix

    \param[in] x x-position offset
    \param[in] y y-position offset
    \param[in] z z-position offset
    \param[in] alpha x-rotation offset
    \param[in] beta y-rotation offset
    \param[in] gamma z-rotation offset

    \return void

*/
void PostureOffset(double *Tc, double *Td,
                   double x, double y, double z, double alpha, double beta, double gamma)
{
    double tmp[4*4];
    Euler2T(x, y, z, alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> TcE(Tc,4,4);
    Eigen::Map<Eigen::MatrixXd> T(tmp,4,4);

    T = TcE*T;

    for (int i=0; i<4*4; i++)
        Td[i] = T(i);
}



/** \brief Given a rotation matrix and an offset specified as X(alpha)->Y(beta)->Z(gamma) (current
    axis) Euler angles, returns a rotation matrix Rd that includes the offset.

    \param[in] Rc 4x4 rotation matrix
    \param[out] Rd 4x4 rotation matrix

    \param[in] alpha x-rotation offset
    \param[in] beta y-rotation offset
    \param[in] gamma z-rotation offset

    \return void

*/
void RotationOffset(double *Rc, double *Rd, double alpha, double beta, double gamma)
{

    double tmp[3*3];

    Euler2Rot(alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> RcE(Rc,3,3);
    Eigen::Map<Eigen::MatrixXd> R(tmp,3,3);

    R = RcE*R;

    for (int i=0; i<3*3; i++)
        Rd[i] = R(i);
}


/** \brief Sets the initial configuration of nao (lets call it the standard initial configuration)

    \param[out] q Joint angles

    \note Only q[0]...q[23] are set. The posture of the base is not set.

    \return void

*/
void InitJointAngles(double *q)
{
    // LEFT LEG
    q[L_HIP_YAW_PITCH] =  0.0;
    q[L_HIP_ROLL]      =  0.0;
    q[L_HIP_PITCH]     = -0.436332;
    q[L_KNEE_PITCH]    =  0.698132;
    q[L_ANKLE_PITCH]   = -0.349066;
    q[L_ANKLE_ROLL]    =  0.0;

    // RIGHT LEG
    q[R_HIP_YAW_PITCH] =  0.0;
    q[R_HIP_ROLL]      =  0.0;
    q[R_HIP_PITCH]     = -0.436332;
    q[R_KNEE_PITCH]    =  0.698132;
    q[R_ANKLE_PITCH]   = -0.349066;
    q[R_ANKLE_ROLL]    =  0.0;

    // LEFT ARM
    q[L_SHOULDER_PITCH] =  1.396263;
    q[L_SHOULDER_ROLL]  =  0.349066;
    q[L_ELBOW_YAW]      = -1.396263;
    q[L_ELBOW_ROLL]     = -1.047198;
    q[L_WRIST_YAW]      =  0.0;

    // RIGHT ARM
    q[R_SHOULDER_PITCH] =  1.396263;
    q[R_SHOULDER_ROLL]  = -0.349066;
    q[R_ELBOW_YAW]      =  1.396263;
    q[R_ELBOW_ROLL]     =  1.047198;
    q[R_WRIST_YAW]      =  0.0;

    // HEAD
    q[HEAD_PITCH] =  0.0;
    q[HEAD_YAW]   =  0.0;
}



/** \brief Print a [m by n] matrix on the screne

    \param[in] m Number of rows
    \param[in] n Number of columns
    \param[in] A Matrix of dimension [m by n]
    \param[in] description A short description of the array (it is printed above the array)

    \note This is the same function as void Matrix_print(unsigned int m, unsigned int n, double * A, const char * description)
    but I changed the name, because if I link to mbs_simulator, I get duplicate symbols

    \return void
*/
void MatrixPrint(unsigned int m, unsigned int n, double * A, const char * description)
{

    unsigned int  i, j;

    printf(" %s", description);
    for (i=0; i<m; i++)
    {
        printf("\n");
        for (j=0; j<n; j++)
            printf("% f ", A[ j*m + i ]);
    }
    printf("\n");

}



/** \brief Extracts the rotation matrix from a 4x4 homogeneous matrix

    \param[in] T 4x4 homogeneous matrix.
    \param[out] Rot 3x3 rotation matrix

    \return void
*/
void T2Rot(double * T, double *Rot)
{
    Rot[0] = T[0];
    Rot[3] = T[4];
    Rot[6] = T[8];
    Rot[1] = T[1];
    Rot[4] = T[5];
    Rot[7] = T[9];
    Rot[2] = T[2];
    Rot[5] = T[6];
    Rot[8] = T[10];

}
