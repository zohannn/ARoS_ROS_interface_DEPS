#ifndef _MUTEX_BASE_H_
#define _MUTEX_BASE_H_

#pragma once

#include <new>
/// <summary>CMutexBase<Type> implements a wrapper to guard any data type
/// with a Mutex object.
/// <para>This class is a template, and its used to implement a wrapper around
/// different mutex types to create an abstraction layer between the user and the
/// type of mutex being used.</para></summary>
template <typename Type>
class CMutexBase
{
protected:
	/// <summary>The mutex object that guards data.</summary>
	Type mutex;

public:
	/// <summary>Virtual function that serves to lock the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para>
	/// <para>This function waits for the mutex to be available.</para></summary>
	virtual void lock( void )=0;

	/// <summary>Virtual function that serves to unlock the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para></summary>
	virtual void unlock( void ) =0;

	/// <summary>Virtual function that serves to try locking the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para>
	/// <para>Unlike function <see cref="lock()"/> that waits for the
	/// mutex to be available, this function doesn't wait.</para></summary>
	/// <returns>true if the mutex was locked, false if it's not.</returns>
	virtual bool try_lock( void ) { return false; };
};

/// <summary>CMutexBasePtr is a wrapper class that holds a pointer to a mutex;
/// <para>Before being used, the alloc() function needs to be called to instantiate 
/// the mutex.</para>
/// <para>This class is a template, and its used to implement a wrapper around
/// different mutex types to create an abstraction layer between the user and the
/// type of mutex being used.</para>
/// <para>Useful when working with vectors or other containers, because offers the 
/// possibility to instantiate the semaphore in a different context.</para></summary>
template <typename Type>
class CMutexBasePtr
{
protected:
	/// <summary>The mutex object that guards data.</summary>
	Type * pmutex;

public:
	///<summary>Instantiate a CMutexBasePtr<Type> object.</summary>
	CMutexBasePtr( void ) : pmutex( nullptr ) {};

	///<summary>Destroy the CMutexBasePtr<Type> object.</summary>
	~CMutexBasePtr( void ){ free(); };

	/// <summary>Virtual function that serves to lock the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para>
	/// <para>This function waits for the mutex to be available.</para></summary>
	virtual void lock( void ){};

	/// <summary>Virtual function that serves to unlock the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para></summary>
	virtual void unlock( void ){};

	/// <summary>Virtual function that serves to try locking the mutex.
	/// <para>The implementation  is done in the derived class because
	/// depends on the type of mutex being used.</para>
	/// <para>Unlike function <see cref="lock()"/> that waits for the
	/// mutex to be available, this function doesn't wait.</para></summary>
	/// <returns>true if the mutex was locked, false if it's not.</returns>
	virtual bool try_lock( void ){ return false; };

	/// <summary>Instantiate the mutex object.</summary>
	virtual void alloc( void )
	{
		free();
		pmutex = new 
		#ifndef _DEBUG 
			(std::nothrow) 
		#endif
			Type();
	};

	/// <summary>Clear the mutex object by releasing the allocated memory.</summary>
	virtual void free( void )
	{
		if( valid() )
		{
			delete pmutex;
			pmutex = nullptr;
		}
	};

protected:
	/// <summary>Test the validity of the mutex object.</summary>
	/// <returns>true if there is a valid mutex object, otherwise false.</returns>
	virtual bool valid( void ) { return ( pmutex != nullptr ); };
};

#endif // _MUTEX_BASE_H_
