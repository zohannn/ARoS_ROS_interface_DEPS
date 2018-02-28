#ifndef _GRABBER_CMU1394_H_
#define _GRABBER_CMU1394_H_

#include "Grabber.h"

#ifdef USE_CMU1394
//#include <Windows.h>
	#define _X86_
	#include <windef.h>
	#include <winbase.h>
	#include <winuser.h>
	#include <winreg.h>
#include <1394Camera.h>
#endif //USE_CMU1394

class CGrabberCMU1394
	: public CGrabber
{
public:
	enum _FRAME_RATE : int {
		FPS_2 = 0,
		FPS_4,
		FPS_7,
		FPS_15,
		FPS_30,
		FPS_60,
		FPS_120,
		FPS_240,
	};

public:
	/// <summary>Default constructor. Accepts the image size and frame rate.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="FrameRate">Frame rate definition ( FPS_2, FPS_4, FPS_7, FPS_15, FPS_30, FPS_60, FPS_120, FPS_240).</param>
	CGrabberCMU1394( int Width = 640, int Height = 480, _FRAME_RATE FrameRate = FPS_15 );
	
	/// <summary>Class destructor. Stops grabber if opened.</summary>
	virtual ~CGrabberCMU1394( void );

protected:
	/// <summary>Specific implementation of CGrabberCMU1394 initialization.</summary>
	/// <returns>true if grabber was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Specific implementation of CGrabberCMU1394 closing.</summary>
	/// <returns>true if grabber was successfully closed, false if failed.</returns>
	bool CloseGrabber( void );

public:
	/// <summary>Acquire a new frame.</summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	bool get_Frame( void );

	/// <summary>Get the last acquired frame into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Image( cv::Mat * mat_ptr );

private:
	/// <summary>Auxiliary image.</summary>
	cv::Mat matImage_Aux;

private:
	/// <summary>Internal variable to store the defined frame rate.</summary>
	int frame_rate;

#ifdef USE_CMU1394
	/// <summary>CMU1394 driver specific camera object.</summary>
	C1394Camera camera;
#endif //USE_CMU1394

private:
	/// <summary>Size in bytes of the image.</summary>
	const int lengthOfBitmap;

	/// <summary>Array to temporarily store the captured image in raw state.</summary>
	unsigned char *pBitmap;
};

#endif //_GRABBER_CMU1394_H_
