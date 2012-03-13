# FGM

#=====================================================================
st := time():

fname := "Leg2EE_new":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):
#=====================================================================

# ===================================================================================
# for convenience we use a homogeneous matrix to store the posture of the LLeg
# LL is used as an input parameter in the C functiona
# ===================================================================================
LL := vector(16): # Left leg transformation matrix <-- this is the support leg

q[28] := LL[1]: q[31] := LL[5]: q[34] :=  LL[9]: q[25] := LL[13]:
q[29] := LL[2]: q[32] := LL[6]: q[35] := LL[10]: q[26] := LL[14]:
q[30] := LL[3]: q[33] := LL[7]: q[36] := LL[11]: q[27] := LL[15]:
#        LL[4]:          LL[8]:          LL[12]:          LL[16]:
# ===================================================================================

JointFrames := FramesWorld(LLeg):

T := EEWorld(JointFrames,RLeg):
T := convert(transpose(T),vector):
LLeg2RLeg := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2RLeg, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Torso):
T := convert(transpose(T),vector):
LLeg2Torso := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2Torso, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,LHand):
T := convert(transpose(T),vector):
LLeg2LHand := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2LHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,RHand):
T := convert(transpose(T),vector):
LLeg2RHand := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2RHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Head):
T := convert(transpose(T),vector):
LLeg2Head := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2Head, optimized, filename = cat("../",cat(fname,".c")));

CM := SystemCoM(JointFrames):
LLeg2CoM := makeproc(CM,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2CoM, optimized, filename = cat("../",cat(fname,".c")));

#---------------------------------------------------------------------

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

JointFrames := FramesWorld(RLeg):

T := EEWorld(JointFrames,LLeg):
T := convert(transpose(T),vector):
RLeg2LLeg := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2LLeg, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Torso):
T := convert(transpose(T),vector):
RLeg2Torso := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2Torso, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,LHand):
T := convert(transpose(T),vector):
RLeg2LHand := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2LHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,RHand):
T := convert(transpose(T),vector):
RLeg2RHand := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2RHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Head):
T := convert(transpose(T),vector):
RLeg2Head := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2Head, optimized, filename = cat("../",cat(fname,".c")));

CM := SystemCoM(JointFrames):
RLeg2CoM := makeproc(CM,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2CoM, optimized, filename = cat("../",cat(fname,".c")));

#=====================================================================

T := Euler2HomogMatrix(x,y,z,X,Y,Z):
T := convert(transpose(T),vector):
Euler2T := makeproc(T,[x,y,z,X,Y,Z]):
C(Euler2T, optimized, filename = cat("../",cat(fname,".c")));

T := Euler2HomogMatrix(x,y,z,X,Y,Z):
R := convert(transpose(T[1..3,1..3]),vector):
Euler2Rot := makeproc(R,[X,Y,Z]):
C(Euler2Rot, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);
