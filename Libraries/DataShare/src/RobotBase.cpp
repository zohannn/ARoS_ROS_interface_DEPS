#include "RobotBase.h"

RobotBase::RobotBase( void )
: dX( 0.0f )
, dY( 0.0f )
, dZ( 0.0f )
, dPhiDeg( 0.0f )
, nType( 0 )
{
}

RobotBase::RobotBase( const RobotBase &obj )
: dX( obj.dX )
, dY( obj.dY )
, dZ( obj.dZ )
, dPhiDeg( obj.dPhiDeg )
, nType( obj.nType )
{
}
	
bool RobotBase::operator==( RobotBase r )
{
	return (
		( dX == r.dX ) &&
		( dY == r.dY ) &&
		( dZ == r.dZ ) );
}
