#ifndef _PROCESS_IMAGE_MOBILE_ROBOT_TARGET_1_H_
#define _PROCESS_IMAGE_MOBILE_ROBOT_TARGET_1_H_

#pragma once

#include "ProcessImage.h"

class CProcessImageMobileRobotTarget1
	: public CProcessImage
{
public:
	CProcessImageMobileRobotTarget1( int width = 640, int height = 480 );
	~CProcessImageMobileRobotTarget1(void);

public:
	bool SearchAllObjects( void );
	void ProcessTask( cv::Mat * mat_ptr );
	void ObjectPreProcessing( void );

protected:
	void Clean_TaskSpecific( void );

protected:
	inline float get_PhysicalDistance( int pixels );
	bool ComputeTarget(int type, int width, int height, float *angle, float *distance);

protected:
	cv::Mat mask_mirror;
};

#endif // _PROCESS_IMAGE_MOBILE_ROBOT_TARGET_1_H_
