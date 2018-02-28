#include "ObjectBasic.h"

CObjectBasic::CObjectBasic( void )
: uType( 0 )
, Coord_Image( 0, 0 )
, Coord_World( 0.0f, 0.0f, 0.0f )
{}

CObjectBasic::CObjectBasic( unsigned int type )
: uType( type )
, Coord_Image( 0, 0 )
, Coord_World( 0.0f, 0.0f, 0.0f )
{}

CObjectBasic::CObjectBasic( unsigned int type, cv::Point2i pt_i, cv::Point3f pt_w )
: uType( type )
, Coord_Image( pt_i )
, Coord_World( pt_w )
{}

CObjectBasic::CObjectBasic( unsigned int type, int i_X, int i_Y, float w_X, float w_Y, float w_Z )
: uType( type )
, Coord_Image( i_X, i_Y )
, Coord_World( w_X, w_Y, w_Z )
{}

CObjectBasic::CObjectBasic( const CObjectBasic& obj )
: uType( obj.uType )
, Coord_Image( obj.Coord_Image )
, Coord_World( obj.Coord_World )
{}

CObjectBasic& CObjectBasic::operator=( CObjectBasic obj )
{
	uType = obj.uType;
	Coord_Image = obj.Coord_Image;
	Coord_World = obj.Coord_World;
	return *this;
}
