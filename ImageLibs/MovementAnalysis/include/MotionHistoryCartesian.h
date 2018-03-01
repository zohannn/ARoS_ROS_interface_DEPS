#ifndef _MOTION_HISTORY_CARTESIAN_H_
#define _MOTION_HISTORY_CARTESIAN_H_

#pragma once

#include "MotionHistory.h"

class CMotionHistoryCartesian :
	public CMotionHistory
{
public:
	CMotionHistoryCartesian( unsigned int count = 5 );

public:
	float Calculate_Distance( cv::Point3f dist3 );
	float Calculate_Speed( cv::Point3f vel );
};

#endif // _MOTION_HISTORY_CARTESIAN_H_
