# from RLeg (see Readme_M.txt)

alias(fname = from_RLeg_1_new):

#---------------------------------------------------------------------
printf("form output ...\n");
st := time():

# ===================================================================================
# for convenience we use a homogeneous matrix to store the posture of the RLeg
# RL is used as an input parameter in the C functiona
# ===================================================================================
RL := vector(16): # Right leg transformation matrix <-- this is the support leg

q[28] := RL[1]: q[31] := RL[5]: q[34] :=  RL[9]: q[25] := RL[13]:
q[29] := RL[2]: q[32] := RL[6]: q[35] := RL[10]: q[26] := RL[14]:
q[30] := RL[3]: q[33] := RL[7]: q[36] := RL[11]: q[27] := RL[15]:
#        RL[4]:          RL[8]:          RL[12]:          RL[16]:
# ===================================================================================

# Form Jacobian matrices
J_LL_from_RL := FormJacobian(LLeg,RLeg):
J_Torso_from_RL := FormJacobian(Torso,RLeg):

Jac := Matrix(11,12,0):
Jac[1...6,1..12] := J_LL_from_RL:
Jac[7..9,7..12] := J_Torso_from_RL[1..3,1..6]:
Jac[10,7..12] := J_Torso_from_RL[5,1..6]:
Jac[11,1..12] := <1|0|0|0|0|0|-1|0|0|0|0|0>: # constraint q[1] = q[7]

# FGM
JointFrames := FramesWorld(RLeg):
LLegT := EEWorld(JointFrames,LLeg):
TorsoT := EEWorld(JointFrames,Torso):

LL := vector(16): # Left leg Transformation matrix
err1 := FormError(convert(transpose(LLegT),vector), LL):

T := vector(16): # Torso Transformation matrix
err2 := FormError(convert(transpose(TorsoT),vector), T):

out := ArrayTools:-Concatenate(2,convert(transpose(Jac),vector),transpose(err1),transpose(err2[1..3]),transpose(err2[5]),0):

printf("completed in %f seconds\n\n",time()-st);

#---------------------------------------------------------------------

printf("makeproc ...\n");
st := time():

fname := makeproc(out,[q::array(1..n), RL::array(1..linalg:-vectdim(RL)), LL::array(1..linalg:-vectdim(LL)), T::array(1..linalg:-vectdim(T))]):

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

