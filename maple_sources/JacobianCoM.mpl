# Example of forming the Jacobian of the CoM w.r.t. all joints using two methods:
# 1. form it "explicitly"
# 2. form it analytically (using linalg:-jacobian)
# The resultant matrices are identical (the first option results into faster code).
# For a derivation of the first approach see Antonio's thesis Appendix C (pp. 95).
# Interestingly enough, in my code from 2010-2011 I made a mistake and the implementation
# in mbs_simulator is wrong.
#

alias(fname = JacCoM):

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
    Je := FormJacobianFull(6+i+1, LLeg):

    if i=0 then
        J := J + TorsoLink:-m.Je[1..3,1..n]:
        m := m + TorsoLink:-m:
    else
        J := J + Links[i]:-m.Je[1..3,1..n]:
        m := m + Links[i]:-m:
    end if;

end do:
J := J/m:

J := convert(transpose(J),vector):

fname := makeproc(J,q::array(1..linalg:-vectdim(q))):

fd := fopen(cat("../",cat(fname,".c")),WRITE):
fprintf(fd,"/* Generated using codegen (%s) */ \n", StringTools:-FormatTime("%Y-%m-%d, %T")):
fclose(fd):
C(fname, optimized, filename = cat("../",cat(fname,".c")));

#---------------------------------------------------------------------

(*
JointFrames := FramesWorld(LLeg):
CM := SystemCoM(JointFrames):

J := linalg:-jacobian(CM,
                      <q[1],q[2],q[3],q[4],q[5],q[6],
                       q[7],q[8],q[9],q[10],q[11],q[12],
                       q[13],q[14],q[15],q[16],q[17],
                       q[18],q[19],q[20],q[21],q[22],
                       q[23],q[24]>):

J := convert(transpose(J),vector):

JacCoM_a := makeproc(J,q::array(1..linalg:-vectdim(q))):
C(JacCoM_a, optimized, filename = "../JacCoM_a.c");
*)
