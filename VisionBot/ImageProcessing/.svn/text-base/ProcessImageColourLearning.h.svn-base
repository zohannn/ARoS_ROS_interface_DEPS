#ifndef _PROCESS_IMAGE_COLOUR_LEARNING_H_
#define _PROCESS_IMAGE_COLOUR_LEARNING_H_

#pragma once

#include "ProcessImage.h"
#include "HistogramHue.h"

class CProcessImageColourLearning 
	: public CProcessImage
{
public:
	CProcessImageColourLearning( int width = 640, int height = 480 );
	~CProcessImageColourLearning( void );

public:
	bool SearchAllObjects( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

protected:
	void SetupProcessing_TaskSpecific( int width, int height );

	void Clean_TaskSpecific( void );

protected:
	bool SearchObjects_Hand( void );

protected:
	CHistogramHue histogram;

	cv::Mat img_mat_hsv;
};

#endif // _PROCESS_IMAGE_COLOUR_LEARNING_H_
