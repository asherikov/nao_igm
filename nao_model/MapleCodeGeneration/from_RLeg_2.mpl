# from RLeg (see Readme_M.txt)

alias(fname = from_RLeg_2):

#---------------------------------------------------------------------
printf("form output ...\n");
st := time():

# Form Jacobian matrices
J_LL_from_RL := FormJacobian(LLeg,RLeg):
J_Torso_from_RL := FormJacobian(Torso,RLeg):

Jac := Matrix(12,12,0):
Jac[1...6,1..12] := J_LL_from_RL:
Jac[7..11,7..12] := J_Torso_from_RL[1..5,1..6]:
Jac[12,1..12] := <1|0|0|0|0|0|-1|0|0|0|0|0>: # constraint q[1] = q[7]

# FGM
JointFrames := FramesWorld(RLeg):
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

