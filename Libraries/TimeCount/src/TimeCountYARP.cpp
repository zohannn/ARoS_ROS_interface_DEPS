#include "TimeCountYARP.h"

#ifdef USE_YARP
	#include <yarp/os/Time.h>
#endif

double Time_Now_YARP( void )
{
#ifdef USE_YARP
	return yarp::os::Time::now();
#else
	return 0.0;
#endif
}

double CTimeCountYARP::Time_Now( void )
{
	return Time_Now_YARP();
}

double CTimeCountBasicYARP::Time_Now( void )
{
	return Time_Now_YARP();
}
