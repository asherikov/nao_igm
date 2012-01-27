/**
 * @file
 * @author Alexander Sherikov
 */

#include <cmath>  // sin, cos

#include "posture_orientation.h"
#include "maple_functions.h"



/**
 * @brief Initialize a 4x4 homogeneous matrix.
 *
 * @param[in] position 3x1 vector of coordinates.
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 */
void posture::init(
        const double *position,
        const double roll,
        const double pitch,
        const double yaw)
{
    this->init (position[0], position[1], position[2], roll, pitch, yaw);
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
 */
void posture::init(
        const double x,
        const double y,
        const double z,
        const double roll,
        const double pitch,
        const double yaw)
{
    data[12] = x;
    data[13] = y;
    data[14] = z;
    data[15] = 1.0;
    data[3] = data[7] = data[11] = 0.0;

    // form the rotation matrix corresponding to a set of roll-pitch-yaw angles
    double rot[ORIENTATION_MATRIX_SIZE];
    rpy2R (roll, pitch, yaw, rot);
    setOrientation (rot);
}



/** @brief Copy rotation matrix to homogeneous matrix
    @param[in] Rot  3x3 Rotation matrix
 */
void posture::setOrientation(const double *Rot)
{
    data[0]  = Rot[0];
    data[1]  = Rot[1];
    data[2]  = Rot[2];

    data[4]  = Rot[3];
    data[5]  = Rot[4];
    data[6]  = Rot[5];

    data[8]  = Rot[6];
    data[9]  = Rot[7];
    data[10] = Rot[8];
}



/** @brief Extracts the rotation matrix from a 4x4 homogeneous matrix
    @param[out] Rot 3x3 rotation matrix
*/
void posture::getOrientation(double *Rot)
{
    Rot[0] = data[0];
    Rot[3] = data[4];
    Rot[6] = data[8];
    Rot[1] = data[1];
    Rot[4] = data[5];
    Rot[7] = data[9];
    Rot[2] = data[2];
    Rot[5] = data[6];
    Rot[8] = data[10];
}



/** @brief Extracts the coordinates from a 4x4 homogeneous matrix
    @param[out] pos 3x1 position vector
*/
void posture::getPosition (double *pos)
{
    pos[0] = data[12];
    pos[1] = data[13];
    pos[2] = data[14];
}



/**
 * @brief Computes difference between positions in two homogeneous matrices.
 *
 * @param[in] posture_a 4x4 homogeneous matrix
 * @param[in,out] position_error 3x1 vector of differences
 */
void posture::getPositionDiff (
        const posture& posture_a, 
        double *position_error)
{
    position_error[0] = data[12] - posture_a.data[12];
    position_error[1] = data[13] - posture_a.data[13];
    position_error[2] = data[14] - posture_a.data[14];
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
