// YarpCommDiffKin.h: interface for the CYarpCommPHRI class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _YARPCOMMDIFFKIN_H_
#define _YARPCOMMDIFFKIN_H_

#include <YarpCommunication.h>
#include "common.h"

class CYarpCommPHRI : public CYarpCommunication
{
public:
	CYarpCommPHRI( const std::string name, const std::string peerName = "", bool isServer = true );

	virtual ~CYarpCommPHRI() {};

protected:
	void Process( CMessage &msgIn, CMessage &msgOut, void *private_data = nullptr );

public:
	CMessage msg;
	//Flag
	int connected;
	int flag_all;// 1 : OK; 0: Error
	int flag_comm;
	int flag_dev;
	//Vision
	int neck;//Allows to trigger the rotation
	float v_pan,v_tilt;

	//Arm
	float jtheta[7];//Joint value
	float gripper;// Gripper value
	float fW[6];//Force at wrist

	//Object detection
	int object_type;
	int refresh;//Start from the first element of obs array
	float obs[20][4];// xyz+size	
	int obs_num; // Obs number
	int obs_f[20];// 1-Normal <0-Others
	//Target detection
	int mode;//1-obstacle 2-target
	float target[4][3];// x,y,z
	int tar_num;

	//Hand
	float jpos[4];  //0.6857;

	//Voice
	int command;
	//Mobile platform
	float mvel[2];
	//Obstacle from laser range finder
	float obs_g[85];

};

#endif // _YARPCOMMDIFFKIN_H_
