# NAO robot definition

n := 24: # 24 joints

if RotationsRepresentation = 0 then
    q := vector(n + 6): # 24 joints + 6 DoF for the base link

    # Defines the posture of the torso frame w.r.t. the world frame.
    TorsoFrame := Euler2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30]):

elif  RotationsRepresentation = 1 then

    q := vector(n + 3 + 4): # 24 joints + 3 DoF for base link translation + 4 parameters describing its orientation (unit quaternion)

    TorsoFrame := Quat2HomogMatrix(q[25],q[26],q[27],q[28],q[29],q[30],q[31]):

elif  RotationsRepresentation = 2 then

    q := vector(n + 3 + 9): # 24 joints + 3 DoF for base link translation + 9 parameters describing its orientation (rotation matrix)

    TorsoFrame := Rot2HomogMatrix(q[25],q[26],q[27],[seq(q[i], i=28..36)]):

else

    printf("The variable RotationsRepresentation should be equal to 0 or 1");

end if:


# Using X(alpha)->Y(beta)->Z(gamma) (current axis) Euler angles
#q[25] x
#q[26] y
#q[27] z
#q[28] alpha
#q[29] beta
#q[30] gamma

# Using {S,X,Y,Z} using quaternion
#q[25] x
#q[26] y
#q[27] z
#q[28] S
#q[29] X
#q[30] Y
#q[31] Z

# Using rotation matrix R
#q[25] x
#q[26] y
#q[27] z
#q[28] R(1,1)
#q[29] R(2,1)
#q[30] R(3,1)
#q[31] R(1,2)
#q[32] R(2,2)
#q[33] R(3,2)
#q[34] R(1,3)
#q[35] R(2,3)
#q[36] R(3,3)

#==========================================================================================================================

NeckOffsetZ      := 126.50/1000.0:
ShoulderOffsetY  :=  98.00/1000.0:
ShoulderOffsetZ  := 100.00/1000.0:
UpperArmLength   :=  90.00/1000.0:
LowerArmLength   :=  50.55/1000.0:
HipOffsetY       :=  50.00/1000.0:
HipOffsetZ       :=  85.00/1000.0:
ThighLength      := 100.00/1000.0:
TibiaLength      := 102.74/1000.0:
FootHeight       :=  45.11/1000.0:
HandOffsetX      :=  58.00/1000.0:
HandOffsetZ      :=  15.90/1000.0:

Joints := []:
Links := []:
EE := []:

#==========================================================================================================================

# Note: Frame 0 is fixed in the Torso.

# Left Leg
Joints := [op(Joints), struct('parent',  0, 'r', <              0,       HipOffsetY,      -HipOffsetZ>, 'e', < -3/4*Pi,     0,  0>)]: #1
Joints := [op(Joints), struct('parent',  1, 'r', <              0,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #2
Joints := [op(Joints), struct('parent',  2, 'r', <              0,                0,                0>, 'e', <    Pi/2, -Pi/4,  0>)]: #3
Joints := [op(Joints), struct('parent',  3, 'r', <   -ThighLength,                0,                0>, 'e', <       0,     0,  0>)]: #4
Joints := [op(Joints), struct('parent',  4, 'r', <   -TibiaLength,                0,                0>, 'e', <       0,     0,  0>)]: #5
Joints := [op(Joints), struct('parent',  5, 'r', <              0,                0,                0>, 'e', <   -Pi/2,     0,  0>)]: #6

# Right Leg
Joints := [op(Joints), struct('parent',  0, 'r', <              0,      -HipOffsetY,      -HipOffsetZ>, 'e', < -1/4*Pi,     0,  0>)]: #7
Joints := [op(Joints), struct('parent',  7, 'r', <              0,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #8
Joints := [op(Joints), struct('parent',  8, 'r', <              0,                0,                0>, 'e', <   -Pi/2, -Pi/4,  0>)]: #9
Joints := [op(Joints), struct('parent',  9, 'r', <    ThighLength,                0,                0>, 'e', <       0,     0,  0>)]: #10
Joints := [op(Joints), struct('parent', 10, 'r', <    TibiaLength,                0,                0>, 'e', <       0,     0,  0>)]: #11
Joints := [op(Joints), struct('parent', 11, 'r', <              0,                0,                0>, 'e', <    Pi/2,     0,  0>)]: #12

# Left arm
Joints := [op(Joints), struct('parent',  0, 'r', <              0,  ShoulderOffsetY,  ShoulderOffsetZ>, 'e', <   -Pi/2,     0,  0>)]: #13
Joints := [op(Joints), struct('parent', 13, 'r', <              0,                0,                0>, 'e', <    Pi/2,     0,  0>)]: #14
Joints := [op(Joints), struct('parent', 14, 'r', < UpperArmLength,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #15
Joints := [op(Joints), struct('parent', 15, 'r', <              0,                0,                0>, 'e', <       0, -Pi/2,  0>)]: #16
Joints := [op(Joints), struct('parent', 16, 'r', < LowerArmLength,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #17

# Right arm
Joints := [op(Joints), struct('parent',  0, 'r', <              0, -ShoulderOffsetY,  ShoulderOffsetZ>, 'e', <   -Pi/2,     0,  0>)]: #18
Joints := [op(Joints), struct('parent', 18, 'r', <              0,                0,                0>, 'e', <    Pi/2,     0,  0>)]: #19
Joints := [op(Joints), struct('parent', 19, 'r', < UpperArmLength,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #20
Joints := [op(Joints), struct('parent', 20, 'r', <              0,                0,                0>, 'e', <       0, -Pi/2,  0>)]: #21
Joints := [op(Joints), struct('parent', 21, 'r', < LowerArmLength,                0,                0>, 'e', <       0,  Pi/2,  0>)]: #22

# Head
Joints := [op(Joints), struct('parent',  0, 'r', <              0,                0,      NeckOffsetZ>, 'e', <       0,     0,  0>)]: #23
Joints := [op(Joints), struct('parent', 23, 'r', <              0,                0,                0>, 'e', <   -Pi/2,     0,  0>)]: #24

#==========================================================================================================================

# Definition of end-effectors

# Indexes
Torso := 1:
LLeg  := 2:
RLeg  := 3:
LHand := 4:
RHand := 5:
Head  := 6:

EE := [op(EE), struct('parent',  0, 'r', <           0,           0,            0>, 'e', <    0,     0,     0>)]: #Torso
EE := [op(EE), struct('parent',  6, 'r', < -FootHeight,           0,            0>, 'e', < Pi/2,  Pi/2,  Pi/2>)]: #LLeg
EE := [op(EE), struct('parent', 12, 'r', <  FootHeight,           0,            0>, 'e', <    0, -Pi/2,     0>)]: #RLeg
EE := [op(EE), struct('parent', 17, 'r', < HandOffsetZ,           0,  HandOffsetX>, 'e', <    0, -Pi/2,     0>)]: #LHand
EE := [op(EE), struct('parent', 22, 'r', < HandOffsetZ,           0,  HandOffsetX>, 'e', <    0, -Pi/2,     0>)]: #RHand
EE := [op(EE), struct('parent', 24, 'r', <           0,           0,            0>, 'e', < Pi/2,     0,     0>)]: #Head

#==========================================================================================================================

# Defines the mass and CoM position (in the TorsoFrame) of the torso link
TorsoLink := struct('r', <-0.004800000000000,  0.000060000000000,  0.042270000000000>, 'm', 1.026280):          #0

Links := [op(Links), struct('r', <-0.007170000000000, -0.010733880938412, -0.027520595923780>, 'm', 0.072440)]: #1
Links := [op(Links), struct('r', <-0.003563818177180,  0.003153696244092, -0.016490000000000>, 'm', 0.135300)]: #2
Links := [op(Links), struct('r', <-0.053860000000000,  0.001310000000000,  0.002010000000000>, 'm', 0.397980)]: #3
Links := [op(Links), struct('r', <-0.048910000000000,  0.004710000000000,  0.002100000000000>, 'm', 0.297060)]: #4
Links := [op(Links), struct('r', < 0.006380000000000,  0.001420000000000,  0.000280000000000>, 'm', 0.138920)]: #5
Links := [op(Links), struct('r', <-0.032080000000000, -0.003300000000000,  0.024890000000000>, 'm', 0.163040)]: #6

Links := [op(Links), struct('r', <-0.007170000000000, -0.010733880938412,  0.027520595923780>, 'm', 0.072440)]: #7
Links := [op(Links), struct('r', < 0.003563818177180,  0.003153696244092, -0.016490000000000>, 'm', 0.135300)]: #8
Links := [op(Links), struct('r', < 0.053860000000000, -0.001310000000000, -0.002010000000000>, 'm', 0.397980)]: #9
Links := [op(Links), struct('r', < 0.048910000000000, -0.004710000000000, -0.002100000000000>, 'm', 0.297060)]: #10
Links := [op(Links), struct('r', <-0.006380000000000, -0.001420000000000, -0.000280000000000>, 'm', 0.138920)]: #11
Links := [op(Links), struct('r', < 0.032080000000000, -0.003300000000000,  0.024890000000000>, 'm', 0.163040)]: #12

Links := [op(Links), struct('r', <-0.001780000000000, -0.000190000000000, -0.025070000000000>, 'm', 0.069840)]: #13
Links := [op(Links), struct('r', < 0.020670000000000,  0.003880000000000,  0.003620000000000>, 'm', 0.121660)]: #14
Links := [op(Links), struct('r', < 0.000200000000000, -0.000010000000000, -0.025730000000000>, 'm', 0.059590)]: #15
Links := [op(Links), struct('r', < 0.019400000000000,  0.003040000000000,  0.002500000000000>, 'm', 0.037700)]: #16
Links := [op(Links), struct('r', < 0.002720000000000,  0.002680000000000,  0.032410000000000>, 'm', 0.143140)]: #17

Links := [op(Links), struct('r', <-0.001780000000000, -0.000190000000000,  0.025070000000000>, 'm', 0.069840)]: #18
Links := [op(Links), struct('r', < 0.020670000000000, -0.003380000000000,  0.003620000000000>, 'm', 0.121660)]: #19
Links := [op(Links), struct('r', < 0.000200000000000,  0.000010000000000, -0.025730000000000>, 'm', 0.059590)]: #20
Links := [op(Links), struct('r', < 0.019400000000000, -0.003040000000000,  0.002500000000000>, 'm', 0.037700)]: #21
Links := [op(Links), struct('r', < 0.002720000000000, -0.002680000000000,  0.032410000000000>, 'm', 0.143140)]: #22

Links := [op(Links), struct('r', <-0.000030000000000,  0.000180000000000, -0.025730000000000>, 'm', 0.059590)]: #23
Links := [op(Links), struct('r', < 0.003830000000000, -0.051560000000000, -0.000930000000000>, 'm', 0.476710)]: #24

#==========================================================================================================================
