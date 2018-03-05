#pragma once
#include "stdafx.h"
#include "Matrix.h"
#include "math.h"
/* 
	 This class contains all main computations to control Anthropomorphic robots --- ( Hung Mai VU (2012) )
*/
#define M_PI 3.14159265358979323846f
#define Obs_N 10
class CArosArm 
{
private:
	//Robotic arm geometric params
	float L1;
	float L3;
	float L5;
	float L7;
	//Other params
	float joint_limit[2][7];
	float joint_offset[7];
	float velocity_max[7];	
	//------------Internal functions
	//Calculate transformation matrix
	CMatrix<float> fTransMatrix(float a, float alpha, float d, float theta);
	CMatrix<float> fTransRPY();// For controlling roll, pitch, yaw velocity	directly
	CMatrix<float> fJacobian(float *jtheta);//Calculate Jacobian
	//Check joints' value if in their limit
	int fCheckRange(float jangle,int index); // For geometric kinematics
	int fCheckRangeAbsolute(float jangle,int index); // For differential kinematics
	
public:
	float jtheta_n[7]; // Joints value now
	float jtheta_p[7]; // Joints value previous
	//Behaviour flag
	int jlimEnable;
	int singEnable;
	int obsEnable;	
	int control_e; //Control end-effecter orientation flag for drawing tasks
	//---------------Joint limit coefficients
	float Kj1;
	float Kj2;
	float Kj3;
	float Kj4;
	//--------------Singularity coefficients
	float Ks1;
	float Ks2;
	float Ks3;
	//--------------Obstacle coefficients
	float Ko1;
	float Ko2;
	float Ko3;

	float Koff1;
	float Koff2;	
	float Kor;

	float K_fm;
	float K_fr;
	// Remember to reset Old gradients
	float gradp[7][5];// Old gradients
	float timp;// Previous time 
	float tims;// Start time
	float tim_ref; // Time to be chosen as a reference
	long start; // Start time
	
	//--------------Integration of behaviours
	float Kn1;
	float Kn2;
	//-------------Obstacles
	int obs_num;
	float obs_array[20][4];// xyz+size
	int obs_f[20];// 1-Normal obstacles <0 - Special obstacle

	//For calibration, compensating the weight of end-effector
	float theta_f;// Orientation of the sensor
	float Lf;//Position of the sensor
	float spline[4][5][5];

	float fW[6];// Current force in base reference frame
	float fW0[6]; // Force in the beginning
	// Force Filtering
	float fWp[6];// Old force
	float K_fmax; //Force filtering parameters
	float K_fmin;
	float K_for;
	float K_mor;
	float timfp;
	//Force processing
	int flag_torquez;//Eliminate secondary torque
	int flag_torquey;//Eliminate secondary torque
	
	// End-effector velocities filtering
	float dposp[6];
	float timEp;
	//End-effecter info
	float Oef[3];
	float ypr[3];//Yaw pitch roll
	float Owr[3],Oel[3]; // Wrist, Elbow position
	//Mobile platform
	float ltar;
	float beta1,beta2;
	float lfor;
	int sensor_number;
	float sensor_sector;	
	float rrobot;
	float vmobp[2];
	//Recovery
	float vmob0[2];
	float tim_re;
	int flag_dir;
	int flag_re;
	float obs_angle[85];// Ignore 9 21 22 //25 26 59 60
	//Obstacle in the ground
	float obs_g[85];

	float timIntegrate;
	float errYPR[3];// PID for roll angle
	float errYPRp[3];
	float Kpw,Tiw,Tdw,Nw;
	float dyprp[3];	
	//Mobile manipulator Integration
	float Fmax[6],Fmin[6],k_force,k_moment;	

	// Virtual Elastic System
	float L_ves[3],L_ves0[3],dL_ves[3],ddL_ves[3],F_ves[3];
	float k_m[3],k_v[3],k_e[3];
	float dLmax[3],L_limit[3][2];

	float F_ds,M_ds,F_dsp,M_dsp; // Imaginary force and moment from Dynamical System
		
	float vendp[6],F_ypr[6];

	int flag_ypr,normal;
	int see_target,get_target;
	float target[4];
	float targetp[4];
	int flag_comp;// For compensation the movement of the mobile platform
	int Mcontrol;// Human, robot leading flag
	int flag_say; // Allow to say
	int flag_hurt;
	int flag_grasp; // Allow to grasp
	int flag_obs;  // Having difficulty with obstacles on the ground
	int finish;
	int avoidobs;// Mobile platform avoids obstacles
	
	//----------Utility function
	//Copy source array to distination array
	void  fCopy(float *dist,float *source, int length);
	void fCopyMatrix(CMatrix<float> *dist,CMatrix<float> *source,int rowd,int cold,int rows0,int cols0,int rows1,int cols1);
	
	//Get position of S,E,W,H
	void fPosition(float *jtheta,float *shoulder,float *elbow,float *wrist,float *hand);
	// Product vector (V1-V2)*(V3-V4)
	float fprod(float *V1,float *V2,float *V3,float *V4);
	// Cross product vector V1^V2
	void fcrossprod(float *V1,float *V2, float *result);
	// End-effector orientation info
	void fEndEffectorInfo(float *jtheta,float *X7, float *Y7, float *Z7);
	// End-effector position info
	void fEndEffectorInfo(float *jtheta,float *O7);	
	// Convert Force at End-effector fE to fE_b in base reference frame with compensation for end-effector
	void fForceCompensated(float *jtheta,float *fE,float *fE_b);
	void fForceStartComp(float *jtheta,float *fE,float tim); // Substract the initial reading
	// Convert Force at End-effector fE to fE_b in base reference frame without compensation
	void fForceEnd(float *jtheta,float *fE,float *fE_b);
	// Calculate force value from the orientation of the end-effector
	float fSpline2D(float theta, float phi, int comp); 
	// Return forces after compensating and filtering
	void fForceProcessing(float *jtheta,float *fE, float tim);
	// Smooth End-effector's velocities
	void fVelSmoothing(float *dpos,float tim);
	//Integrating with mobile platform	
	void fMobile(float *dmob, float tim); // use the Virtual Elastic System approach 
	//mode=0->by force   mode=1->by target   mode=3->by force for transportation 
	void fEndEffector(float *dmob,float *dpos,float tim,int mode);
	void fReaction(float *dpos,int *tomove);// Generate end-effector velocity from forces
	// Look at the target
	void fPanTilt(float *tar,float *pan, float *tilt);
	//Reset 
	void fReset(float tim);
	// Mobile movement
	void fMobileDriving(float *dmob);// Mobile autonomous driving
	void fCalculateTarget(float *dmob,float *pos,float dtim); // Estimate the target position when vision info is unavailable temporarily

	
	//------------Constructor
	CArosArm(void);
	//-----------------Destructor
	~CArosArm(void);

	//------------Direct Geometric kinematrics
	void GeoKine(float *jtheta,float *pos,float *ypr0);

	//------------Inverse Geometric kinematrics
	//Search for the suitable value of the plane angle if flag=1
	// Mode=0: Euler fixed angle =1: Yaw Pitch Roll
	int InvGeoKine(float* pos,float* jtheta,float *plane_angle,int flag,int mode);

	//------------Direct Differential kinematrics
	void DiffKine(float *jtheta,float* djtheta,float* dpos);

	//------------Inverse Differential kinematrics
	int InvDiffKine(float* dpos,float *jtheta,float* djtheta,int *mask,float tim);

};