#ifndef _IMAGE_STREAM_H_
#define _IMAGE_STREAM_H_

#pragma once

#include <string>

#include <opencv2/core/core.hpp>

#include <yarp/os/Port.h>
#include <yarp/os/RateThread.h>
#include <yarp/sig/Image.h>

#include "AttachMutex.hpp"

/// <summary>Stream images using a YARP port.</summary>
class CImageStream 
	: public yarp::os::RateThread  
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts the port name and image size.</summary>
	/// <param name="port_name">Port name that the streamer will use.</param>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	CImageStream( std::string port_name, int width = 640, int height = 480 );

	/// <summary>Class destructor.</summary>
	~CImageStream( void );

//---------------------------------------------------------------------------
// Thread implementation
public:
	/// <summary>Initialization tasks that need to be done by the thread itself.
	/// <para>If this function fails, the thread does not start.</para></summary>
	/// <returns>true if it succeeds, false if it failse.</returns>
	bool threadInit( void );

	/// <summary>Main body of the thread.</summary>
	void run( void );
	
	/// <summary>Release method. The thread executes this function once when it exits.</summary>
	void threadRelease( void );
	
	/// <summary>Called just before the thread starts.</summary>
	void beforeStart( void );

//---------------------------------------------------------------------------
// Accessors
public:
	// Image accessors

	/// <summary>Copy image to the streamer, as soon as it is possible this image will be streamed.
	/// <para>This function makes verification of size and makes adjustments if necessary.</para></summary>
	/// <param name="mat_ptr">Pointer to the image.</param>
	void copyImage( cv::Mat * mat_ptr );
	
	/// <summary>Copy region of an image to the streamer, as soon as it is possible this image will be streamed.
	/// <para>This function makes verification of size and makes adjustments if necessary.</para></summary>
	/// <param name="mat_ptr">Pointer to the image.</param>
	/// <param name="region">Rectangle defining region on the image.</param>
	void copyImage( cv::Mat * mat_ptr, cv::Rect region );

	/// <summary>Direct copy of an image to the streamer, as soon as it is possible this image will be streamed.</summary>
	/// <param name="mat_ptr">Pointer to the image.</param>
	void copyImage_direct( cv::Mat * mat_ptr );

	/// <summary>Define the port name that the streamer will use.</summary>
	/// <param name="port_name">Port name.</param>
	void set_PortName( std::string port_name );
	
	/// <summary>Get the port name that the streamer is using.</summary>
	/// <returns>Port name.</returns>
	std::string get_PortName( void );
	
	/// <summary>Define image resolution.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	void set_Resolution( int width, int height );
	
//---------------------------------------------------------------------------
// Data members
protected:
	CAttachMutex_n<int>
		/// <summary>Image width (pixels).</summary>
		Width, 
		/// <summary>Image height (pixels).</summary>
		Height;

	/// <summary>Semaphore protected OpenCV Image.</summary>
	CAttachMutex_o< cv::Mat > matImage;

	/// <summary>Semaphore protected YARP Image.</summary>
	CAttachMutex_o< yarp::sig::ImageOf<yarp::sig::PixelBgr> > yarp_image;
	
	/// <summary>Semaphore protected YARP port.</summary>
	CAttachMutex_t< yarp::os::Port > outPort;
	
	/// <summary>The port name.</summary>
	std::string sPort_Name;
};

#endif // _IMAGE_STREAM_H_
