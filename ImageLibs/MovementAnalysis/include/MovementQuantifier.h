#ifndef _MOVEMENT_FRAMES_H_
#define _MOVEMENT_FRAMES_H_

#pragma once

#include <opencv2/core/core.hpp>

#include "AttachMutex.hpp"
#include "SegmentedImage.h"

class CMovementQuantifier
{
//---------------------------------------------------------------------------
// Construction / Destruction
public:
	CMovementQuantifier( unsigned int frame_count = 10 );
	CMovementQuantifier( const CMovementQuantifier &obj );
	~CMovementQuantifier( void );

//---------------------------------------------------------------------------
// Object setup and cleaning
public:
	void Setup( int width, int height, unsigned int frame_count = 10 );
	void Clear( void );

	bool isReady( void );

protected:
	CAttachMutex_n<bool> bReady;

//---------------------------------------------------------------------------
// Windows management
public:
	void set_Display_Image_Movement( bool state );
	void set_Display_Image_Stereo( bool state );

	bool get_Display_Image_Movement( void );
	bool get_Display_Image_Stereo( void );

protected:
	std::string 
		strWindowName_Movement,	// Name of window where to show the results
		strWindowName_Stereo;

	CAttachMutex_n<bool> 
		bWindowCreated_Movement,
		bWindowCreated_Stereo,
		bShowImage_Movement,
		bShowImage_Stereo;

	void Create_Window_Movement( void );
	void Create_Window_Stereo( void );

	void Destroy_Window_Movement( void );
	void Destroy_Window_Stereo( void );

//---------------------------------------------------------------------------
// Frames management
protected:
	unsigned int 
		iNextFrameDiff,	// Index where to store the next frame difference result
		iFrameCurrent,	// Index where to get the current frame
		iFramePrevious,	// Index where to get the previous frame
		uFrameCount;

	std::vector< cv::Mat > 
		vFrame,		// Vector that stores previous and current frame
		vFrameDiff;	// Vector that stores frame differences

	void Cycle_Frames_Index( void );

	// Vector to store the history of the regions of the user on the last frames
	std::vector< cv::Rect > vUserRoI;

//---------------------------------------------------------------------------
// Segmented images
protected:
	CSegmentedImage 
		*ptrSegmentedImage_Movement,
		*ptrSegmentedImage_Stereo;

public:
	void set_SegmentedImage_Pointer_Movement( CSegmentedImage * pSegImg );
	void set_SegmentedImage_Pointer_Stereo( CSegmentedImage * pSegImg );

//---------------------------------------------------------------------------
// Movement quantification functions
protected:
	void Perform_1xFrame_Difference( void );
	void Perform_MultiFrame_Difference( void );
	void Estimate_User_Movement( void );
	cv::Rect Estimate_User_Region( void );

public:
	void Process_Frame( cv::Mat * mat_ptr );

//---------------------------------------------------------------------------
// Movement quantification
public:
	CAttachMutex_n<float> fMovement;

//---------------------------------------------------------------------------
// Images part of this object
protected:
	cv::Mat 
		mat_ImageGray,
		mat_Image;
};

#endif // _MOVEMENT_FRAMES_H_
