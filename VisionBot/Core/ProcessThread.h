#ifndef _PROCESS_THREAD_H_
#define _PROCESS_THREAD_H_

#pragma once

#include <yarp/os/Thread.h>

#include "Grabber.h"
#include "ImageStream.h"
#include "ProcessImage.h"

#ifdef USE_StereoVidere
	#include "StereoProcess.h"
#endif

class CProcessThread 
	: public yarp::os::Thread  
{
public:
	CProcessThread( int task = Task::CONSTRUCTION_TOY_STEREO );
	~CProcessThread( void );

//---------------------------------------------------------------------------
// yarp::os::Thread -> funtion implementation
public:
	bool threadInit( void );
	void run( void );
	void threadRelease( void );

//---------------------------------------------------------------------------
// Accessors
public:
	void set_GrabberType( int type, std::string image_file_name = "" );

	int get_GrabberType( void );

	void ConfigureCurrentGrabber( void );

//---------------------------------------------------------------------------
// Initializing functions
	void Instantiate_Image_Processing( int task );

//---------------------------------------------------------------------------
// Core objects for this to work...
public:
	CGrabber * grabberObject;

	CProcessImage * image_procObject;

//---------------------------------------------------------------------------
// Image streaming objects
public:
	CImageStream 
		imgStream_orig,
		imgStream_proc;

//---------------------------------------------------------------------------
// Control variables
public:
	CAttachMutex_n<float> fFrameRate;
	
	CAttachMutex_n<int> nGrabberType;

	CAttachMutex_n<bool> 
		bStream_Image_Original,
		bStream_Image_Processed,
		bUse_Stereo;

private:
	std::string sFileImage;

//---------------------------------------------------------------------------
// Stereo Stuff
public:
	void Instantiate_Stereo( void );
	void Clean_Stereo( void );

#ifdef USE_StereoVidere
	CStereoProcess * pStereoProcess;
#endif

};

#endif // _PROCESS_THREAD_H_
