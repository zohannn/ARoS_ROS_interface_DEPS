#include "MotionHistoryCartesian.h"
#include "MathOperations.h"

CMotionHistoryCartesian::CMotionHistoryCartesian( unsigned int count )
: CMotionHistory( count )
{
}

float CMotionHistoryCartesian::Calculate_Distance( cv::Point3f dist3 )
{
	return norm2( dist3.x, dist3.y, dist3.z );
}

float CMotionHistoryCartesian::Calculate_Speed( cv::Point3f vel )
{
	return norm2( vel.x, vel.y, vel.z );
}
