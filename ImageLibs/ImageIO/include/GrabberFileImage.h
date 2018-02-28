#ifndef _GRABBER_FILE_IMAGE_H_
#define _GRABBER_FILE_IMAGE_H_

#pragma once

#include "Grabber.h"

/// <summary>Open an image file supported by OpenCV.
/// <para>Supported file types:</para>
/// <para>Windows Bitmap Images (*.bmp; *.dib)</para>
/// <para>JPEG files (*.jpeg; *.jpg; *.jpe)</para>
/// <para>JPEG 2000 images (*.jp2)</para>
/// <para>"Portable Network Graphics (*.png)</para>
/// <para>"Portable image format (*.pbm; *.pgm; *.ppm)</para>
/// <para>"Sun rasters (*.sr; *.ras)</para>
/// <para>"TIFF files (*.tiff; *.tif)</para></summary>
class CGrabberFileImage 
	: public CGrabber  
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. 
	/// <para>Accepts a string with the path to the file that will be opened.</para></summary>
	/// <param name="file_name">Path of image to be opened.</param>
	CGrabberFileImage( std::string file_name = "" );

	/// <summary>Class destructor.</summary>
	~CGrabberFileImage();

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

	/// <summary>Get the last acquired frame into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Image( cv::Mat * mat_ptr );
};

#endif // _GRABBER_FILE_IMAGE_H_
