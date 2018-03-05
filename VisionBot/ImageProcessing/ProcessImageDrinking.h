#ifndef _PROCESS_IMAGE_DRINKING_H_
#define _PROCESS_IMAGE_DRINKING_H_

#pragma once

#include "ProcessImage.h"
#include "TimeCountOpenCV.h"

class CProcessImageDrinking 
	: public CProcessImage
{
public:
	CProcessImageDrinking( int width = 640, int height = 480 );

public:
	bool SearchAllObjects( void );

	bool SearchObjects_Bottle( void );

	bool SearchObjects_GlassInverted( void );

	bool SearchObjects_Glass( void );

	bool SearchObjects_Hand( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

protected:
	unsigned int isGlassPresent( void );
	unsigned int isGlassInvertedPresent( void );
	unsigned int isGlass_in_HandOver_Position( void );

	unsigned int isBottle_in_Place( void );
	unsigned int isBottle_Lifted( void );
	unsigned int isBottle_Turned( void );

	void Hand_Reach_Release_Glass( void );
	void Hand_Reach_Release_GlassInverted( void );
	void Hand_Reach_Release_Bottle( void );

	void Hand_Reach_Release_Object(unsigned int object, int gesture, CTimeCountBasic * timer_reach, CTimeCountBasic * timer_release, bool * bHand_Near_Frame_Previous, bool * bHand_Near_Frame_Current, bool * bHand_Reach, bool * bHand_Release);
	
	bool
		bHand_Near_Glass_Frame_Previous,
		bHand_Near_Glass_Frame_Current,
		bHand_Reach_Glass,
		bHand_Release_Glass,
		bHand_Near_Glass_Inverted_Frame_Previous,
		bHand_Near_Glass_Inverted_Frame_Current,
		bHand_Reach_Glass_Inverted,
		bHand_Release_Glass_Inverted,
		bHand_Near_Bottle_Frame_Previous,
		bHand_Near_Bottle_Frame_Current,
		bHand_Reach_Bottle,
		bHand_Release_Bottle;
	
	// How much time the information is going to be kept
	double dTime_to_Retain_Information;

	CTimeCountBasicOpenCV 
		Timer_Reach_Glass,
		Timer_Release_Glass,
		Timer_Reach_Glass_Inverted,
		Timer_Release_Glass_Inverted,
		Timer_Reach_Bottle,
		Timer_Release_Bottle;
};

#endif // _PROCESS_IMAGE_DRINKING_H_
