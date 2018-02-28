#ifndef _TIME_COUNT_YARP_H_
#define _TIME_COUNT_YARP_H_

#pragma once

#include "TimeCount.h"
#include "TimeCountBasic.h"

/// <summary>Time counting implementation using YARP (thread safe).</summary>
class CTimeCountYARP
	: public CTimeCount
{
public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

//------------------------------------------------------------------------------

/// <summary>Time counting implementation using YARP.</summary>
class CTimeCountBasicYARP
	: public CTimeCountBasic
{
public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

#endif // _TIME_COUNT_YARP_H_
