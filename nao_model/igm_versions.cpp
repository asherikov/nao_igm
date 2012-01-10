#include <math.h> // sqrt

#include <Eigen/Core>       // Cholesky decomposition + solving
#include <Eigen/Cholesky>   // of system of linear equations.

#include "nao_igm.h"
#include "maple_functions.h"


#ifdef ENABLE_ALL_IGM_VERSIONS
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

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] TorsoT A 4x4 homogeneous matrix specifying the desired posture of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).


    \note It is assumed that the leading matrix of the constraints (A) is nonsingular.

    \par

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm_1(double *LegT, double *TorsoT)
{

    const int N = LOWER_JOINTS_NUM;

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
        for (int i=0; i < LOWER_JOINTS_NUM; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}



/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] CoM Desired position of the Center of Mass in the world frame.

    \param[in] RotTorso A 3x3 rotation matrix specifying the desired orientation of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    \par

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.


    \note For more details see #igm_1

    \note J2(1:3,:) is the Jacobian of the Torso position (not the CoM position) w.r.t. joints 0..11.
          Even though this is incorrect, the results seem to be reasonable
          (the real Jacobian of the CoM is used in #igm_3(...)).

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm_2(double *LegT, double *CoM, double *RotTorso)
{

    const int N = LOWER_JOINTS_NUM;

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
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}
#endif


/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] CoM Desired position of the Center of Mass in the world frame.

    \param[in] RotTorso A 3x3 rotation matrix specifying the desired orientation of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \par

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note For more details see #igm_1

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm_3(double *LegT, double *CoM, double *RotTorso)
{
    /**
    @todo Some parameters are defined in the class, probably we do not need to specify them explicitly.
    */

    const int N = LOWER_JOINTS_NUM;

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
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}



/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \param[in] LegT A 4x4 homogeneous matrix specifying the desired posture of the leg not in support.

    \param[in] CoM Desired position of the Center of Mass in the world frame.

    \param[in] RotTorso A 3x3 rotation matrix specifying the desired orientation of the torso (note
    that the orientation of the Torso frame around the z axis of the world frame is ignored).

    \par

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note For more details see #igm_1

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm_3(double *LegT, double *CoM, double *RotTorso, const double *q0, const double mu)
{
    /**
    @todo Some parameters are defined in the class, probably we do not need to specify them explicitly.
    */

    const int N = LOWER_JOINTS_NUM;

    double out[N*N+N];
    Eigen::VectorXd dq(N);
    Eigen::VectorXd z(N);
    Eigen::LLT<Eigen::MatrixXd> llt;

    double tol = 0.0005;
    int max_iter = 20;
    int iter = 0;
    double norm_dq = 1.0;

    while (norm_dq > tol && iter != -1)
    {
        for (int i=0; i<N; i++)
            z(i) = mu*(q[i] - q0[i]);

        // Form data
        if (support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_3(q, LegT, CoM, RotTorso, out);
        else
            from_RLeg_3(q, LegT, CoM, RotTorso, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err-A*z, &dq);
        dq = -z-A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
            q[i] += dq[i];

        // Check termination condition
        norm_dq = 0;
        for (int i=0; i<11; i++)
            norm_dq += dq[i]*dq[i];
        norm_dq = sqrt(norm_dq);

        iter++;
        if (iter > max_iter)
        {
            iter = -1;
        }
    }

    return(iter);
}



#ifdef ENABLE_ALL_IGM_VERSIONS
/**
 * @todo Add description for this function.
 */
int nao_igm::igm_4(double *LegT, double *CoM, double *RotTorso, const double *q0, const double mu)
{

    const int M = LOWER_JOINTS_NUM;
    const int N = JOINTS_NUM;

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
            iter = -1;
        }
    }

    return(iter);
}
#endif
