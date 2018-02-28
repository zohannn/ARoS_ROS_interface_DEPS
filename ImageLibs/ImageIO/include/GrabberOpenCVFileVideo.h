#ifndef _GRABBER_OPENCV_FILE_VIDEO_H_
#define _GRABBER_OPENCV_FILE_VIDEO_H_

#pragma once

#include "GrabberOpenCVCapture.h"

#include <string>

/// <summary>Class that wrapps the OpenCV video file capture object into the
/// CGrabber structure.</summary>
class CGrabberOpenCVFileVideo 
	: public CGrabberOpenCVCapture
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the path of the file to open.</summary>
	/// <param name="file_name">Path to file.</param>
	CGrabberOpenCVFileVideo( std::string file_name = "" );

//---------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Initialize capture with the previously specified file.</summary>
	/// <returns>true if the camera was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Initialize capture with the given file.</summary>
	/// <param name="file_name">Path to file.</param>
	/// <returns>true if the camera was successfully initialized, false if failed.</returns>
	bool OpenGrabber( std::string file_name = "" );

//---------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Acquire a new frame.</summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	bool get_Frame( void );
	
//---------------------------------------------------------------------------
// Variables to control video playback
private:
	/// <summary>Counter to track the current frame number.</summary>
	double dFrameCount;

public:
	/// <summary>Mutex protected flag to signal if the video should loop at the end or not.</summary>
	CAttachMutex_n<bool> bVideoLoop;
};

#endif // _GRABBER_OPENCV_FILE_VIDEO_H_
