#ifndef _GRABBER_MODE_H_
#define _GRABBER_MODE_H_

#pragma once

/// <summary>Define a grabber mode from image width, height, frame rate, depth and number of channels.</summary>
class CGrabberMode
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor. Accepts mode parameters</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="framerate">Frame rate (frames per second).</param>
	/// <param name="depth">Depth (bits per pixel).</param>
	/// <param name="channels">Channels (channels per pixel).</param>
	CGrabberMode( int width=0, int height=0, float framerate=0.0f, int depth=8, int channels=3 );

	/// <summary>Copy constructor.</summary>
	/// <param name="copy">Mode to be copied.</param>
	CGrabberMode( const CGrabberMode& copy );

//---------------------------------------------------------------------------
// Data members
protected:
	int 
		/// <summary>Image width (pixels).</summary>
		nWidth,
		/// <summary>Image height (pixels).</summary>
		nHeight,
		/// <summary>Depth (bits per pixel>.</summary>
		nDepth,
		/// <summary>Channels (channels per pixel>.</summary>
		nChannels,
		/// <summary>Image type as used by OpenCV.</summary>
		cvImageType;

	/// <summary>Frame rate (frames per second).</summary>
	float fFrameRate;

//---------------------------------------------------------------------------
// Accessors
public:
	/// <summary>Define grabber mode (explicit).</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="framerate">Frame rate (frames per second).</param>
	/// <param name="depth">Depth (bits per pixel).</param>
	/// <param name="channels">Channels (channels per pixel).</param>
	void set_Mode( int width, int height, float framerate, int depth=8, int channels=3 );

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
	int get_cvImageType( void );


//---------------------------------------------------------------------------
// Operators
public:
	/// <summary>Comparison operator. Compares this mode with a given one.</summary>
	/// <param name="op">Second operand. Mode to compare with.</param>
	/// <returns>true: modes are equal, false: modes are different.</returns>
	bool operator==( CGrabberMode op );

	/// <summary>Attribution operator. Copies second operand (op) into first operand (this).</summary>
	/// <param name="op">Second operand. Mode to copy.</param>
	/// <returns>Current mode after operation.</returns>
	CGrabberMode operator=( const CGrabberMode op );

//---------------------------------------------------------------------------
// Mode comparison
public:
	/// <summary>Explicit comparison (partial: width, height, frame rate).</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="framerate">Frame rate (frames per second).</param>
	/// <returns>true: modes are equal, false: modes are different.</returns>
	bool equal_to( int width, int height, float framerate );

	/// <summary>Explicit comparison.</summary>
	/// <param name="width">Image width (pixels).</param>
	/// <param name="height">Image height (pixels).</param>
	/// <param name="framerate">Frame rate (frames per second).</param>
	/// <param name="depth">Depth (bits per pixel).</param>
	/// <param name="channels">Channels (channels per pixel).</param>
	/// <returns>true: modes are equal, false: modes are different.</returns>
	bool equal_to( int width, int height, float framerate, int depth, int channels );
};

#endif // _GRABBER_MODE_H_
