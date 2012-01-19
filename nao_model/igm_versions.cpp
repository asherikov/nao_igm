#include <Eigen/Core>       // Cholesky decomposition + solving
#include <Eigen/Cholesky>   // of system of linear equations.

#include "nao_igm.h"
#include "maple_functions.h"


/** \brief Solves the Inverse Geometric Problem (IGM). Constraints for (x, y, z, X(alpha), Y(beta),
    Z(gamma)) of the foot not in support as well as (x, y, z) position of the CoM, and X(alpha),
    Y(beta) rotation of the torso can be imposed.

    \return int iter - the number of iterations performed until convergence, or -1 if the algorithm
    did not converge within max_iter number of iterations.

    \note It is assumed that the leading matrix of the constraints is nonsingular.

    @note On input, the entries of q are the joint angles from where to start the
    search for a solution (i.e., the initial guess). On output q contains a solution of the
    inverse kinematics problem (if iter != -1). Only q[0]...q[11] are altered.
*/
int nao_igm::igm()
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
        if (state.support_foot == IGM_SUPPORT_LEFT)
            from_LLeg_3(state.q, swing_foot_posture, CoM_position, torso_orientation, out);
        else
            from_RLeg_3(state.q, swing_foot_posture, CoM_position, torso_orientation, out);

        Eigen::Map<Eigen::MatrixXd> A(out,N,N);
        Eigen::Map<Eigen::VectorXd> err(out+N*N,N);

        // Solve KKT system
        llt.compute(A*A.transpose());
        llt.solve(-err, &dq);
        dq = -A.transpose()*dq;

        // Update angles (of legs)
        for (int i=0; i<LOWER_JOINTS_NUM; i++)
            state.q[i] += dq[i];

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
