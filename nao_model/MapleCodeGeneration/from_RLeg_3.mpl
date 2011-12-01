# from RLeg (see Readme_M.txt and JacobianCoM.mpl)

alias(fname = from_RLeg_3):

#---------------------------------------------------------------------
printf("form output ...\n");
st := time():

# Form Jacobian matrices
J_LL_from_RL := FormJacobian(LLeg,RLeg):
J_Torso_from_RL := FormJacobian(Torso,RLeg):

JointFrames := FramesWorld(RLeg):
CM := SystemCoM(JointFrames):

# Associate an end-effector with the CoM of each link
# (because I don't want to change the function FormJacobianFull)
EE:=EE[1..6]: # leave only the first 6 in the list in case others have been added
EE := [op(EE), struct('parent',  0, 'r', TorsoLink:-r, 'e', < 0,0,0>)]:
for i from 1 to n do
    EE := [op(EE), struct('parent',  i, 'r', Links[i]:-r, 'e', < 0,0,0>)]:
end do:

m := 0:
J := Matrix(3,24,0):
for i from 0 to n do
    Je := FormJacobianFull(6+i+1, RLeg):

    if i=0 then
        J := J + TorsoLink:-m.Je[1..3,1..n]:
        m := m + TorsoLink:-m:
    else
        J := J + Links[i]:-m.Je[1..3,1..n]:
        m := m + Links[i]:-m:
    end if;

end do:
J := J/m: # Jacobian of the CoM

Jac := Matrix(12,12,0):
Jac[1...6,1..12] := J_LL_from_RL:
Jac[7..9,1..12] := J[1..3,1..12]:
Jac[10..11,7..12] := J_Torso_from_RL[4..5,1..6]:
Jac[12,1..12] := <1|0|0|0|0|0|-1|0|0|0|0|0>: # constraint q[1] = q[7]

# FGM
LLegT := EEWorld(JointFrames,LLeg):
TorsoT := EEWorld(JointFrames,Torso):

LL := vector(16): # Left leg Transformation matrix
err1 := FormError(convert(transpose(LLegT),vector), LL):

CoM := vector(3): # CoM position
CM := SystemCoM(JointFrames):
err2 := <CoM[1] - CM[1], CoM[2] - CM[2], CoM[3] - CM[3]>:

Rot := vector(9): # Torso rotation matrix
err3 := FormErrorRot(convert(transpose(TorsoT[1..3,1..3]),vector), Rot):

out := ArrayTools:-Concatenate(2,convert(transpose(Jac),vector),transpose(err1),transpose(err2),transpose(err3[1..2]),0):

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

printf("makeproc ...\n");
st := time():

fname := makeproc(out,[q::array(1..linalg:-vectdim(q)), LL::array(1..linalg:-vectdim(LL)), CoM::array(1..linalg:-vectdim(CoM)), Rot::array(1..linalg:-vectdim(Rot))]):

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

printf("Generating %s ...\n",cat(fname,".c"));
st := time():

fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

C(fname, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

