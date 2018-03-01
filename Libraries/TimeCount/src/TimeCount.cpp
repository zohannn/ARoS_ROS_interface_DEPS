#include "TimeCount.h"

CTimeCount::CTimeCount( void )
: dTimeStart( 0.0 )
, dTimeFinish( 0.0 )
{
}

double CTimeCount::get_Time_Start( void )
{
	return dTimeStart.get();
}

double CTimeCount::get_Time_Finish( void )
{
	return dTimeFinish.get();
}

void CTimeCount::set_Time_Start( void )
{
	dTimeStart.set( Time_Now() );
}

void CTimeCount::set_Time_Finish( void )
{
	dTimeFinish.set( Time_Now() );
}

double CTimeCount::HowLong( void )
{
	return ( dTimeFinish.get() - dTimeStart.get() );
}

double CTimeCount::HowLong_untilNow( void )
{
	return ( Time_Now() - dTimeStart.get() );
}
