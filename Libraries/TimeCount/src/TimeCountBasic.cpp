#include "TimeCountBasic.h"

CTimeCountBasic::CTimeCountBasic(void)
: dTimeStart( 0.0 )
, dTimeFinish( 0.0 )
{
}

double CTimeCountBasic::get_Time_Start( void )
{
	return dTimeStart;
}

double CTimeCountBasic::get_Time_Finish( void )
{
	return dTimeFinish;
}

void CTimeCountBasic::set_Time_Start( void )
{
	dTimeStart = Time_Now();
}

void CTimeCountBasic::set_Time_Finish( void )
{
	dTimeFinish = Time_Now();
}

double CTimeCountBasic::HowLong( void )
{
	return ( dTimeFinish - dTimeStart );
}

double CTimeCountBasic::HowLong_untilNow( void )
{
	return ( Time_Now() - dTimeStart );
}
