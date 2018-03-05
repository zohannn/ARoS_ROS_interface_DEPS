#ifndef _PROCESS_IMAGE_TOY_ROBOT_FULL_AVT_H_
#define _PROCESS_IMAGE_TOY_ROBOT_FULL_AVT_H_

#pragma once

#include "ProcessImage.h"

class CProcessImageToyRobotFull_AVT 
	: public CProcessImage
{
public:
	CProcessImageToyRobotFull_AVT( int width = 640, int height = 480 );

public:
	bool SearchAllObjects( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );
};

#endif // _PROCESS_IMAGE_TOY_ROBOT_FULL_AVT_H_
