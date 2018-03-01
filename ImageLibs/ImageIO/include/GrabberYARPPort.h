#ifndef _GRABBER_YARP_PORT_H_
#define _GRABBER_YARP_PORT_H_

#pragma once

#include "Grabber.h"

#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>

/// <summary>Frame grabber to acquire images from a remote YARP port.</summary>
class CGrabberYARPPort 
	: public CGrabber
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor.</summary>
	CGrabberYARPPort( void );

	/// <summary>Class destructor.</summary>
	~CGrabberYARPPort( void );

//---------------------------------------------------------------------------
// Open/Close grabber
protected:
	/// <summary>Open the ports and initialize capture.</summary>
	/// <returns>true if the capture was successfully initialized, false if failed.</returns>
	bool OpenGrabber( void );

	/// <summary>Stop the capture and close the ports.</summary>
	/// <returns>true if the capture was successfully closed, false if failed.</returns>
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

//---------------------------------------------------------------------------
// Accessors for Local and Remote port names
public:
	/// <summary>Define the port name that the grabber will use.</summary>
	/// <param name="name">Port name.</param>
	void set_PortNameLocal( std::string name );

	/// <summary>Define the remote port name where the grabber will connect to.</summary>
	/// <param name="name">Port name.</param>
	void set_PortNameRemote( std::string name );

	/// <summary>Get the port name that the grabber is using.</summary>
	/// <returns>Port name.</returns>
	std::string get_PortNameLocal( void );

	/// <summary>Get the remote port name where the grabber is connecting.</summary>
	/// <returns>Port name.</returns>
	std::string get_PortNameRemote( void );

//---------------------------------------------------------------------------
// Data members
private:
	/// <summary>Buffered port used by the grabber.</summary>
	yarp::os::BufferedPort< yarp::sig::ImageOf<yarp::sig::PixelRgb> > yPort_in;

	std::string
		/// <summary>Port name that the grabber will use.</summary>
		strPortName_Local,
		/// <summary>Remote port name where the grabber will connect to.</summary>
		strPortName_Remote;
};

#endif // _GRABBER_YARP_PORT_H_
