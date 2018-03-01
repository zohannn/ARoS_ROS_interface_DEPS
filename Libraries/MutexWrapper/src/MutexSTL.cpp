#include "MutexSTL.h"

void CMutexSTL::lock( void )
{
#ifdef USE_STL
	mutex.lock();
#endif
}

void CMutexSTL::unlock( void )
{
#ifdef USE_STL
	mutex.unlock();
#endif
}

bool CMutexSTL::try_lock( void )
{
#ifdef USE_STL
	return mutex.try_lock();
#else
	return CMutexBase::try_lock();
#endif
}

void CMutexSTLPtr::lock( void )
{
#ifdef USE_STL
	pmutex->lock();
#endif
}

void CMutexSTLPtr::unlock( void )
{
#ifdef USE_STL
	pmutex->unlock();
#endif
}

bool CMutexSTLPtr::try_lock( void )
{
#ifdef USE_STL
	return pmutex->try_lock();
#else
	return 	CMutexBasePtr::try_lock();
#endif
}

