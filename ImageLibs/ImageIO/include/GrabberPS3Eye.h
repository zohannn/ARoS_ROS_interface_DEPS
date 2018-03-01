#ifndef _GRABBER_PS3EYE_H_
#define _GRABBER_PS3EYE_H_

#pragma once

#include "Grabber.h"

#ifdef USE_CLEye
	#include <CLEyeMulticam.h>
	#define PS3EYE_CAM_PARAMETER	CLEyeCameraParameter
	#define PS3EYE_CAM_INSTANCE		CLEyeCameraInstance
	#define PS3EYE_CAM_COLORMODE	CLEyeCameraColorMode
	#define PS3EYE_CAM_RESOLUTION	CLEyeCameraResolution
#else
	#define PS3EYE_CAM_PARAMETER	int
	#define PS3EYE_CAM_INSTANCE		void*
	#define PS3EYE_CAM_COLORMODE	int
	#define PS3EYE_CAM_RESOLUTION	int
#endif

/// <summary>Frame grabber to use the PS3 Eye camera.</summary>
class CGrabberPS3Eye 
	: public CGrabber
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabberPS3Eye( int width = 640, int height = 480, bool colour = true );

	/// <summary>Class destructor.</summary>
	~CGrabberPS3Eye( void );

//---------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Initialize camera.</summary>
	/// <returns>true if the camera was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Close the camera interface.</summary>
	/// <returns>true if the camera was successfully closed, false if failed.</returns>
	bool CloseGrabber( void );

//---------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Acquire a new frame.</summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	bool get_Frame( void );

	/// <summary>Acquire a new frame directly into an image pointer.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Frame_toPointer( cv::Mat * mat_ptr );

	/// <summary>Get the last acquired frame into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Image( cv::Mat * mat_ptr );

//---------------------------------------------------------------------------
// Test if current grab mode is valid
protected:
	/// <summary>Query the grabber if the current used mode is supported.</summary>
	/// <returns>true if the mode is supported, false if it's not.</returns>
	bool ValidMode( CGrabberMode mode );

//---------------------------------------------------------------------------
// CL-Eye SDK objects
private:
	/// <summary>Camera instance.</summary>
	PS3EYE_CAM_INSTANCE		ps3eye_cam;
	
	/// <summary>Colour mode.</summary>
	PS3EYE_CAM_COLORMODE	ps3eye_mode;

	/// <summary>Camera resolution.</summary>
	PS3EYE_CAM_RESOLUTION	ps3eye_resolution;

//---------------------------------------------------------------------------
protected:
	/// <summary>Load default video parameters for the camera.</summary>
	void LoadDefaultVideoParameters( void );

//---------------------------------------------------------------------------
public:
	/// <summary>PS3 Eye camera Red LED control.</summary>
	/// <param name="on">Led state. true: on, false: off.</param>
	/// <returns>true if the operation was successfuly performed, false if an error ocurred.</returns>
	bool set_CameraLED( bool on );

//---------------------------------------------------------------------------
// Accessors to camera parameters
protected:
	/// <summary>Normalize a parameter.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <param name="value">Raw parameter value.</param>
	/// <returns>Normalized parameter.</returns>
	int NormalizeVideoParameter( PS3EYE_CAM_PARAMETER param, int value );
	
	/// <summary>Transform a normalized parameter in a range adapted to device.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <param name="value">Normalized parameter value.</param>
	/// <returns>Raw parameter.</returns>
	int FitVideoParameterToDevice( PS3EYE_CAM_PARAMETER param, int value );

	/// <summary>Set a normalized parameter in the camera.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <param name="value">Parameter value.</param>
	void set_Camera_Video_Parameter( PS3EYE_CAM_PARAMETER param, int value );

	/// <summary>Get a normalized parameter from the camera.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <returns>Parameter value.</returns>
	int get_Camera_Video_Parameter( PS3EYE_CAM_PARAMETER param );

//---------------------------------------------------------------------------
// Functions that have access to video parameters in the device itself
private:
	/// <summary>Get a parameter from the camera.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <returns>Parameter value.</returns>
	int get_Camera_Parameter( PS3EYE_CAM_PARAMETER par );

	/// <summary>Set a parameter in the camera.</summary>
	/// <param name="param">Parameter ID.</param>
	/// <param name="val">Parameter value.</param>
	void set_Camera_Parameter( PS3EYE_CAM_PARAMETER par, int val );

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

	/// <summary>Access the camera and define the red component of white balance.</summary>
	/// <param name="red">Desired red component value of white balance.</param>
	void set_Device_WhiteBalance_Red( int red );

	/// <summary>Access the camera and define the green component of white balance.</summary>
	/// <param name="green">Desired green component value of white balance.</param>
	void set_Device_WhiteBalance_Green( int green );

	/// <summary>Access the camera and define the blue component of white balance.</summary>
	/// <param name="blue">Desired blue component value of white balance.</param>
	void set_Device_WhiteBalance_Blue( int blue );

	/// <summary>Access the camera and define the use of automatic white balance control.</summary>
	/// <param name="autobalance">Activate automatic white balance. true: on ; false: off.</param>
	void set_Device_WhiteBalance_Auto( bool autobalance );

	/// <summary>Access the camera and define the horizontal flipping of the image.</summary>
	/// <param name="autobalance">Activate horizontal flipping. true: on ; false: off.</param>
	void set_Device_FlipHorizontal( bool flip );

	/// <summary>Access the camera and define the vertical flipping of the image.</summary>
	/// <param name="autobalance">Activate vertical flipping. true: on ; false: off.</param>
	void set_Device_FlipVertical( bool flip );

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

	/// <summary>Access the camera and get current red component of white balance.</summary>
	/// <returns>Red component of white balance.</returns>
	int get_Device_WhiteBalance_Red( void );

	/// <summary>Access the camera and get current green component of white balance.</summary>
	/// <returns>Green component of white balance.</returns>
	int get_Device_WhiteBalance_Green( void );

	/// <summary>Access the camera and get current blue component of white balance.</summary>
	/// <returns>Blue component of white balance.</returns>
	int get_Device_WhiteBalance_Blue( void );

	/// <summary>Access the camera and get auto white balance state.</summary>
	/// <returns>State for auto white balance. true: on ; false: off.</returns>
	bool get_Device_WhiteBalance_Auto( void );

	/// <summary>Access the camera and get horizontal image flipping state.</summary>
	/// <returns>State for horizontal flipping. true: on ; false: off.</returns>
	bool get_Device_FlipHorizontal( void );

	/// <summary>Access the camera and get vertical image flipping state.</summary>
	/// <returns>State for vertical flipping. true: on ; false: off.</returns>
	bool get_Device_FlipVertical( void );
};

#endif // _GRABBER_PS3EYE_H_
