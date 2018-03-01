#ifndef _GRABBER_OPENCV_CAPTURE_H_
#define _GRABBER_OPENCV_CAPTURE_H_

#pragma once

#include "Grabber.h"

#include <opencv2/highgui/highgui.hpp>

/// <summary>Class that wrapps the OpenCV Video capture object into the
/// CGrabber structure.
/// <para>This is a base class that will be derived into two new classes
/// one to handle video files and other to handle cameras.</para></summary>
class CGrabberOpenCVCapture 
	: public CGrabber
	, protected cv::VideoCapture
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabberOpenCVCapture( int width = 640, int height = 480 );

	/// <summary>Class destructor.</summary>
	~CGrabberOpenCVCapture( void );

//---------------------------------------------------------------------------
// Close grabber
protected:
	/// <summary>Close the current capture.</summary>
	/// <returns>true if the file was successfully closed, false if failed.</returns>
	bool CloseGrabber( void );

//---------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Acquire a new frame.
	/// <para>This method must be reimplemented in the derived classes to handle video or cameras.</para></summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	virtual bool get_Frame( void );

	/// <summary>Get the last acquired frame into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Image( cv::Mat * mat_ptr );

//---------------------------------------------------------------------------
protected:
	/// <summary>Set a normalized parameter in the capture.</summary>
	/// <param name="param">ID of the parameter.</param>
	/// <param name="value">Desired value for the parameter.</param>
	void set_Camera_Video_Parameter( int param, int value );

	/// <summary>Get a normalized parameter from the capture.</summary>
	/// <param name="param">ID of the parameter.</param>
	/// <returns>Parameter current value.</returns>
	int get_Camera_Video_Parameter( int param );

//---------------------------------------------------------------------------
// Functions that have access to video parameters in the device itself
protected:
	/// <summary>Access the capture object and define exposure.</summary>
	/// <param name="exp">Desired exposure value.</param>
	void set_Device_Exposure( int exp );

	/// <summary>Access the capture object and define the use of auto exposure.</summary>
	/// <param name="autoexp">Activate auto exposure. true: on ; false: off.</param>
	void set_Device_Exposure_Auto( bool autoexp );

	/// <summary>Access the capture object and define gain.</summary>
	/// <param name="gain">Desired gain value.</param>
	void set_Device_Gain( int gain );

	/// <summary>Access the capture object and define brightness.</summary>
	/// <param name="bright">Desired brightness value.</param>
	void set_Device_Brightness( int bright );

	/// <summary>Access the capture object and define saturation.</summary>
	/// <param name="sat">Desired saturation value.</param>
	void set_Device_Saturation( int sat );

	/// <summary>Access the capture object and define the red component of white balance.</summary>
	/// <param name="red">Desired red component value of white balance.</param>
	void set_Device_WhiteBalance_Red( int red );

	/// <summary>Access the capture object and define the blue component of white balance.</summary>
	/// <param name="blue">Desired red component value of white balance.</param>
	void set_Device_WhiteBalance_Blue( int blue );

	/// <summary>Access the capture and get current exposure value.</summary>
	/// <returns>Exposure value.</returns>
	int get_Device_Exposure( void );

	/// <summary>Access the capture and get auto exposure state.</summary>
	/// <returns>State for auto exposure. true: on ; false: off.</returns>
	bool get_Device_Exposure_Auto( void );

	/// <summary>Access the capture and get current gain value.</summary>
	/// <returns>Gain value.</returns>
	int get_Device_Gain( void );

	/// <summary>Access the capture and get current brightness value.</summary>
	/// <returns>Brightness value.</returns>
	int get_Device_Brightness( void );

	/// <summary>Access the capture and get current saturation value.</summary>
	/// <returns>Saturation value.</returns>
	int get_Device_Saturation( void );

	/// <summary>Access the capture and get current red component of white balance.</summary>
	/// <returns>Red component of white balance.</returns>
	int get_Device_WhiteBalance_Red( void );

	/// <summary>Access the capture and get current blue component of white balance.</summary>
	/// <returns>Blue component of white balance.</returns>
	int get_Device_WhiteBalance_Blue( void );
};

#endif // _GRABBER_OPENCV_CAPTURE_H_
