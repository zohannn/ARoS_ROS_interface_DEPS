#ifndef _GRABBER_PROPERTIES_H_
#define _GRABBER_PROPERTIES_H_

#pragma once

#include "AttachMutex.hpp"
#include "GrabberMode.h"

/// <summary>Class that attempts to define all parameters of the grabber:
/// width, height, frame rate, image depth, channels.
/// <para>Using this class will help to define what modes each grabber supports.</para></summary>
class CGrabberProperties
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor.</summary>
	CGrabberProperties( void );

	/// <summary>Constructor with image width and height.</summary>
	CGrabberProperties( int width, int height );

	/// <summary>Copy constructor.</summary>
	CGrabberProperties( CGrabberProperties &obj );

//---------------------------------------------------------------------------
// Data members
protected:
	CAttachMutex_n<int>
		/// <summary>Image width (pixels).</summary>
		Width,
		/// <summary>Image height (pixels).</summary>
		Height,
		/// <summary>Depth (bits per pixel>.</summary>
		Depth,
		/// <summary>Channels (channels per pixel>.</summary>
		Channels;

	CAttachMutex_n<float>
		/// <summary>Frame rate (frames per second).</summary>
		FrameRate;

	CAttachMutex_n<bool>
		/// <summary>Flag that signals if the current defined parameters are valid.</summary>
		Valid;

	/// <summary>Grabber mode (width, height, depth, channels, frame rate).</summary>
	CGrabberMode Mode;

//---------------------------------------------------------------------------
// Test if 'mode' is supported
protected:
	/// <summary>Test if given mode is valid.</summary>
	/// <param name="mode">Grabber mode to test.</param>
	/// <returns>true if given mode is valid, false if invalid.</returns>
	virtual bool ValidMode( CGrabberMode mode );

//---------------------------------------------------------------------------
// Accessors
public:
	/// <summary>Define grabber mode.</summary>
	/// <param name="mode">Grabber mode to define.</param>
	void set_GrabberMode( CGrabberMode mode );

	/// <summary>Define grabber mode (explicit).</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="framerate">Frame rate (frames per second).</param>
	/// <param name="depth">Depth (bits per pixel).</param>
	/// <param name="channels">Channels (channels per pixel).</param>
	void set_GrabberMode( int width, int height, float framerate, int depth=8, int channels=3 );

	/// <summary>Access image width.</summary>
	/// <returns>Image width.</returns>
	int get_Width( void );

	/// <summary>Access image height.</summary>
	/// <returns>Image height.</returns>
	int get_Height( void );

	/// <summary>Access frame rate.</summary>
	/// <returns>Frame rate.</returns>
	float get_FrameRate( void );
	
	/// <summary>Access image depth.</summary>
	/// <returns>Image depth.</returns>
	int get_Depth( void );

	/// <summary>Access number of channels.</summary>
	/// <returns>Number of channels.</returns>
	int get_Channels( void );

	/// <summary>Access image type as used by OpenCV.</summary>
	/// <returns>Image type.</returns>
	int get_CVImageType( void );

	/// <summary>Is this mode valid?.</summary>
	/// <returns>true: mode is valid, false: mode is invalid.</returns>
	bool is_ModeValid( void );
};

#endif // _GRABBER_PROPERTIES_H_
