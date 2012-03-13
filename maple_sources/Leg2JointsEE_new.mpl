# FGM of Joints, end-effectors and CoM (from both LLeg and RLeg)

#=====================================================================
st := time():

fname := "LLeg2JointsEE_new":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

# ===================================================================================
# for convenience we use a homogeneous matrix to store the posture of the LLeg
# LL is used as an input parameter in the C function
# ===================================================================================
LL := vector(16): # Left leg transformation matrix <-- this is the support leg

q[28] := LL[1]: q[31] := LL[5]: q[34] :=  LL[9]: q[25] := LL[13]:
q[29] := LL[2]: q[32] := LL[6]: q[35] := LL[10]: q[26] := LL[14]:
q[30] := LL[3]: q[33] := LL[7]: q[36] := LL[11]: q[27] := LL[15]:
#        LL[4]:          LL[8]:          LL[12]:          LL[16]:
# ===================================================================================


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

LLeg2Joints := makeproc(T,[q::array(1..n),LL::array(1..linalg:-vectdim(LL))]):
C(LLeg2Joints, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);

#=====================================================================
st := time():

fname := "RLeg2JointsEE_new":
printf("Generating %s ...\n",cat(fname,".c"));
fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):

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

RLeg2Joints := makeproc(T,[q::array(1..n),RL::array(1..linalg:-vectdim(RL))]):
C(RLeg2Joints, optimized, filename = cat("../",cat(fname,".c")));

printf("completed in %f seconds\n\n",time()-st);
