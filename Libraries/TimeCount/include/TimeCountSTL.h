#ifndef _TIME_COUNT_STL_H_
#define _TIME_COUNT_STL_H_

#include "TimeCount.h"
#include "TimeCountBasic.h"

#ifdef USE_STL
#include <chrono>
#endif

/// <summary>Time counting implementation using STL (thread safe).</summary>
class CTimeCountSTL
	: public CTimeCount
{
public:
	/// <summary>Default constructor. Initializes STL time structure</summary>
	CTimeCountSTL( void );

#ifdef USE_STL
private:
	/// <summary>STL time initialization structure.</summary>
	const std::chrono::high_resolution_clock::time_point time_init;
#endif

public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

//------------------------------------------------------------------------------

/// <summary>Time counting implementation using STL.</summary>
class CTimeCountBasicSTL
	: public CTimeCountBasic
{
public:
	/// <summary>Default constructor. Initializes STL time structure</summary>
	CTimeCountBasicSTL( void );

#ifdef USE_STL
private:
	/// <summary>STL time initialization structure.</summary>
	const std::chrono::high_resolution_clock::time_point time_init;
#endif

public:
	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	double Time_Now( void );
};

#endif // _TIME_COUNT_STL_H_
