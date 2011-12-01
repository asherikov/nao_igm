# FGM

#=====================================================================
st := time():

fname := "Leg2EE":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):
#=====================================================================

JointFrames := FramesWorld(LLeg):

T := EEWorld(JointFrames,RLeg):
T := convert(transpose(T),vector):
LLeg2RLeg := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2RLeg, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Torso):
T := convert(transpose(T),vector):
LLeg2Torso := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2Torso, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,LHand):
T := convert(transpose(T),vector):
LLeg2LHand := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2LHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,RHand):
T := convert(transpose(T),vector):
LLeg2RHand := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2RHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Head):
T := convert(transpose(T),vector):
LLeg2Head := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2Head, optimized, filename = cat("../",cat(fname,".c")));

CM := SystemCoM(JointFrames):
LLeg2CoM := makeproc(CM,q::array(1..linalg:-vectdim(q))):
C(LLeg2CoM, optimized, filename = cat("../",cat(fname,".c")));

#---------------------------------------------------------------------

JointFrames := FramesWorld(RLeg):

T := EEWorld(JointFrames,LLeg):
T := convert(transpose(T),vector):
RLeg2LLeg := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2LLeg, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Torso):
T := convert(transpose(T),vector):
RLeg2Torso := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2Torso, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,LHand):
T := convert(transpose(T),vector):
RLeg2LHand := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2LHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,RHand):
T := convert(transpose(T),vector):
RLeg2RHand := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2RHand, optimized, filename = cat("../",cat(fname,".c")));

T := EEWorld(JointFrames,Head):
T := convert(transpose(T),vector):
RLeg2Head := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2Head, optimized, filename = cat("../",cat(fname,".c")));

CM := SystemCoM(JointFrames):
RLeg2CoM := makeproc(CM,q::array(1..linalg:-vectdim(q))):
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
