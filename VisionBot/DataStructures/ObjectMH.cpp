#include "ObjectMH.h"

#ifndef DEFAULT_HISTORY_SIZE_OBJECTMH
	#define DEFAULT_HISTORY_SIZE_OBJECTMH 10
#endif

CObjectMH::CObjectMH( void )
: CMotionHistoryCartesian( DEFAULT_HISTORY_SIZE_OBJECTMH )
, CObject_f()
{
}

CObjectMH::CObjectMH( unsigned int type )
: CMotionHistoryCartesian( DEFAULT_HISTORY_SIZE_OBJECTMH )
, CObject_f( type )
{
}

CObjectMH::CObjectMH( unsigned int type, int x, int y, int width, int height )
: CMotionHistoryCartesian( DEFAULT_HISTORY_SIZE_OBJECTMH )
, CObject_f( type, x, y, width, height )
{
}

CObjectMH::CObjectMH( unsigned int type, cv::Rect region )
: CMotionHistoryCartesian( DEFAULT_HISTORY_SIZE_OBJECTMH )
, CObject_f( type, region )
{
}

CObjectMH::CObjectMH( const CObjectMH& obj )
{
	CObject_f::Copy_From( &obj );
	CMotionHistoryCartesian::Copy_From( &obj );
}

bool CObjectMH::operator==( int id )
{
	return ( nObjectID == id );
}

bool CObjectMH::operator==( CObjectMH obj )
{
	return this->operator==( obj.nObjectID );
}
