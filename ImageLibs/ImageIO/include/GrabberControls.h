#ifndef _GRABBER_CONTROLS_H_
#define _GRABBER_CONTROLS_H_

#pragma once

#include "AttachMutex.hpp"
#include "ConfigFile.h"

/// <summary>CGrabberControls offers an abstraction layer for common controls
/// for different grabbers (exposure, gain, brightness, white balance).</summary>
class CGrabberControls 
	: public CConfigFile
{
public:
	/// <summary>Default constructor.</summary>
	CGrabberControls( void );

	/// <summary>Copy constructor.</summary>
	/// <param name="obj">Object to be copied.</param>
	CGrabberControls( const CGrabberControls &obj );

//---------------------------------------------------------------------------
// Grabber Control Parameters
protected:
	CAttachMutex_n<int> 
		/// <summary>Mutex protected variable that holds the exposure value.</summary>
		nExposure,
		/// <summary>Mutex protected variable that holds the gain value.</summary>
		nGain,
		/// <summary>Mutex protected variable that holds the red component of white balance.</summary>
		nRed,
		/// <summary>Mutex protected variable that holds the green component of white balance.</summary>
		nGreen,
		/// <summary>Mutex protected variable that holds the blue component of white balance.</summary>
		nBlue,
		/// <summary>Mutex protected variable that holds the brightness value.</summary>
		nBrightness,
		/// <summary>Mutex protected variable that holds the saturation value.</summary>
		nSaturation,
		/// <summary>Mutex protected variable that holds the gamma value.</summary>
		nGamma;

	CAttachMutex_n<bool> 
		/// <summary>Flag that signals if the grabber has auto exposure control.</summary>
		bAutoExposure,
		/// <summary>Flag that signals if the grabber is using auto gain control.</summary>
		bAutoGain,
		/// <summary>Flag that signals if the grabber is using auto white balance control.</summary>
		bWhiteBalanceAuto,
		/// <summary>Flag that signals if the grabber is using auto brightness control.</summary>
		bAutoBrightness,
		/// <summary>Flag that signals if the grabber has the image horizontally flipped.</summary>
		bFlipHorizontal,
		/// <summary>Flag that signals if the grabber has the image vertically flipped.</summary>
		bFlipVertical,
		/// <summary>Flag that signals if the grabber has usable control parameters.</summary>
		bControlParameters,
		/// <summary>Flag that signals if the grabber has exposure control.</summary>
		bControl_Exposure,
		/// <summary>Flag that signals if the grabber has gain control.</summary>
		bControl_Gain,
		/// <summary>Flag that signals if the grabber has brightness control.</summary>
		bControl_Brightness,
		/// <summary>Flag that signals if the grabber has gamma control.</summary>
		bControl_Gamma,
		/// <summary>Flag that signals if the grabber has saturation.</summary>
		bControl_Saturation,
		/// <summary>Flag that signals if the grabber has white balance control.</summary>
		bControl_WhiteBalance,
		/// <summary>Flag that signals if the grabber has control over the red component of white balance.</summary>
		bControl_WB_Red,
		/// <summary>Flag that signals if the grabber has control over the green component of white balance.</summary>
		bControl_WB_Green,
		/// <summary>Flag that signals if the grabber has control over the blue component of white balance.</summary>
		bControl_WB_Blue,
		/// <summary>Flag that signals if the grabber has the capability to horizontally flip the image.</summary>
		bControl_Flip_Horizontal,
		/// <summary>Flag that signals if the grabber has the capability to vertically flip the image.</summary>
		bControl_Flip_Vertical;

public:
//---------------------------------------------------------------------------
	// Video control parameters

	/// <summary>Manually define exposure value.</summary>
	/// <param name="exp">Exposure value.</param>
	void set_Exposure( int exp );

	/// <summary>Define if the grabber will use automatic control of exposure.</summary>
	/// <param name="autoexp">true - on, false - off.</param>
	void set_Exposure_Auto( bool autoexp );

	/// <summary>Manually define gain value.</summary>
	/// <param name="gain">Gain value.</param>
	void set_Gain(  int gain );

	/// <summary>Define if the grabber will use automatic control of gain.</summary>
	/// <param name="autogain">true - on, false - off.</param>
	void set_Gain_Auto( bool autogain );

	/// <summary>Manually define brightness value.</summary>
	/// <param name="bright">Brightness value.</param>
	void set_Brightness( int bright );

	/// <summary>Define if the grabber will use automatic control of brightness.</summary>
	/// <param name="autobright">true - on, false - off.</param>
	void set_Brightness_Auto( bool autobright );

	/// <summary>Manually define the red component of white balance.</summary>
	/// <param name="red">Component value.</param>
	void set_WhiteBalance_Red( int red );

	/// <summary>Manually define the green component of white balance.</summary>
	/// <param name="green">Component value.</param>
	void set_WhiteBalance_Green( int green );

	/// <summary>Manually define the blue component of white balance.</summary>
	/// <param name="blue">Component value.</param>
	void set_WhiteBalance_Blue( int blue );

	/// <summary>Define if the grabber will use automatic control of white balance.</summary>
	/// <param name="autobalance">true - on, false - off.</param>
	void set_WhiteBalance_Auto( bool autobalance );
	
	/// <summary>Manually define the gamma value.</summary>
	/// <param name="gamma">Gamma value.</param>
	void set_Gamma( int gamma );

	/// <summary>Manually define the colour saturation value.</summary>
	/// <param name="sat">Saturation value.</param>
	void set_Saturation( int sat );

	/// <summary>Manually define horizontal image flipping.</summary>
	/// <param name="flip">true - flip, false, do not flip.</param>
	void set_FlipHorizontal( bool flip );

	/// <summary>Manually define vertical image flipping.</summary>
	/// <param name="flip">true - flip, false, do not flip.</param>
	void set_FlipVertical( bool flip );

	/// <summary>Access to the current exposure value.</summary>
	/// <returns>Exposure value.</returns>
	int get_Exposure( void );

	/// <summary>Access to the status of the exposure automatic control.</summary>
	/// <returns>Automatic control enabled - true: on, false: off.</returns>
	bool get_Exposure_Auto( void );

	/// <summary>Access to the current gain value.</summary>
	/// <returns>Gain value.</returns>
	int get_Gain( void );

	/// <summary>Access to the status of the gain automatic control.</summary>
	/// <returns>Automatic control enabled - true: on, false: off.</returns>
	bool get_Gain_Auto( void );

	/// <summary>Access to the current brightness value.</summary>
	/// <returns>Brightness value.</returns>
	int get_Brightness( void );

	/// <summary>Access to the status of the brightness automatic control.</summary>
	/// <returns>Automatic control enabled - true: on, false: off.</returns>
	bool get_Brightness_Auto( void );

	/// <summary>Access to the current red component of white balance.</summary>
	/// <returns>Red component of white balance.</returns>
	int get_WhiteBalance_Red( void );

	/// <summary>Access to the current gree component of white balance.</summary>
	/// <returns>Green component of white balance.</returns>
	int get_WhiteBalance_Green( void );

	/// <summary>Access to the current blue component of white balance.</summary>
	/// <returns>Blue component of white balance.</returns>
	int get_WhiteBalance_Blue( void );

	/// <summary>Access to the status of the white balance automatic control.</summary>
	/// <returns>Automatic control enabled - true: on, false: off.</returns>
	bool get_WhiteBalance_Auto( void );

	/// <summary>Access to the current gamma value.</summary>
	/// <returns>Gamma value.</returns>
	int get_Gamma( void );

	/// <summary>Access to the current saturation value.</summary>
	/// <returns>Saturation value.</returns>
	int get_Saturation( void );

	/// <summary>Access to the status of the horizontal image flipping.</summary>
	/// <returns>Image flipping enabled - true: on, false: off.</returns>
	bool get_FlipHorizontal( void );

	/// <summary>Access to the status of the vertical image flipping.</summary>
	/// <returns>Image flipping enabled - true: on, false: off.</returns>
	bool get_FlipVertical( void );

//---------------------------------------------------------------------------
// Parameter control info
public:
	/// <summary>Informs if the current grabber has implemented parameter control.</summary>
	/// <returns>Parameter control availability. true: available, false: not available.</returns>
	bool has_ParamaterControl( void );

	/// <summary>Informs if the current grabber has implemented exposure control.</summary>
	/// <returns>Exposure control availability. true: available, false: not available.</returns>
	bool has_Control_Exposure( void );

	/// <summary>Informs if the current grabber has implemented gain control.</summary>
	/// <returns>Gain control availability. true: available, false: not available.</returns>
	bool has_Control_Gain( void );

	/// <summary>Informs if the current grabber has implemented brightness control.</summary>
	/// <returns>Brightness control availability. true: available, false: not available.</returns>
	bool has_Control_Brightness( void );

	/// <summary>Informs if the current grabber has implemented gamma control.</summary>
	/// <returns>Gamma control availability. true: available, false: not available.</returns>
	bool has_Control_Gamma( void );

	/// <summary>Informs if the current grabber has implemented saturation control.</summary>
	/// <returns>Saturation control availability. true: available, false: not available.</returns>
	bool has_Control_Saturation( void );

	/// <summary>Informs if the current grabber has implemented white balance control.</summary>
	/// <returns>White balance control availability. true: available, false: not available.</returns>
	bool has_Control_WhiteBalance( void );

	/// <summary>Informs if the current grabber has implemented individual control 
	/// of red component of white balance.</summary>
	/// <returns>Red component of white balance control availability.
	/// <para>true: available, false: not available.</para></returns>
	bool has_Control_WB_Red( void );

	/// <summary>Informs if the current grabber has implemented individual control 
	/// of green component of white balance.</summary>
	/// <returns>Green component of white balance control availability.
	/// <para>true: available, false: not available.</para></returns>
	bool has_Control_WB_Green( void );

	/// <summary>Informs if the current grabber has implemented individual control 
	/// of blue component of white balance.</summary>
	/// <returns>Blue component of white balance control availability.
	/// <para>true: available, false: not available.</para></returns>
	bool has_Control_WB_Blue( void );

	/// <summary>Informs if the current grabber has implemented horizontal image flip control.</summary>
	/// <returns>Horizontal image flip control availability. true: available, false: not available.</returns>
	bool has_Control_FlipHorizontal( void );

	/// <summary>Informs if the current grabber has implemented vertical image flip control.</summary>
	/// <returns>Vertical image flip control availability. true: available, false: not available.</returns>
	bool has_Control_FlipVertical( void );

//---------------------------------------------------------------------------
// These functions must be implemented in each grabber 
// because they are supposed to be specific to each device

protected:
	/// <summary>Virtual function to access the hardware and set exposure.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="exp">Desired exposure value.</param>
	virtual void set_Device_Exposure( int exp ) {};

	/// <summary>Virtual function to access the hardware and set auto exposure.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="autoexp">Activate auto exposure. true: on ; false: off.</param>
	virtual void set_Device_Exposure_Auto( bool autoexp ) {};

	/// <summary>Virtual function to access the hardware and set gain.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="gain">Desired gain value.</param>
	virtual void set_Device_Gain(  int gain ) {};

	/// <summary>Virtual function to access the hardware and set auto gain.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="autogain">Activate auto gain. true: on ; false: off.</param>
	virtual void set_Device_Gain_Auto( bool autogain ) {};

	/// <summary>Virtual function to access the hardware and set brightness.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="bright">Desired brightness value.</param>
	virtual void set_Device_Brightness( int bright ) {};

	/// <summary>Virtual function to access the hardware and set auto brightness.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="autobright">Activate auto brightness. true: on ; false: off.</param>
	virtual void set_Device_Brightness_Auto( bool autobright ) {};

	/// <summary>Virtual function to access the hardware and set red component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="red">Desired red component of white balance.</param>
	virtual void set_Device_WhiteBalance_Red( int red ) {};

	/// <summary>Virtual function to access the hardware and set green component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="green">Desired green component of white balance.</param>
	virtual void set_Device_WhiteBalance_Green( int green ) {};

	/// <summary>Virtual function to access the hardware and set blue component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="blue">Desired blue component of white balance.</param>
	virtual void set_Device_WhiteBalance_Blue( int blue ) {};

	/// <summary>Virtual function to access the hardware and set auto white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="autobalance">Activate auto white balance. true: on ; false: off.</param>
	virtual void set_Device_WhiteBalance_Auto( bool autobalance ) {};
	
	/// <summary>Virtual function to access the hardware and set gamma.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="gamma">Desired gamma value.</param>
	virtual void set_Device_Gamma( int gamma ) {};

	/// <summary>Virtual function to access the hardware and set saturation.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="sat">Desired saturation value.</param>
	virtual void set_Device_Saturation( int sat ) {};

	/// <summary>Virtual function to access the hardware and set horizontal image flipping.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="flip">Activate horizontal image flipping. true: on ; false: off.</param>
	virtual void set_Device_FlipHorizontal( bool flip ) {};

	/// <summary>Virtual function to access the hardware and set vertical image flipping.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <param name="flip">Activate vertical image flipping. true: on ; false: off.</param>
	virtual void set_Device_FlipVertical( bool flip ) {};

	/// <summary>Virtual function to access the hardware and get current exposure value.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Exposure value.</returns>
	virtual int get_Device_Exposure( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get auto exposure state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for auto exposure. true: on ; false: off.</returns>
	virtual bool get_Device_Exposure_Auto( void ) { return false; };

	/// <summary>Virtual function to access the hardware and get current gain value.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Exposure value.</returns>
	virtual int get_Device_Gain( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get auto gain state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for auto gain. true: on ; false: off.</returns>
	virtual bool get_Device_Gain_Auto( void ) { return false; };

	/// <summary>Virtual function to access the hardware and get current brightness value.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Brightness value.</returns>
	virtual int get_Device_Brightness( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get auto brightness state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for auto brightness. true: on ; false: off.</returns>
	virtual bool get_Device_Brightness_Auto( void ) { return false; };

	/// <summary>Virtual function to access the hardware and get current value for red 
	/// component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Red component of white balance.</returns>
	virtual int get_Device_WhiteBalance_Red( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get current value for green 
	/// component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Green component of white balance.</returns>
	virtual int get_Device_WhiteBalance_Green( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get current value for blue 
	/// component of white balance.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Blue component of white balance.</returns>
	virtual int get_Device_WhiteBalance_Blue( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get auto white balance state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for auto white balance. true: on ; false: off.</returns>
	virtual bool get_Device_WhiteBalance_Auto( void ) { return false; };

	/// <summary>Virtual function to access the hardware and get current gamma value.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Gamma value.</returns>
	virtual int get_Device_Gamma( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get current saturation value.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>Saturation value.</returns>
	virtual int get_Device_Saturation( void ) { return 0; };

	/// <summary>Virtual function to access the hardware and get horizontal image flipping state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for horizontal image flipping. true: on ; false: off.</returns>
	virtual bool get_Device_FlipHorizontal( void ) { return false; };

	/// <summary>Virtual function to access the hardware and get vertical image flipping state.
	/// <para>To be implemented in each different grabber.</para></summary>
	/// <returns>State for vertical image flipping. true: on ; false: off.</returns>
	virtual bool get_Device_FlipVertical( void ) { return false; };

//---------------------------------------------------------------------------
// Load/Save video parameters
protected:
	/// <summary>Parses the parameters of grabber controls to be loaded from a file.
	/// <para>Implementation of CConfigFile::Parse_Parameters_Load.</para></summary>	
	bool Parse_Parameters_Load( void );

	/// <summary>Parses the parameters of grabber controls to be written to a file.
	/// <para>Implementation of CConfigFile::Parse_Parameters_Save.</para></summary>	
	void Parse_Parameters_Save( void );
};

#endif // _GRABBER_SVS_VIDEO_H_
