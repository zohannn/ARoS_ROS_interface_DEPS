#ifndef _PROCESS_IMAGE_BAUFIX_H_
#define _PROCESS_IMAGE_BAUFIX_H_

#pragma once

#include "ProcessImage.h"

class CProcessImageBaufix 
	: public CProcessImage
{
public:
	CProcessImageBaufix( int width = 640, int height = 480 );
	~CProcessImageBaufix( void );

protected:
	cv::Mat 
		img_gray,
		img_canny;

	CSegmentedImage img_segmented;

	std::vector< cv::Rect > vWrist;

public:
	bool SearchAllObjects( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

	void Calculate_Pointing_Directions( void );

protected:
	void SetupProcessing_TaskSpecific( int width, int height );
	void Clean_TaskSpecific( void );
};

#endif // _PROCESS_IMAGE_BAUFIX_H_
