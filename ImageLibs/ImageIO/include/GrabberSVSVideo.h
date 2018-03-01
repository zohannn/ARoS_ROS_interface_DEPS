#ifndef _GRABBER_SVS_VIDEO_H_
#define _GRABBER_SVS_VIDEO_H_

#pragma once

#include "GrabberSVS.h"

#ifdef USE_SVS
	#define SVS_VIDEO_OBJECT_TYPE svsVideoImages
#else
	#define SVS_VIDEO_OBJECT_TYPE void
#endif

/// <summary>Frame grabber to acquire video from a Videre camera.</summary>
class CGrabberSVSVideo 
	: public CGrabberSVS
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabberSVSVideo( int width = 640, int height = 480 );

	/// <summary>Class destructor.</summary>
	~CGrabberSVSVideo( void );

//---------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Specific implementation of Videre camera initialization.</summary>
	/// <returns>true if grabber was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Specific implementation of Videre camera closing.</summary>
	/// <returns>true if grabber was successfully closed, false if failed.</returns>
	bool CloseGrabber( void );

//---------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Acquire a new frame.</summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	bool get_Frame( void );

//---------------------------------------------------------------------------
// Test if current grab mode is valid
protected:
	/// <summary>Query the grabber if the current used mode is supported.</summary>
	/// <returns>true if the mode is supported, false if it's not.</returns>
	bool ValidMode( CGrabberMode mode );

//---------------------------------------------------------------------------
// Small Vision System's API object
private:
	/// <summary>Videre video object.</summary>
	SVS_VIDEO_OBJECT_TYPE * videoObject;

//---------------------------------------------------------------------------
// Functions that have access to video parameters in the device itself
protected:
	/// <summary>Access the camera and define exposure.</summary>
	/// <param name="exp">Desired exposure value.</param>
	void set_Device_Exposure( int exp );

	/// <summary>Access the camera and define the use of automatic exposure.</summary>
	/// <param name="autoexp">Activate auto exposure. true: on ; false: off.</param>
	void set_Device_Exposure_Auto( bool autoexp );

	/// <summary>Access the camera and define gain.</summary>
	/// <param name="gain">Desired gain value.</param>
	void set_Device_Gain( int gain );

	/// <summary>Access the camera and define the use of automatic gain.</summary>
	/// <param name="autogain">Activate automatic gain. true: on ; false: off.</param>
	void set_Device_Gain_Auto( bool autogain );

	/// <summary>Access the camera and define brightness.</summary>
	/// <param name="bright">Desired brightness value.</param>
	void set_Device_Brightness( int bright );

	/// <summary>Access the camera and define the use of automatic brightness.</summary>
	/// <param name="autobright">Activate auto brightness. true: on ; false: off.</param>
	void set_Device_Brightness_Auto( bool autobright );

	/// <summary>Access the camera and define the red component of white balance.</summary>
	/// <param name="red">Desired red component value of white balance.</param>
	void set_Device_WhiteBalance_Red( int red );

	/// <summary>Access the camera and define the blue component of white balance.</summary>
	/// <param name="blue">Desired blue component value of white balance.</param>
	void set_Device_WhiteBalance_Blue( int blue );

	/// <summary>Access the camera and define the use of automatic white balance control.</summary>
	/// <param name="autobalance">Activate automatic white balance. true: on ; false: off.</param>
	void set_Device_WhiteBalance_Auto( bool autobalance );
	
	/// <summary>Access the camera and define gamma.</summary>
	/// <param name="gamma">Desired gamma value.</param>
	void set_Device_Gamma( int gamma );

	/// <summary>Access the camera and define saturation.</summary>
	/// <param name="sat">Desired saturation value.</param>
	void set_Device_Saturation( int sat );

	/// <summary>Access the camera and get current exposure value.</summary>
	/// <returns>Exposure value.</returns>
	int get_Device_Exposure( void );

	/// <summary>Access the camera and get auto exposure state.</summary>
	/// <returns>State for auto exposure. true: on ; false: off.</returns>
	bool get_Device_Exposure_Auto( void );

	/// <summary>Access the camera and get current gain value.</summary>
	/// <returns>Gain value.</returns>
	int get_Device_Gain( void );

	/// <summary>Access the camera and get auto gain state.</summary>
	/// <returns>State for auto gain. true: on ; false: off.</returns>
	bool get_Device_Gain_Auto( void );

	/// <summary>Access the camera and get current brightness value.</summary>
	/// <returns>Brightness value.</returns>
	int get_Device_Brightness( void );

	/// <summary>Access the camera and get auto brightness state.</summary>
	/// <returns>State for auto brightness. true: on ; false: off.</returns>
	bool get_Device_Brightness_Auto( void );

	/// <summary>Access the camera and get current red component of white balance.</summary>
	/// <returns>Red component of white balance.</returns>
	int get_Device_WhiteBalance_Red( void );

	/// <summary>Access the camera and get current blue component of white balance.</summary>
	/// <returns>Blue component of white balance.</returns>
	int get_Device_WhiteBalance_Blue( void );

	/// <summary>Access the camera and get auto white balance state.</summary>
	/// <returns>State for auto white balance. true: on ; false: off.</returns>
	bool get_Device_WhiteBalance_Auto( void );

	/// <summary>Access the camera and get current gamma value.</summary>
	/// <returns>Gamma value.</returns>
	int get_Device_Gamma( void );

	/// <summary>Access the camera and get current saturation value.</summary>
	/// <returns>Saturation value.</returns>
	int get_Device_Saturation( void );
};

#endif // _GRABBER_SVS_VIDEO_H_
