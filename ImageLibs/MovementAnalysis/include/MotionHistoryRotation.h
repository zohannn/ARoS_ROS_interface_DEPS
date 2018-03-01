#ifndef _MOTION_HISTORY_ROTATION_H_
#define _MOTION_HISTORY_ROTATION_H_

#pragma once

#include "MotionHistory.h"

class CMotionHistoryRotation :
	public CMotionHistory
{
public:
	CMotionHistoryRotation( unsigned int count = 5 );

public:
	float Calculate_Distance( cv::Point3f dist3 );
	float Calculate_Speed( cv::Point3f vel );
};

#endif // _MOTION_HISTORY_ROTATION_H_
