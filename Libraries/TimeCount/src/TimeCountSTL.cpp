#include "TimeCountSTL.h"

CTimeCountSTL::CTimeCountSTL( void )
#ifdef USE_STL
: time_init( std::chrono::high_resolution_clock::now() )
#endif
{
}

double CTimeCountSTL::Time_Now( void )
{
#ifdef USE_STL
	return std::chrono::duration_cast<std::chrono::duration<double>>( std::chrono::high_resolution_clock::now() - time_init ).count();
#else
	return 0.0;
#endif
}

CTimeCountBasicSTL::CTimeCountBasicSTL( void )
#ifdef USE_STL
: time_init( std::chrono::high_resolution_clock::now() )
#endif
{
}

double CTimeCountBasicSTL::Time_Now( void )
{
#ifdef USE_STL
	return std::chrono::duration_cast<std::chrono::duration<double>>( std::chrono::high_resolution_clock::now() - time_init ).count();
#else
	return 0.0;
#endif
}
