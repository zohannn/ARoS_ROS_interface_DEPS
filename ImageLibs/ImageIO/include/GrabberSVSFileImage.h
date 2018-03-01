#ifndef _GRABBER_SVS_FILE_IMAGE_H_
#define _GRABBER_SVS_FILE_IMAGE_H_

#pragma once

#include "GrabberSVS.h"

#ifdef USE_SVS
	#define SVS_IMAGE_OBJECT_TYPE svsFileImages
#else
	#define SVS_IMAGE_OBJECT_TYPE int
#endif

/// <summary>Frame grabber to use image pairs taken from a Videre camera.</summary>
class CGrabberSVSFileImage 
	: public CGrabberSVS
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. 
	/// <para>Accepts a string with the path to the file that will be opened.</para></summary>
	/// <param name="file_name">Path of image to be opened.</param>
	CGrabberSVSFileImage( std::string file_name = "" );

	/// <summary>Class destructor.</summary>
	~CGrabberSVSFileImage( void );

//---------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Open the given image file.</summary>
	/// <returns>true if the file was successfully opened, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Close the image file.</summary>
	/// <returns>true if the file was successfully closed, false if failed.</returns>
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
// Small Vision System's API objects
private:
	/// <summary>Videre image object.</summary>
	SVS_IMAGE_OBJECT_TYPE imageObject;
};

#endif // _GRABBER_SVS_FILE_IMAGE_H_
