#include "TimeCountOpenCV.h"

#ifdef USE_OpenCV
	#include <opencv2/core/core.hpp>
#endif

double Time_Now_OpenCV( void )
{
#ifdef USE_OpenCV
	return static_cast<double>( cv::getTickCount() )/cv::getTickFrequency();
#else
	return 0.0;
#endif
}

double CTimeCountOpenCV::Time_Now( void )
{
	return Time_Now_OpenCV();
}

double CTimeCountBasicOpenCV::Time_Now( void )
{
	return Time_Now_OpenCV();
}
