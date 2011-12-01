# from LLeg (see Readme_M.txt)

alias(fname = from_LLeg_1):

#---------------------------------------------------------------------
printf("form output ...\n");
st := time():

# Form Jacobian matrices
J_RL_from_LL := FormJacobian(RLeg,LLeg):
J_Torso_from_LL := FormJacobian(Torso,LLeg):

Jac := Matrix(12,12,0):
Jac[1...6,1..12] := J_RL_from_LL:
Jac[7..11,1..6] := J_Torso_from_LL[1..5,1..6]:
Jac[12,1..12] := <1|0|0|0|0|0|-1|0|0|0|0|0>: # constraint q[1] = q[7]

# FGM
JointFrames := FramesWorld(LLeg):
RLegT := EEWorld(JointFrames,RLeg):
TorsoT := EEWorld(JointFrames,Torso):

RL := vector(16): # Right leg transformation matrix
err1 := FormError(convert(transpose(RLegT),vector), RL):

T := vector(16): # Torso transformation matrix
err2 := FormError(convert(transpose(TorsoT),vector), T):

out := ArrayTools:-Concatenate(2,convert(transpose(Jac),vector),transpose(err1),transpose(err2[1..5]),0):

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

printf("makeproc ...\n");
st := time():

fname := makeproc(out,[q::array(1..linalg:-vectdim(q)), RL::array(1..linalg:-vectdim(RL)), T::array(1..linalg:-vectdim(T))]):

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

printf("Generating %s ...\n",cat(fname,".c"));
st := time():

fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */\n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

C(fname, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

