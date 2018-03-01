#ifndef _GRABBER_SVS_H_
#define _GRABBER_SVS_H_

#pragma once

#include "Grabber.h"

#ifdef USE_SVS
	#include <svsclass.h>
	#define STEREO_IMAGE_TYPE svsStereoImage
	#define STEREO_IMAGE_OBJECT CAttachMutex_t<STEREO_IMAGE_TYPE>
#else
	#define STEREO_IMAGE_TYPE int
	#define STEREO_IMAGE_OBJECT CAttachMutex_n<STEREO_IMAGE_TYPE>
#endif

/// <summary>Frame grabber to use Videre API based image objects.</summary>
class CGrabberSVS :
	public CGrabber
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the image size.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CGrabberSVS( int width = 640, int height = 480 );

	/// <summary>Class destructor.</summary>
	~CGrabberSVS( void );

//---------------------------------------------------------------------------
// Frame grabbing
public:
	/// <summary>Get the last acquired frame into an image.</summary>
	/// <param name="mat_ptr">Pointer to location where to store the image.</param>
	/// <returns>true if the operation was successfull, false if it failed.</returns>
	bool get_Image( cv::Mat * mat_ptr );

//---------------------------------------------------------------------------
// Functions and objects related to stereo
public:
	/// <summary>Get a pointer to the SVS image.
	/// <para>Must be implemented in the derived class.</para></summary>
	/// <param name="svs_ptr">Object that will point to the SVS image.</param>
	virtual void get_SVSImage_Pointer( STEREO_IMAGE_TYPE * svs_ptr );

	/// <summary>Copy the current SVS image into another object.
	/// <para>Must be implemented in the derived class.</para></summary>
	/// <param name="svs_ptr">Object that will point to the location where the image will be saved.</param>
	virtual void get_SVSImage_Copy( STEREO_IMAGE_TYPE * svs_ptr );

	/// <summary>The Stereo Image object.</summary>
	STEREO_IMAGE_OBJECT stereo_image;

//---------------------------------------------------------------------------
// Auxiliary objects and functions to handle them
protected:
	/// <summary>Create an auxiliary image to be used in data extraction.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	void CreateAuxImage( int width, int height );

	/// <summary>Destroy the auxiliary image.</summary>
	void DestroyAuxImage( void );

	/// <summary>Auxiliary image.</summary>
	cv::Mat	matImage_Aux;

	/// <summary>Object that points to the original buffer owned by the auxiliary image.</summary>
	unsigned char * Original_aux_buffer;
};

#endif // _GRABBER_SVS_H_
