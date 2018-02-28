#ifndef _GRABBER_H_
#define _GRABBER_H_

#pragma once

#include <string>
#include <opencv2/core/core.hpp>

#include "AttachMutex.hpp"
#include "GrabberControls.h"
#include "GrabberProperties.h"

/// <summary>Class that implements a frame grabber.
/// <para>Using this class to implement different grabbers makes the
/// code transparent to any image source used.</para></summary>
class CGrabber
	: public CGrabberControls
	, public CGrabberProperties
{
//------------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabber( int width = 640, int height = 480 );
	
	/// <summary>Copy constructor.</summary>
	/// <param name="obj">Object to be copied.</param>
	CGrabber( CGrabber &obj );

	/// <summary>Class destructor.</summary>
	virtual ~CGrabber( void );

//------------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Virtual funcition that will serve to implement the specific way 
	/// to initialize each derived grabber.</summary>
	/// <returns>true if grabber was successfully initialized, false if failed.</returns>
	virtual bool OpenGrabber( void ) = 0;

	/// <summary>Virtual funcition that will serve to implement the specific way 
	/// to close each derived grabber.</summary>
	/// <returns>true if grabber was successfully closed, false if failed.</returns>
	virtual bool CloseGrabber( void ) = 0;

public:
	/// <summary>Public funcition that must be called to initialize the grabber.</summary>
	/// <returns>true if grabber was successfully initialized, false if failed.</returns>
	bool Open( void );

	/// <summary>Public funcition that must be called to close the grabber.</summary>
	/// <returns>true if grabber was successfully closed, false if failed.</returns>
	bool Close( void );

//------------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Acquire a new frame.
	/// <para>Pure virtual function that must be implemented in the derived class.</para></summary>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	virtual bool get_Frame( void ) = 0;

	/// <summary>Get the last acquired frame into an image.
	/// <para>Pure virtual function that must be implemented in the derived class.</para></summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	virtual bool get_Image( cv::Mat * mat_ptr ) = 0;

	/// <summary>Aquire a new frame directly into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if a new frame was successfuly acquired, false if it failed.</returns>
	virtual bool get_Frame_toPointer( cv::Mat * mat_ptr ) { return false; };

//---------------------------------------------------------------------------
// Grabber Objects/Parameters
protected:
	CAttachMutex_n<bool> 
		/// <summary>Flag to signal if the grabber was opened successfuly.</summary>	
		isOpen,
		/// <summary>Flag to signal if the grabber has stereo capabilities or not.</summary>	
		bHasStereo;

	/// <summary>Mutex protected image, this object holds the captured image.</summary>	
	CAttachMutex_o< cv::Mat > matImage;

	/// <summary>String that holds a file name when a video or an image is used.</summary>	
	std::string sFileName;

//---------------------------------------------------------------------------
// Accessors for some parameters
public:
	/// <summary>Function that provides an accessor to the presence or not of stereo capabilities.</summary>
	/// <returns>true if the grabber is stereo capable, false otherwise.</returns>
	bool has_Stereo( void );

	/// <summary>Accessor to know if the grabber is initialized or not.</summary>
	/// <returns>true if the grabber is initialized, false otherwise.</returns>
	bool is_GrabberOpen( void );

	/// <summary>Define the path of the file to use.</summary>
	/// <param name="file_name">Path to file.</param>
	void set_FileName( std::string file_name );
};

#endif // _GRABBER_H_
