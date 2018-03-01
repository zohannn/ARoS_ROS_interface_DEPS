#include "MutexYarp.h"

void CMutexYarp::lock( void )
{
#ifdef USE_YARP
	mutex.wait();
#endif
}

void CMutexYarp::unlock( void )
{
#ifdef USE_YARP
	mutex.post();
#endif
}

bool CMutexYarp::try_lock( void )
{
#ifdef USE_YARP
	return mutex.check();
#else
	return CMutexBase::try_lock();
#endif
}

void CMutexYarpPtr::lock( void )
{
#ifdef USE_YARP
	pmutex->wait();
#endif
}

void CMutexYarpPtr::unlock( void )
{
#ifdef USE_YARP
	pmutex->post();
#endif
}

bool CMutexYarpPtr::try_lock( void )
{
#ifdef USE_YARP
	return pmutex->check();
#else
	return 	CMutexBasePtr::try_lock();
#endif
}
