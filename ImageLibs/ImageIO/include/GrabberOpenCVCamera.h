#ifndef _GRABBER_OPENCV_CAMERA_H_
#define _GRABBER_OPENCV_CAMERA_H_

#pragma once

#include "GrabberOpenCVCapture.h"

/// <summary>Class that wrapps the OpenCV Camera capture object into the
/// CGrabber structure.</summary>
class CGrabberOpenCVCamera
	: public CGrabberOpenCVCapture
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabberOpenCVCamera( int width = 640, int height = 480 );

//---------------------------------------------------------------------------
// Open grabber
protected:
	/// <summary>Initialize capture with the first available camera.</summary>
	/// <returns>true if the camera was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );
	
	/// <summary>Initialize capture with the specified camera.</summary>
	/// <param name="nCamera">Camera ID.</param>
	/// <returns>true if the camera was successfully initialized, false if failed.</returns>
	bool OpenGrabber( int nCamera );
};

#endif // _GRABBER_OPENCV_CAMERA_H_
