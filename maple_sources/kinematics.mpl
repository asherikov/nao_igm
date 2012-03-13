#--------------------------------------------------------------------------------
# Loading libraries
#--------------------------------------------------------------------------------
with(LinearAlgebra):
with(MTM):
with(codegen, C, makeproc):
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Utility functions
#--------------------------------------------------------------------------------

#
# The variable RotationsRepresentation should be defined before this file is used.
# The orientation of the base is specified using:
#  -- Euler angles,    if RotationsRepresentation = 0,
#  -- unit quaternion, if RotationsRepresentation = 1,
#  -- rotation matrix, if RotationsRepresentation = 2.
#
# Any of the end-effectors can be chosen to be the base of the chain.
# In the case of NAO, LLeg or RLeg will be used as base.
#

if RotationsRepresentation = 0 then
    printf("Using X->Y->Z (current axis) Euler angles\n");
elif  RotationsRepresentation = 1 then
    printf("Using unit quaternion\n");
elif  RotationsRepresentation = 2 then
    printf("Using rotation matrix\n");
else
    printf("The variable RotationsRepresentation should be equal to 0 or 1\n");
end if;


# Outputs the contents of a list to a file
# It is assumed that all elements of the list are matrices with the same dimensions.
#
# Note: In order to store a single matrix A use for example List2File([A],"A.m",A)
#
List2File := proc(L,filename,varname)
local i_max, j_max, k_max, i, j, k, fd;

    i_max := ArrayTools:-Size(L[1],1);
    j_max := ArrayTools:-Size(L[1],2);
    k_max := max(ArrayTools:-Size(L,2),ArrayTools:-Size(L,2));

    fd := fopen(filename,WRITE):
    for k to k_max do
        fprintf(fd,"%s{%d} = [\n",varname,k):
        for i to i_max do
            for j to j_max do
                fprintf(fd," %02.10f ", L[k][i,j]):
            end do;
            fprintf(fd,"\n"):
        end do;
        fprintf(fd,"]; \n\n"):
    end do;
    fclose(fd):

end proc:

# Returns joints between the torso frame and frame with index FrameIndex
# (there is a frame fixed in each joint).
JointTree := proc(FrameIndex)
local i, joints, parent;

    joints := [];
    if FrameIndex < 1 or FrameIndex > n then
        pritnf("ERROR: FrameIndex is out of range [1..24]");
        return joints;
    end if;

    joints := [op(joints),FrameIndex];
    parent := Joints[FrameIndex]:-parent;
    if parent <> 0 then
        for i from 1 to n while parent <> 0 do
            joints := [op(joints),parent];
            parent := Joints[parent]:-parent;
        end do;
    end if;

    return joints;
end proc:

# Returs the inverse of a homogeneous matrix T
InverseOfHomogMatrix := proc(T)
local R, r, iT;

    R := T[1..3,1..3];
    r := T[1..3,4];

    iT := Matrix([[1, 0, 0, 0],
                  [0, 1, 0, 0],
                  [0, 0, 1, 0],
                  [0, 0, 0, 1]]);

    iT[1..3,4] := -Transpose(R).r;
    iT[1..3,1..3] := Transpose(R);

    return iT;
end proc:

#--------------------------------------------------------------------------------
# Defines a homogeneous matrix
# s - is either Joints or EE
# k - specifies the k-th entry of the list "s"
#--------------------------------------------------------------------------------
HomogMatrixLocal := proc(s,k)
local T, X, Y, Z;

    # X->Y->Z Euler angles (current axis)
    X := s[k]:-e[1];
    Y := s[k]:-e[2];
    Z := s[k]:-e[3];

    T := Matrix([[                      cos(Y)*cos(Z),                      -cos(Y)*sin(Z),         sin(Y), s[k]:-r[1]],
                 [ sin(X)*sin(Y)*cos(Z)+cos(X)*sin(Z), -sin(X)*sin(Y)*sin(Z)+cos(X)*cos(Z), -sin(X)*cos(Y), s[k]:-r[2]],
                 [-cos(X)*sin(Y)*cos(Z)+sin(X)*sin(Z),  cos(X)*sin(Y)*sin(Z)+sin(X)*cos(Z),  cos(X)*cos(Y), s[k]:-r[3]],
                 [                                  0,                                   0,              0,         1]]);

    return T;
end proc:
#--------------------------------------------------------------------------------

# Gnerates a homogeneous matrix given
# position (x,y,z)
# rotation (X,Y,Z) specified using # X->Y->Z Euler angles (current axis)
#
Euler2HomogMatrix := proc(x,y,z,X,Y,Z)
local T;

    T := Matrix([[                      cos(Y)*cos(Z),                      -cos(Y)*sin(Z),         sin(Y), x],
                 [ sin(X)*sin(Y)*cos(Z)+cos(X)*sin(Z), -sin(X)*sin(Y)*sin(Z)+cos(X)*cos(Z), -sin(X)*cos(Y), y],
                 [-cos(X)*sin(Y)*cos(Z)+sin(X)*sin(Z),  cos(X)*sin(Y)*sin(Z)+sin(X)*cos(Z),  cos(X)*cos(Y), z],
                 [                                  0,                                   0,              0, 1]]);

    return T;
end proc:

#--------------------------------------------------------------------------------

# Gnerates a homogeneous matrix given
# position (x,y,z)
# a unit quaternion ([S,X,Y,Z]) where S>0 is the scalar part
#
Quat2HomogMatrix := proc(x,y,z,S,X,Y,Z)
local T;

    T := Matrix([[ 1-2*Z^2-2*Y^2,   2*X*Y-2*S*Z,   2*X*Z+2*S*Y, x],
                 [   2*X*Y+2*S*Z, 1-2*Z^2-2*X^2,   2*Y*Z-2*S*X, y],
                 [   2*X*Z-2*S*Y,   2*Y*Z+2*S*X, 1-2*Y^2-2*X^2, z],
                 [             0,             0,             0, 1]]);

    return T;
end proc:


#--------------------------------------------------------------------------------

# Gnerates a homogeneous matrix given
# position (x,y,z)
# a rotation matrix (defined as a [9x1] vector, column-wise)
#
Rot2HomogMatrix := proc(x,y,z,R)
local T;

    T := Matrix([[R[1], R[4], R[7], x],
                 [R[2], R[5], R[8], y],
                 [R[3], R[6], R[9], z],
                 [    0,   0,    0, 1]]);

    return T;
end proc:


#--------------------------------------------------------------------------------

# Computes the homogeneous matrix (w.r.t. the world frame) for the j-th end-effector
EEWorld := proc(JointFrames,j)
local T, parent;

    parent := EE[j]:-parent;

    if parent <> 0 then
        T := JointFrames[parent].HomogMatrixLocal(EE,j);
    else
        T := TorsoFrame.HomogMatrixLocal(EE,j);
    end if;

    return T;
end proc:

# Computes the position (w.r.t. the world frame) of the j-th CoM
CoMWorld := proc(JointFrames,j)
local Cw;

    if j <> 0 then
        Cw := JointFrames[j].ArrayTools:-Concatenate(1,Links[j]:-r,1);
    else
        Cw := TorsoFrame.ArrayTools:-Concatenate(1,TorsoLink:-r,1);
    end if;

    return Cw[1..3];
end proc:

# Computes the position of the CoM of the whole system (w.r.t. the world frame)
SystemCoM := proc(JointFrames)
local CoM, mass, i;

    mass := TorsoLink:-m;
    CoM := TorsoLink:-m.CoMWorld(JointFrames,0);
    for i from 1 to n do
        CoM := CoM + Links[i]:-m.CoMWorld(JointFrames,i);
        mass := mass + Links[i]:-m;
    end do;
    CoM := CoM/mass;

    return CoM;
end proc:

#--------------------------------------------------------------------------------

# Computes the posture of each frame starting from the base (with indes BaseEEIndex)
# given the joint angles q
FramesWorld := proc(BaseEEIndex)
local F, Tq, Tp, p, i, j, j1, ListOfJoints, m, inclusion_test, BaseFrameIndex;
global TorsoFrame; # the torso frame is modified here

    # Create a list with n elements (each of which will be overwritten with a homogeneous matrix)
    F:=[];
    for i from 1 to n do
        F := [op(F),[]];
    end do;

    BaseFrameIndex := EE[BaseEEIndex]:-parent;

    # Find the joints between the base frame (with index BaseFrameIndex) and the Torso frame
    ListOfJoints := JointTree(BaseFrameIndex);

    m := ArrayTools:-Size(ListOfJoints ,2);
    Tq := Matrix([[1, 0, 0, 0],
                  [0, 1, 0, 0],
                  [0, 0, 1, 0],
                  [0, 0, 0, 1]]);

    if m = 0 then
        if RotationsRepresentation = 0 then
            TorsoFrame := Euler2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30]);
        elif  RotationsRepresentation = 1 then
            TorsoFrame := Quat2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30],q[31]);
        elif  RotationsRepresentation = 2 then
            TorsoFrame := Rot2HomogMatrix(q[25],q[26],q[27],[seq(q[i], i=28..36)]);
        else
            printf("The variable RotationsRepresentation should be equal to 0 or 1");
        end if;
    else
        if RotationsRepresentation = 0 then
            F[BaseFrameIndex] := Euler2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30]).InverseOfHomogMatrix(HomogMatrixLocal(EE,BaseEEIndex));
        elif  RotationsRepresentation = 1 then
            F[BaseFrameIndex] := Quat2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30],q[31]).InverseOfHomogMatrix(HomogMatrixLocal(EE,BaseEEIndex));
        elif  RotationsRepresentation = 2 then
            F[BaseFrameIndex] := Rot2HomogMatrix(q[25],q[26],q[27],[seq(q[i], i=28..36)]).InverseOfHomogMatrix(HomogMatrixLocal(EE,BaseEEIndex));
        else
            printf("The variable RotationsRepresentation should be equal to 0 or 1");
        end if;
    end if;

    for i from 1 to m do

        j := ListOfJoints[i];

        Tq[1,1] :=  cos(q[j]);
        Tq[1,2] := -sin(q[j]);
        Tq[2,1] :=  sin(q[j]);
        Tq[2,2] :=  cos(q[j]);

        if i = m then # Update TorsoFrame
            TorsoFrame := F[j].Transpose(Tq).InverseOfHomogMatrix(HomogMatrixLocal(Joints,j));
        else
            j1 := ListOfJoints[i+1];
            F[j1] := F[j].Transpose(Tq).InverseOfHomogMatrix(HomogMatrixLocal(Joints,j));
        end if;

    end do;

    # Compute the homogeneous matrices for the remaining frames.
    for i from 1 to n do

        # update only unexplored frames so far
        inclusion_test := convert(i,set) subset convert(ListOfJoints,set);

        if not inclusion_test then

            Tq[1,1] :=  cos(q[i]);
            Tq[1,2] := -sin(q[i]);
            Tq[2,1] :=  sin(q[i]);
            Tq[2,2] :=  cos(q[i]);

            p := Joints[i]:-parent;
            if p <> 0 then
                Tp := F[p];
            else
                Tp := TorsoFrame;
            end if;

            F[i] := Tp.HomogMatrixLocal(Joints,i).Tq;

        end if;
    end do;

    return F;
end proc:

# Forms a jacobian matrix of an end-effector with index EEIndex w.r.t the joints between it and the base
# (with index BaseEEIndex)
FormJacobian := proc(EEIndex, BaseEEIndex)
local J, Base2Torso, EE2Torso, joints, joints1, joints2, m, i, k, rJ, r, inclusion_test, JointFrames, EF, BaseFrameIndex;

    JointFrames := FramesWorld(BaseEEIndex);
    EF := EEWorld(JointFrames,EEIndex);

    BaseFrameIndex := EE[BaseEEIndex]:-parent;

    Base2Torso := JointTree(BaseFrameIndex);
    EE2Torso := JointTree(EE[EEIndex]:-parent);

    joints1 := convert(convert(EE2Torso,set) union convert(Base2Torso,set),list);
    joints2 := convert(convert(EE2Torso,set) intersect convert(Base2Torso,set),list);
    joints := convert(convert(joints1,set) minus convert(joints2,set),list);

    #print(Base2Torso);
    #print(EE2Torso);
    #print(joints);

    m := ArrayTools:-Size(joints,2);
    J := Matrix(6,m);

    for i from 1 to m do

        inclusion_test := convert(joints[i],set) subset convert(Base2Torso,set);

        if not inclusion_test then
            k := JointFrames[joints[i]][1..3,3]:
        else
            k := -JointFrames[joints[i]][1..3,3]:
        end if;

        rJ := JointFrames[joints[i]][1..3,4]:
        r := EF[1..3,4] - rJ;

        J[1..6,i] := <CrossProduct(k,r),k>;
    end do;

    return J;
end proc:

# The difference between this function and FormJacobian is that here
# a Jacobian matrix with size 6x24 is constructed, i.e., the zero coulms are not eliminated.
#
FormJacobianFull := proc(EEIndex, BaseEEIndex)
local J, Base2Torso, EE2Torso, joints, joints1, joints2, m, i, k, rJ, r, inclusion_test, JointFrames, EF, BaseFrameIndex;

    JointFrames := FramesWorld(BaseEEIndex);
    EF := EEWorld(JointFrames,EEIndex);

    BaseFrameIndex := EE[BaseEEIndex]:-parent;

    Base2Torso := JointTree(BaseFrameIndex);
    EE2Torso := JointTree(EE[EEIndex]:-parent);

    joints1 := convert(convert(EE2Torso,set) union convert(Base2Torso,set),list);
    joints2 := convert(convert(EE2Torso,set) intersect convert(Base2Torso,set),list);
    joints := convert(convert(joints1,set) minus convert(joints2,set),list);

#    print(Base2Torso);
#    print(EE2Torso);
#    print(joints);
#    print("-------------------------------------------------");

    m := ArrayTools:-Size(joints,2);
    J := Matrix(6,n,0);

    for i from 1 to m do

        inclusion_test := convert(joints[i],set) subset convert(Base2Torso,set);

        if not inclusion_test then
            k := JointFrames[joints[i]][1..3,3]:
#            print(i);
#            print("OK");
        else
            k := -JointFrames[joints[i]][1..3,3]:
#            print(i);
#            print("Negeted");
        end if;

        rJ := JointFrames[joints[i]][1..3,4]:
        r := EF[1..3,4] - rJ;

        J[1..6,joints[i]] := <CrossProduct(k,r),k>;
    end do;

    return J;
end proc:


# Forms an error vector between the current homogeneous matrix Tc and desired one Td
#
# Tc and Td are passed as vectors with indexes
# [1 5  9 13;
#  2 6 10 14;
#  3 7 11 15
#  4 8 12 16];
#
# In Matlab
# syms Tc1 Tc2 Tc3 Tc4 Tc5 Tc6 Tc7 Tc8 Tc9 Tc10 Tc11 Tc12 Tc13 Tc14 Tc15 Tc16
# syms Td1 Td2 Td3 Td4 Td5 Td6 Td7 Td8 Td9 Td10 Td11 Td12 Td13 Td14 Td15 Td16
#
# Tc = [Tc1 Tc2 Tc3 Tc4 Tc5 Tc6 Tc7 Tc8 Tc9 Tc10 Tc11 Tc12 Tc13 Tc14 Tc15 Tc16];
# Td = [Td1 Td2 Td3 Td4 Td5 Td6 Td7 Td8 Td9 Td10 Td11 Td12 Td13 Td14 Td15 Td16];
#
# Tc = reshape(Tc,4,4);
# Td = reshape(Td,4,4);
#
# e = R_err(Tc(1:3,1:3),Td(1:3,1:3));
#
FormError := proc(Tc, Td)
local err;

    err := < Td[13] - Tc[13],
             Td[14] - Tc[14],
             Td[15] - Tc[15],
             0.5*(Td[3]*Tc[2] - Td[2]*Tc[3] + Td[7]*Tc[6] - Td[6]*Tc[7] + Td[11]*Tc[10] - Td[10]*Tc[11]),
             0.5*(Td[1]*Tc[3] - Td[3]*Tc[1] + Td[5]*Tc[7] - Td[7]*Tc[5] + Td[9]*Tc[11] - Td[11]*Tc[9]),
             0.5*(Td[2]*Tc[1] - Td[1]*Tc[2] + Td[6]*Tc[5] - Td[5]*Tc[6] + Td[10]*Tc[9] - Td[9]*Tc[10])>;

    return err;
end proc:

# Forms an error vector between the current rotation matrix Rc and desired one Rd
#
# Rc and Rd are passed as vectors with indexes
# [1 4 7;
#  2 5 8;
#  3 6 9]
#
# In Matlab
#
# syms Rc1 Rc2 Rc3 Rc4 Rc5 Rc6 Rc7 Rc8 Rc9
# syms Rd1 Rd2 Rd3 Rd4 Rd5 Rd6 Rd7 Rd8 Rd9
#
# Rc = [Rc1 Rc2 Rc3 Rc4 Rc5 Rc6 Rc7 Rc8 Rc9];
# Rd = [Rd1 Rd2 Rd3 Rd4 Rd5 Rd6 Rd7 Rd8 Rd9];
#
# Rc = reshape(Rc,3,3);
# Rd = reshape(Rd,3,3);
#
# e=R_err(Rc,Rd);
#
FormErrorRot := proc(Rc, Rd)
local err;

    err := < 0.5*(Rd[3]*Rc[2] - Rd[2]*Rc[3] + Rd[6]*Rc[5] - Rd[5]*Rc[6] + Rd[9]*Rc[8] - Rd[8]*Rc[9]),
             0.5*(Rd[1]*Rc[3] - Rd[3]*Rc[1] + Rd[4]*Rc[6] - Rd[6]*Rc[4] + Rd[7]*Rc[9] - Rd[9]*Rc[7]),
             0.5*(Rd[2]*Rc[1] - Rd[1]*Rc[2] + Rd[5]*Rc[4] - Rd[4]*Rc[5] + Rd[8]*Rc[7] - Rd[7]*Rc[8])>;

    return err;
end proc:

#EOF
