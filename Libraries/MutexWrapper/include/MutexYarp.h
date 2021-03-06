#ifndef _MUTEX_YARP_H_
#define _MUTEX_YARP_H_

#pragma once

/// <summary> Preprocessor condition that prevents compilation errors
/// in case the user is not using YARP</summary>
#ifdef USE_YARP
	#include <yarp/os/Semaphore.h>
#else
namespace yarp{
	namespace os{ 
		class Semaphore
		{
		public:
			Semaphore( void ){};
		};
	};
};	
#endif

#include "MutexBase.hpp"

//---------------------------------------------------------------------------

/// <summary>Mutex class based in yarp::os::Semaphore.</summary>
class CMutexYarp
	: public CMutexBase<yarp::os::Semaphore>
{
public:
	/// <summary>Lock the mutex.
	/// <para>See <see cref=CMutexBase::lock"/> for more information.</para></summary>
	void lock( void );

	/// <summary>Unlock the mutex.
	/// <para>See <see cref=CMutexBase::unlock"/> for more information.</para></summary>
	void unlock( void );

	/// <summary>Try to lock the mutex.
	/// <para>See <see cref=CMutexBase::try_lock"/> for more information.</para></summary>
	/// <returns>true if the mutex was locked, otherwise false.</returns>
	bool try_lock( void );
};

//---------------------------------------------------------------------------

/// <summary>Mutex pointer class based in yarp::os::Semaphore.</summary>
class CMutexYarpPtr
	: public CMutexBasePtr<yarp::os::Semaphore>
{
public:
	/// <summary>Lock the mutex.
	/// <para>See <see cref=CMutexBase::lock"/> for more information.</para></summary>
	void lock( void );

	/// <summary>Unlock the mutex.
	/// <para>See <see cref=CMutexBase::unlock"/> for more information.</para></summary>
	void unlock( void );

	/// <summary>Try to lock the mutex.
	/// <para>See <see cref=CMutexBase::try_lock"/> for more information.</para></summary>
	/// <returns>true if the mutex was locked, otherwise false.</returns>
	bool try_lock( void );
};

#endif // _MUTEX_YARP_H_
