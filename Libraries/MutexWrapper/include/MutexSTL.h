#ifndef _MUTEX_STL_H_
#define _MUTEX_STL_H_

/// <summary> Preprocessor condition that prevents compilation errors
/// in case the user is not using STL</summary>
#ifdef USE_STL
	#include <mutex>
#else
namespace std
{
	class mutex
	{
	public:
		mutex( void ) {};
	};
}
#endif

#include "MutexBase.hpp"

//---------------------------------------------------------------------------

/// <summary>Mutex class based in std::mutex.</summary>
class CMutexSTL
	: public CMutexBase<std::mutex>
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

/// <summary>Mutex pointer class based in std::mutex.</summary>
class CMutexSTLPtr
	: public CMutexBasePtr<std::mutex>
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

#endif // _MUTEX_STL_H_
