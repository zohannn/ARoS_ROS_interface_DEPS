#include "Object.h"

#include <algorithm>

Object::Object( void )
: isInHumanHand( false )
, isInserted( false )
, isGrip( 0 )
, isTarget( 0 )
, nType( 0 )
, nTask( 0 )
, dX( 0.0f )
, dY( 0.0f )
, dZ( 0.0f )
, dPhiDeg( 0.0f )
, dPsiDeg( 0.0f )
, dTetaDeg( 0.0f )
, pnListObject( nullptr )
{
	aColor[0] = 0;
	aColor[1] = 0;
	aColor[2] = 0;
	std::fill_n(sZsize, 3, 0.0f);
}

Object::Object( const Object &obj )
: isInHumanHand( obj.isInHumanHand )
, isInserted( obj.isInserted )
, isGrip( obj.isGrip )
, isTarget( obj.isTarget )
, nType( obj.nType )
, nTask( obj.nTask )
, dX( obj.dX )
, dY( obj.dY )
, dZ( obj.dZ )
, dPhiDeg( obj.dPhiDeg )
, dPsiDeg( obj.dPsiDeg )
, dTetaDeg( obj.dTetaDeg )
, pnListObject( obj.pnListObject )
{
	aColor[0] = obj.aColor[0];
	aColor[1] = obj.aColor[1];
	aColor[2] = obj.aColor[2];

	std::copy_n(obj.sZsize, 3, sZsize);
	default_pose = obj.default_pose;
}

bool Object::operator==( Object o )
{
	return ( ( dX == o.dX ) && ( dY == o.dY ) && ( dZ == o.dZ ) && ( nType == o.nType ) );
}

bool Object::EqualType( Object o1, Object o2 )
{
	return o1.nType==o2.nType;
}

bool Object::GetByType( Object o )
{
	return nType==o.nType;
}

bool Object::EqualColor( Object o1, Object o2 )
{
	return o1.aColor[0] == o2.aColor[0];
}

bool Object::GetByColor( int nColor )
{
	return ( aColor[0] == nColor );
}

bool Object::FindByColor( Object o1, Object o2 )
{
	return ( o1.aColor[0] == o2.aColor[0] );
}

bool Object::FindByType( Object o1, Object o2 )
{
	return (o1.nType == o2.nType);
}
