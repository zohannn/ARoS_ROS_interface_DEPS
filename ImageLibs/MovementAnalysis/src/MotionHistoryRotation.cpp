#include "MotionHistoryRotation.h"
#include "MathOperations.h"

CMotionHistoryRotation::CMotionHistoryRotation( unsigned int count )
: CMotionHistory( count )
{
}

float CMotionHistoryRotation::Calculate_Distance( cv::Point3f dist3 )
{
	// Using Simultaneous Orthogonal Rotations Angle (SORA)
	// The equivalent rotation in 3 Axis is given as a single rotation by norm2 of the components:
	return norm2( dist3.x, dist3.y, dist3.z );
}

float CMotionHistoryRotation::Calculate_Speed( cv::Point3f vel )
{
	// Using Simultaneous Orthogonal Rotations Angle (SORA)
	// The equivalent rotation in 3 Axis is given as a single rotation by norm2 of the components:
	return norm2( vel.x, vel.y, vel.z );
}
