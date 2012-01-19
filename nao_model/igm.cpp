#include <math.h> // sqrt

#include "nao_igm.h"
#include "maple_functions.h"
#include "joint_bounds.h"

#include <Eigen/Core>   
#include <Eigen/Cholesky>



/**
 * @brief Check that all joint angles lie within bounds.
 *
 * @return -1 if all values are corrent, id of the first joint violating the
 * bounds.
 *
 * @attention No collision checks!
 */
int nao_igm::checkJointBounds()
{
    return(joint_bounds.check(state.q));
}



/** @brief Given a posture of a frame (specified using a 4x4 homogeneous matrix Tc) and an offset
    (x, y, z, X(alpha), Y(beta), Z(gamma)) returns a posture Td that includes the offset.

    @param[in] Tc 4x4 homogeneous matrix
    @param[out] Td 4x4 homogeneous matrix

    @param[in] x x-position offset
    @param[in] y y-position offset
    @param[in] z z-position offset
    @param[in] alpha x-rotation offset
    @param[in] beta y-rotation offset
    @param[in] gamma z-rotation offset
*/
void nao_igm::PostureOffset(
        const double *Tc, 
        double *Td,
        const double x, 
        const double y, 
        const double z, 
        const double alpha, 
        const double beta, 
        const double gamma)
{
    double tmp[4*4];
    Euler2T(x, y, z, alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> TcE(Tc,4,4);
    Eigen::Map<Eigen::MatrixXd> T(tmp,4,4);

    T = TcE*T;

    for (int i=0; i<4*4; i++)
        Td[i] = T(i);
}



/**
 * @brief Set position in a 4x4 homogeneous matrix.
 *
 * @param[in,out] Tc 4x4 homogeneous matrix.
 * @param[in] position 3x1 vector of coordinates.
 * @param[in] roll angle
 * @param[in] pitch angle
 * @param[in] yaw angle
 */
void nao_igm::initPosture (
        double *Tc, 
        const double *position,
        const double roll,
        const double pitch,
        const double yaw)
{
    Tc[12] = position[0];
    Tc[13] = position[1];
    Tc[14] = position[2];

    // form the rotation matrix corresponding to a set of roll-pitch-yaw angles
    rpy2R_hom (roll, pitch, yaw, Tc);
}



/**
 * @brief Set coordinates of center of mass.
 *
 * @param[in] x coordinate
 * @param[in] y coordinate
 * @param[in] z coordinate
 */
void nao_igm::setCoM (const double x, const double y, const double z)
{
    CoM_position[0] = x;
    CoM_position[1] = y;
    CoM_position[2] = z;
}



/** @brief Given a rotation matrix and an offset specified as X(alpha)->Y(beta)->Z(gamma) (current
    axis) Euler angles, returns a rotation matrix Rd that includes the offset.

    @param[in] Rc 4x4 rotation matrix
    @param[out] Rd 4x4 rotation matrix

    @param[in] alpha x-rotation offset
    @param[in] beta y-rotation offset
    @param[in] gamma z-rotation offset
*/
void nao_igm::RotationOffset(
        const double *Rc, 
        double *Rd, 
        const double alpha, 
        const double beta, 
        const double gamma)
{
    double tmp[3*3];

    Euler2Rot(alpha, beta, gamma, tmp);

    Eigen::Map<Eigen::MatrixXd> RcE(Rc,3,3);
    Eigen::Map<Eigen::MatrixXd> R(tmp,3,3);

    R = RcE*R;

    for (int i=0; i<3*3; i++)
        Rd[i] = R(i);
}



/**
 * @brief Initialize model.
 *
 * @param[in] support_foot_ current support foot.
 * @param[in] x x-position
 * @param[in] y y-position
 * @param[in] z z-position
 * @param[in] sup_orientation orientation of the supprt foot.
 */
void nao_igm::init(
        const igmSupportFoot support_foot_, 
        const double x, 
        const double y, 
        const double z, 
        const double *sup_orientation)
{
    state.q[SUPPORT_FOOT_POS_START]       = x;
    state.q[SUPPORT_FOOT_POS_START + 1]   = y;
    state.q[SUPPORT_FOOT_POS_START + 2]   = z;


    for (int i = SUPPORT_FOOT_ORIENTATION_START; 
            i < SUPPORT_FOOT_ORIENTATION_START + SUPPORT_FOOT_ORIENTATION_NUM; 
            i++)
    {
        state.q[i] = sup_orientation[i - SUPPORT_FOOT_ORIENTATION_START];
    }


    state.support_foot = support_foot_;
    double torso_posture[POSTURE_MATRIX_SIZE];
    if (state.support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(state.q, swing_foot_posture);
        LLeg2Torso(state.q, torso_posture);
        LLeg2CoM(state.q, CoM_position);
    }
    else
    {
        RLeg2LLeg(state.q, swing_foot_posture);
        RLeg2Torso(state.q, torso_posture);
        RLeg2CoM(state.q, CoM_position);
    }
    T2Rot(torso_posture, torso_orientation);
}



/** @brief Initialize model.

    @param[in] support_foot_ current support foot.
    @param[in] x x-position
    @param[in] y y-position
    @param[in] z z-position
    @param[in] alpha x-rotation
    @param[in] beta y-rotation
    @param[in] gamma z-rotation
*/
void nao_igm::init(
        const igmSupportFoot support_foot_, 
        const double x, 
        const double y, 
        const double z, 
        const double alpha, 
        const double beta, 
        const double gamma)
{
    double Rot[ORIENTATION_MATRIX_SIZE];
    Euler2Rot(alpha, beta, gamma, Rot);
    init(support_foot_, x, y, z, Rot);
}


/**
 * @brief Switch support foot.
 */
void nao_igm::switchSupportFoot()
{
    if (state.support_foot == IGM_SUPPORT_LEFT)
    {
        state.support_foot = IGM_SUPPORT_RIGHT;
    }
    else
    {
        if (state.support_foot == IGM_SUPPORT_RIGHT)
        {
            state.support_foot = IGM_SUPPORT_LEFT;
        }
    }

    for (int i = SUPPORT_FOOT_POS_START; 
            i < SUPPORT_FOOT_POS_START + SUPPORT_FOOT_POS_NUM; 
            i++)
    {
        state.q[i] = swing_foot_posture[12 + i - SUPPORT_FOOT_POS_START];
    }

    double sup_orientation[ORIENTATION_MATRIX_SIZE];
    T2Rot (swing_foot_posture, sup_orientation);
    for (int i = SUPPORT_FOOT_ORIENTATION_START; 
            i < SUPPORT_FOOT_ORIENTATION_START + SUPPORT_FOOT_ORIENTATION_NUM; 
            i++)
    {
        state.q[i] = sup_orientation[i - SUPPORT_FOOT_ORIENTATION_START];
    }


    double torso_posture[POSTURE_MATRIX_SIZE];
    if (state.support_foot == IGM_SUPPORT_LEFT)
    {
        LLeg2RLeg(state.q, swing_foot_posture);
        LLeg2Torso(state.q, torso_posture);
        LLeg2CoM(state.q, CoM_position);
    }
    else
    {
        RLeg2LLeg(state.q, swing_foot_posture);
        RLeg2Torso(state.q, torso_posture);
        RLeg2CoM(state.q, CoM_position);
    }
    T2Rot(torso_posture, torso_orientation);
}



/** @brief Extracts the rotation matrix from a 4x4 homogeneous matrix

    @param[in] T 4x4 homogeneous matrix.
    @param[out] Rot 3x3 rotation matrix
*/
void nao_igm::T2Rot(const double * T, double *Rot)
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
void nao_igm::rpy2R(const double roll, const double pitch, const double yaw, double *R)
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
void nao_igm::rpy2R_hom(const double roll, const double pitch, const double yaw, double *R)
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
