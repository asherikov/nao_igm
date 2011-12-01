# FGM of Joints, end-effectors and CoM (from both LLeg and RLeg)

#=====================================================================
st := time():

fname := "LLeg2JointsEE":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

JointFrames := FramesWorld(LLeg):

for i from 1 to n do
    T||(i) := convert(transpose(JointFrames[i]),vector):
end do:

E1 := EEWorld(JointFrames,Torso):
E1 := convert(transpose(E1),vector):

E2 := EEWorld(JointFrames,LLeg):
E2 := convert(transpose(E2),vector):

E3 := EEWorld(JointFrames,RLeg):
E3 := convert(transpose(E3),vector):

E4 := EEWorld(JointFrames,LHand):
E4 := convert(transpose(E4),vector):

E5 := EEWorld(JointFrames,RHand):
E5 := convert(transpose(E5),vector):

E6 := EEWorld(JointFrames,Head):
E6 := convert(transpose(E6),vector):

CM := SystemCoM(JointFrames):

T := ArrayTools:-Concatenate(2,
                             T1, T2, T3, T4, T5, T6,
                             T7, T8, T9,T10,T11,T12,
                             T13,T14,T15,T16,T17,
                             T18,T19,T20,T21,T22,
                             T23,T24,
                             E1,E2,E3,E4,E5,E6,
                             transpose(CM)):

LLeg2Joints := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(LLeg2Joints, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);

#=====================================================================
st := time():

fname := "RLeg2JointsEE":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

JointFrames := FramesWorld(RLeg):

for i from 1 to n do
    T||(i) := convert(transpose(JointFrames[i]),vector):
end do:

E1 := EEWorld(JointFrames,Torso):
E1 := convert(transpose(E1),vector):

E2 := EEWorld(JointFrames,LLeg):
E2 := convert(transpose(E2),vector):

E3 := EEWorld(JointFrames,RLeg):
E3 := convert(transpose(E3),vector):

E4 := EEWorld(JointFrames,LHand):
E4 := convert(transpose(E4),vector):

E5 := EEWorld(JointFrames,RHand):
E5 := convert(transpose(E5),vector):

E6 := EEWorld(JointFrames,Head):
E6 := convert(transpose(E6),vector):

CM := SystemCoM(JointFrames):

T := ArrayTools:-Concatenate(2,
                             T1, T2, T3, T4, T5, T6,
                             T7, T8, T9,T10,T11,T12,
                             T13,T14,T15,T16,T17,
                             T18,T19,T20,T21,T22,
                             T23,T24,
                             E1,E2,E3,E4,E5,E6,
                             transpose(CM)):

RLeg2Joints := makeproc(T,q::array(1..linalg:-vectdim(q))):
C(RLeg2Joints, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);
