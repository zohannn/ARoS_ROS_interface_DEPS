#pragma once
#include <yarp/os/Thread.h>
#include "stdafx.h"
#include "YarpCommPHRI.h"
// This class is to create threads to control indirect modules
class CModuleThread : public yarp::os::Thread
{
public:
	CModuleThread(void);
	~CModuleThread(void);
	bool threadInit( void );
	void run( void );
	void threadRelease( void );
	void handOpen();

	//Module flags
	bool started;
	bool stop;// set 1 to stop the thread
	int mod_type;// 1-Hand 2-Neck (Pan-Tilt module via Vision)
	int mode;//Function mode of the module

	//Flag for hand module
	int catched;
	int closed;
	float angle_f;// Fingers angle

	//Flag for pan-tilt
	float value_pan;
	float value_tilt;


	CYarpCommPHRI *YMOD; // Module pointer
};
