/** 
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:34:06 MSK
 */



/****************************************
 * INCLUDES 
 ****************************************/
#include <stdio.h>
#include <Eigen/Core>
#include <Eigen/Cholesky>

#include "posture_orientation.h"


/****************************************
 * FUNCTIONS 
 ****************************************/

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


/** @brief Given a posture of a frame (specified using a 4x4 homogeneous matrix Tc) and an offset
    (x, y, z, X(alpha), Y(beta), Z(gamma)) computed a posture that includes the offset.

    @param[in] Tc 4x4 homogeneous matrix

    @param[in] x x-position offset
    @param[in] y y-position offset
    @param[in] z z-position offset
    @param[in] roll x-rotation offset
    @param[in] pitch y-rotation offset
    @param[in] yaw z-rotation offset

    @param[in] Td 4x4 homogeneous matrix
*/
void postureOffset(
        const posture& Tc,
        const double x,
        const double y,
        const double z,
        const double roll,
        const double pitch,
        const double yaw,
        posture & Td)
{
    posture tmp;
    tmp.init (x, y, z, roll, pitch, yaw);

    Eigen::Map<Eigen::MatrixXd> TcE(Tc.data,4,4);
    Eigen::Map<Eigen::MatrixXd> T(tmp.data,4,4);

    T = TcE*T;

    for (int i=0; i<POSTURE_MATRIX_SIZE; i++)
    {
        Td.data[i] = T(i);
    }
}



/** @brief Given a rotation matrix and an offset specified as X(alpha)->Y(beta)->Z(gamma) (current
    axis) Euler angles, returns a rotation matrix Rd that includes the offset.

    @param[in] Rc 4x4 rotation matrix

    @param[in] alpha x-rotation offset
    @param[in] beta y-rotation offset
    @param[in] gamma z-rotation offset

    @param[out] Rd 4x4 rotation matrix
*/
void rotationOffset(
        const double *Rc,
        const double alpha,
        const double beta,
        const double gamma,
        double *Rd)
{
    double tmp[ORIENTATION_MATRIX_SIZE];

    rpy2R(alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> RcE(Rc,3,3);
    Eigen::Map<Eigen::MatrixXd> R(tmp,3,3);

    R = RcE*R;

    for (int i=0; i<3*3; i++)
        Rd[i] = R(i);
}
