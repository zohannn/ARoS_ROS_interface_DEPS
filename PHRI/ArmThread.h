#pragma once
#include <yarp/os/Thread.h>
#include "stdafx.h"
#include "YarpCommPHRI.h"
#include "ArosArm.h"
#include <fstream>
#include "ModuleThread.h"

// This class is responsible to perform tasks and start the main task thread --- ( Hung Mai VU (2012) )
class CArmThread : public yarp::os::Thread
{
public:
	CArmThread(void);
	~CArmThread(void);
	// yarp::os::Thread -> function implementation
public:	
	bool threadInit( void );
	void run( void );
	void threadRelease( void );

	//Thread flags
	bool started;
	bool stop;	
	float tim,tims,timp;// Time now, start and previous
	int mode;// Task
	int modep;//Previous task

	//-----Inputs for differential kinematics - Arm
	int mask[6];  // Velocity components need to be controlled 1-Control 0-Floating
	float jtheta[7]; //Temporary joints value
	float djtheta[7]; // Temporary joints velocity
	float dpos[6]; // Temporary desired end-effector velocity
	float dpos0[6];	 // Desired velocity input from GUI
	float dpos_check[6];// For direct kinematics - Checking purpose
	float pos[3];//End-effector position

	// Mobile platform
	float dmob[2]; // Temporary velocity
	float dmobp[2]; // Previous velocity

	//Smooth start end-effector velocity
	int dpos_s;// Flag 0: No smooth start 1: Smooth start
	float K_time; // Time constant for smoothing

	//---PID parameters for force control Kp(1+1/(Ti.s)+Td.s/(Td.s/N+1))
	float Kpf;//Force
	float Tif;
	float Tdf;
	float Nf;// low-pass filter
	float Kpm;//Moment
	float Tim;
	float Tdm;
	float Nm;// low-pass filter	
	float fW[6];

	//-------Target 
	float target[3];
	float vtar[3];
	float dist0;
	int see_tar;
	int search_handle,search_tar,search_loc;
	//Vision
	float pan,tilt;
	int rotating;
	float tim_rot;


	// Other flags
	int flag_armstop; // Arm stop because joints velocity exceed
	int flag_notify;
	float tim_notify;// Time when notify at the first time
	float dtt; // Time step
	int stage;
	int flag_neck;

	//-----------
	// Direct modules
	CYarpCommPHRI *YPHRI;  // Arm	
	CArosArm * ArosArm;   // General Anthropomorphic robot controller
	CYarpCommPHRI *YMOB;  //Mobile platform
	CYarpCommPHRI *YOBS;  // Laser range finder
	CYarpCommPHRI *YSPE;  // Speech
	CYarpCommPHRI *YVIS;  // Speech

	// Threads to control other modules
	CModuleThread HandThread; 	
	//CModuleThread VoiceThread;
	
};
