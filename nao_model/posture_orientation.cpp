/**
 * @file
 * @author Alexander Sherikov
 */

#include "posture_orientation.h"
#include "maple_functions.h"


#include <Eigen/Core>   
#include <Eigen/Cholesky>


/** @brief Given a posture of a frame (specified using a 4x4 homogeneous matrix Tc) and an offset
    (x, y, z, X(alpha), Y(beta), Z(gamma)) returns a posture Td that includes the offset.

    @param[in] Tc 4x4 homogeneous matrix

    @param[in] x x-position offset
    @param[in] y y-position offset
    @param[in] z z-position offset
    @param[in] roll x-rotation offset
    @param[in] pitch y-rotation offset
    @param[in] yaw z-rotation offset

    @param[out] Td 4x4 homogeneous matrix
*/
void PostureOffset(
        const double *Tc, 
        const double x, 
        const double y, 
        const double z, 
        const double roll, 
        const double pitch, 
        const double yaw,
        double *Td)
{
    double tmp[POSTURE_MATRIX_SIZE];
    initPosture (x, y, z, roll, pitch, yaw, tmp);

    Eigen::Map<Eigen::MatrixXd> TcE(Tc,4,4);
    Eigen::Map<Eigen::MatrixXd> T(tmp,4,4);

    T = TcE*T;

    for (int i=0; i<POSTURE_MATRIX_SIZE; i++)
    {
        Td[i] = T(i);
    }
}



/**
 * @brief Initialize a 4x4 homogeneous matrix.
 *
 * @param[in] position 3x1 vector of coordinates.
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 * @param[in,out] Tc 4x4 homogeneous matrix.
 */
void initPosture (
        const double *position,
        const double roll,
        const double pitch,
        const double yaw,
        double *Tc) 
{
    initPosture (position[0], position[1], position[2], roll, pitch, yaw, Tc);
}



/**
 * @brief Initialize a 4x4 homogeneous matrix.
 *
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] z Z coordinate
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 * @param[in,out] Tc 4x4 homogeneous matrix.
 */
void initPosture (
        const double x,
        const double y,
        const double z,
        const double roll,
        const double pitch,
        const double yaw,
        double *Tc) 
{
    Tc[12] = x;
    Tc[13] = y;
    Tc[14] = z;
    Tc[15] = 1.0;
    Tc[3] = Tc[7] = Tc[11] = 0.0;

    // form the rotation matrix corresponding to a set of roll-pitch-yaw angles
    rpy2R_hom (roll, pitch, yaw, Tc);
}



/** @brief Given a rotation matrix and an offset specified as X(alpha)->Y(beta)->Z(gamma) (current
    axis) Euler angles, returns a rotation matrix Rd that includes the offset.

    @param[in] Rc 4x4 rotation matrix
    @param[out] Rd 4x4 rotation matrix

    @param[in] alpha x-rotation offset
    @param[in] beta y-rotation offset
    @param[in] gamma z-rotation offset
*/
void RotationOffset(
        const double *Rc, 
        double *Rd, 
        const double alpha, 
        const double beta, 
        const double gamma)
{
    double tmp[ORIENTATION_MATRIX_SIZE];

    rpy2R(alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> RcE(Rc,3,3);
    Eigen::Map<Eigen::MatrixXd> R(tmp,3,3);

    R = RcE*R;

    for (int i=0; i<3*3; i++)
        Rd[i] = R(i);
}



/** @brief Extracts the rotation matrix from a 4x4 homogeneous matrix

    @param[in] T 4x4 homogeneous matrix.
    @param[out] Rot 3x3 rotation matrix
*/
void T2Rot(const double * T, double *Rot)
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


/** @brief Forms the rotation matrix corresponding to a set of roll-pitch-yaw angles

    @param[in] roll roll angle [radian]
    @param[in] pitch pitch angle [radian]
    @param[in] yaw yaw angle [radian]
    @param[out] R Rotation matrix corresponding to the roll-pitch-yaw angles
    
    \note The rotation defined using roll, pitch and yaw angles is assumed to be formed by first
    applying a rotation around the x axis (roll), then a rotation around the new y axis (pitch) and
    finally a rotation around the new z axis (yaw).

    \note The matrix is stored column-wise (Fortran formatting)
 */
void rpy2R(const double roll, const double pitch, const double yaw, double *R)
{
    double sr = sin(roll);
    double cr = cos(roll);

    double sp = sin(pitch);
    double cp = cos(pitch);

    double sy = sin(yaw);
    double cy = cos(yaw);

    R[0] =  cp*cy;            R[3] = -cp*sy;            R[6] =  sp;
    R[1] =  sr*sp*cy + cr*sy; R[4] = -sr*sp*sy + cr*cy; R[7] = -sr*cp;
    R[2] = -cr*sp*cy + sr*sy; R[5] =  cr*sp*sy + sr*cy; R[8] =  cr*cp;
}


/** @brief Forms the rotation matrix corresponding to a set of roll-pitch-yaw angles

    @param[in] roll roll angle [radian]
    @param[in] pitch pitch angle [radian]
    @param[in] yaw yaw angle [radian]
    @param[out] R homogeneous matrix
    
    \note The rotation defined using roll, pitch and yaw angles is assumed to be formed by first
    applying a rotation around the x axis (roll), then a rotation around the new y axis (pitch) and
    finally a rotation around the new z axis (yaw).

    \note The matrix is stored column-wise (Fortran formatting)
 */
void rpy2R_hom(const double roll, const double pitch, const double yaw, double *R)
{
    double sr = sin(roll);
    double cr = cos(roll);

    double sp = sin(pitch);
    double cp = cos(pitch);

    double sy = sin(yaw);
    double cy = cos(yaw);

    R[0] =  cp*cy;            R[4] = -cp*sy;            R[8] =  sp;
    R[1] =  sr*sp*cy + cr*sy; R[5] = -sr*sp*sy + cr*cy; R[9] = -sr*cp;
    R[2] = -cr*sp*cy + sr*sy; R[6] =  cr*sp*sy + sr*cy; R[10] =  cr*cp;
}
