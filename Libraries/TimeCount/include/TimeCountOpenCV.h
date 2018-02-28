#ifndef _TIME_COUNT_OPENCV_H_
#define _TIME_COUNT_OPENCV_H_

#pragma once

#include "TimeCount.h"
#include "TimeCountBasic.h"

/// <summary>Time counting implementation using OpenCV (thread safe).</summary>
class CTimeCountOpenCV
	: public CTimeCount
{
public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

//------------------------------------------------------------------------------

/// <summary>Time counting implementation using OpenCV.</summary>
class CTimeCountBasicOpenCV
	: public CTimeCountBasic
{
public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

#endif // !defined( _TIME_COUNT_OPENCV_H_ ) && defined( USE_OpenCV )
