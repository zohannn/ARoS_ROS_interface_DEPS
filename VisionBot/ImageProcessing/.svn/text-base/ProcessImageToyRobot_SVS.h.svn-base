#ifndef _PROCESS_IMAGE_TOY_ROBOT_SVS_H_
#define _PROCESS_IMAGE_TOY_ROBOT_SVS_H_

#pragma once

#include "ProcessImage.h"

class CProcessImageToyRobot_SVS 
	: public CProcessImage
{
public:
	CProcessImageToyRobot_SVS( int width = 640, int height = 480 );

public:
	bool SearchAllObjects( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

public:
	cv::Rect Hand_ROI;
};

#endif // _PROCESS_IMAGE_TOY_ROBOT_SVS_H_
