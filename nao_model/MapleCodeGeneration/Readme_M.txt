Information about functions.

ToDo:

1. To test with visualization (before testing on the robot).
2. To test an igm formulation with joitn angle limits (for this I will need a QP solver - maybe qpOASES)

Notes: 

1. All arrays are stored column-wise (Fortran convention)
2. Due to the fact that q[0] and q[6] should have the same value, we could either 
   -- impose this explicitly as a constraint
   -- eliminate one of the two variables.
   Now I take the former approach, while in mbs_simulator I was taking the latter one.
   There seems to be no difference in execution time.
3. Using Maple I can generate code for J*J' and it seems that this results in faster computation 
   (but not by much) so for the moment I form J*J' online (using BLAS). Generating code for J*J'
   is a bit slow (takes around 15 min.) but maybe in the end this is what we will use. 

-----------------------------------------------------------------------------------

model_NAO.mpl   : defines a model of the NAO robot

kinematics.mpl  : defines functions used to solve fgm and form Jacobian matrices

main_NAO.mw     : file that is executed in Maple (not readable from emacs)

-----------------------------------------------------------------------------------

from_LLeg_1.mpl : Generates a file from_LLeg_1.c that contains the function
		  void from_LLeg_1(q,RL,T,A)

		  q[n+nB]  - Joint angles and parameters defining the posture of the base (LLeg)
		  	     n = 24 (the number of joints)
		  	     if RotationsRepresentation = 0, nB = 3+3 (Euler angles) 
		  	     if RotationsRepresentation = 1, nB = 3+4 (unit quaternion)
		  	     if RotationsRepresentation = 2, nB = 3+9 (rotation matrix) <-- used now
		  RL[4*4]  - A homogeneous matrix specifying the desired posture of RLeg
		  T[4*4]   - A homogeneous matrix specifying the desired posture of Torso	
		  A[156]   - An output array
		             -- The first 12*12 elements store the following matrix
		  	        J = [J1;
  			             J2;
 			             1 0 0 0 0 0 -1 0 0 0 0 0]
 			        --- J1 is the Jacobian of the RLeg w.r.t the motion of q[0]...q[11].
			        --- J2 is the Jacobian of the Torso w.r.t the motion of q[0]...q[11].
			            Note that since LLeg is the base, the motion of the Torso does not 
				    depend on q[6]...q[11]
			        --- [1 0 0 0 0 0 -1 0 0 0 0 0] is used to define a constraint q[0] = q[6]
			     -- The next 11 elements represent an error from the current postures of 
			        RLeg and Torso and the desired ones (given by RL and T)
			        [err_RLeg_xPosition;
				 err_RLeg_yPosition;	
				 err_RLeg_zPosition;
				 err_RLeg_xRotation;
		  		 err_RLeg_yRotation;
				 err_RLeg_zRotation;
				 err_Torso_xPosition;
				 err_Torso_yPosition;
				 err_Torso_zPosition;
				 err_Torso_xRotation;				 
				 err_Torso_yRotation]
				 Note that we do not impose constraint on the torso rotation around the z axis 
				 and thats why err_Torso_zRotation is not considered
			     -- The final element is 0 (this is the right-hand-side of the constraint q[0]-q[6] = 0)

-----------------------------------------------------------------------------------

from_RLeg_1.mpl : Generates a file from_RLeg_1.c that contains the function
		  void from_RLeg_1(q,LL,T,A)

		  q[n+nB]  - Joint angles and parameters defining the posture of the base (RLeg)
		  	     n = 24 (the number of joints)
		  	     if RotationsRepresentation = 0, nB = 3+3 (Euler angles) 
		  	     if RotationsRepresentation = 1, nB = 3+4 (unit quaternion)
		  	     if RotationsRepresentation = 2, nB = 3+9 (rotation matrix) <-- used now
		  LL[4*4]  - A homogeneous matrix specifying the desired posture of LLeg
		  T[4*4]   - A homogeneous matrix specifying the desired posture of Torso	
		  A[156]   - An output array
		             -- The first 12*12 elements store the following matrix
		  	        J = [J1;
  			             J2;
 			             1 0 0 0 0 0 -1 0 0 0 0 0]
 			        --- J1 is the Jacobian of the LLeg w.r.t the motion of q[0]...q[11].
			        --- J2 is the Jacobian of the Torso w.r.t the motion of q[0]...q[11].
			            Note that since RLeg is the base, the motion of the Torso does not 
				    depend on q[0]...q[5]
			        --- [1 0 0 0 0 0 -1 0 0 0 0 0] is used to define a constraint q[0] = q[6]
			     -- The next 11 elements represent an error from the current postures of 
			        LLeg and Torso and the desired ones (given by RL and T)
			        [err_LLeg_xPosition;
				 err_LLeg_yPosition;	
				 err_LLeg_zPosition;
				 err_LLeg_xRotation;
		  		 err_LLeg_yRotation;
				 err_LLeg_zRotation;
				 err_Torso_xPosition;
				 err_Torso_yPosition;
				 err_Torso_zPosition;
				 err_Torso_xRotation;				 
				 err_Torso_yRotation]
				 Note that we do not impose constraint on the torso rotation around the z axis 
				 and thats why err_Torso_zRotation is not considered
			     -- The final element is 0 (this is the right-hand-side of the constraint q[0]-q[6] = 0)

-----------------------------------------------------------------------------------

from_LLeg_2.mpl : Generates a file from_LLeg_2.c that contains the function
		  void from_LLeg_2(q,RL,CoM,Rot,A)

		  q[n+nB]  - Joint angles and parameters defining the posture of the base (LLeg)
		  	     n = 24 (the number of joints)
		  	     if RotationsRepresentation = 0, nB = 3+3 (Euler angles) 
		  	     if RotationsRepresentation = 1, nB = 3+4 (unit quaternion)
		  	     if RotationsRepresentation = 2, nB = 3+9 (rotation matrix) <-- used now
		  RL[4*4]  - A homogeneous matrix specifying the desired posture of RLeg
		  CoM[3]   - Desired position of the CoM
		  Rot[3*3] - A rotation matrix specifying the desired orientation of Torso	
		  A[156]   - An output array
		             -- The first 12*12 elements store the following matrix
		  	        J = [J1;
  			             J2;
 			             1 0 0 0 0 0 -1 0 0 0 0 0]
 			        --- J1 is the Jacobian of the RLeg w.r.t the motion of q[0]...q[11].
			        --- J2 is the Jacobian of the Torso w.r.t the motion of q[0]...q[11].
			            Note that since LLeg is the base, the motion of the Torso does not 
				    depend on q[6]...q[11]
			        --- [1 0 0 0 0 0 -1 0 0 0 0 0] is used to define a constraint q[0] = q[6]
			     -- The next 11 elements represent an error as follows
			        [err_RLeg_xPosition;
				 err_RLeg_yPosition;	
				 err_RLeg_zPosition;
				 err_RLeg_xRotation;
		  		 err_RLeg_yRotation;
				 err_RLeg_zRotation;
				 err_CoM_xPosition;
				 err_CoM_yPosition;
				 err_CoM_zPosition;
				 err_Torso_xRotation;				 
				 err_Torso_yRotation]
				 Note that we do not impose constraint on the torso rotation around the z axis 
				 and thats why err_Torso_zRotation is not considered
			     -- The final element is 0 (this is the right-hand-side of the constraint q[0]-q[6] = 0)

		  Note: The only difference with the function void from_LLeg_1(q,RL,T,A) is that instead of error for
		  torso position error of CoM position is formed. 
		  
		  Note: Note that J2(1:3,:) is the Jacobian of the Torso position (not the CoM position) w.r.t. joints 0..11.
		        Even though this is incorrect, the results seem to be reasonable 
			(the real Jacobian of the CoM is used in igm_3(...)). 

-----------------------------------------------------------------------------------

from_RLeg_2.mpl : Generates a file from_RLeg_2.c that contains the function
		  void from_RLeg_2(q,LL,CoM,Rot,A)

		  The idea is the same as for from_LLeg_2.mpl (but when RLeg is the base)

-----------------------------------------------------------------------------------

from_LLeg_3.mpl : Similar to from_LLeg_2.mpl but J2(1:3,:) is the Jacobian of the CoM 

-----------------------------------------------------------------------------------

from_RLeg_3.mpl : Similar to from_LLeg_3.mpl but for RLeg

-----------------------------------------------------------------------------------

from_LLeg_4.mpl: Similar to from_LLeg_3.mpl but all 24 joints are considered. And 
		 dq'*(mu*z) is added to the objective function in order to obtian 
		 "repetative" solution.

-----------------------------------------------------------------------------------

from_RLeg_4.mpl: Similar to from_LLeg_4.mpl but for RLeg

-----------------------------------------------------------------------------------

Leg2EE.mpl      : Generates a file Leg2EE.c that contains the following functions 
		void LLeg2RLeg(q,T)          : fgm of RLeg from LLeg
		void LLeg2Torso(q,T)         : fgm of Torso from LLeg
		void LLeg2LHand(q,T)         : fgm of LHand from LLeg
		void LLeg2RHand(q,T)         : fgm of RHand from LLeg
		void LLeg2Head(q,T)          : fgm of Head from LLeg
		void LLeg2CoM(q,A)           : fgm of CoM from LLeg
		void RLeg2LLeg(q,T)          : fgm of LLeg from RLeg
		void RLeg2Torso(q,T)         : fgm of Torso from RLeg
		void RLeg2LHand(q,T)         : fgm of LHand from RLeg
		void RLeg2RHand(q,T)         : fgm of RHand from RLeg
		void RLeg2Head(q,T)          : fgm of Head from RLeg
		void RLeg2CoM(q,A)           : fgm of CoM from RLeg

		All above functions return T (a 4x4 homogeneous matrix) and take as an input argument the joint angles q
		
		Note: If RotationsRepresentation = 0, then q[24]...q[29] define the posture of the base. 
		      -- for the functions LLeg2... q[24]...q[29] specify the posture of LLeg
		      -- for the functions RLeg2... q[24]...q[29] specify the posture of RLeg
		
		void Euler2T(x,y,z,X,Y,Z,T)  : returns a 4x4 homogeneous matrix given 
		                               (x,y,z) position 
					       (X,Y,Z) rotation (X->Y->Z current axis Euler angles)

		void Euler2Rot(X,Y,Z,R)      : returns a 3x3 rotation matrix given 
					       (X,Y,Z) rotation (X->Y->Z current axis Euler angles)

		Note: #include <math.h> appears multiple times in Leg2EE.c, but this is not a problem 
		(I am too lazy to remove it).

-----------------------------------------------------------------------------------

JacobianCoM.mpl : Forms the Jacobian of the CoM using two methods

-----------------------------------------------------------------------------------
