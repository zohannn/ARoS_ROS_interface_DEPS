#include "Colour.h"

CColour::CColour( int colour )
: sName( "" )
, r( 0 )
, g( 0 )
, b( 0 )
{
	set_Colour( colour );
}

CColour::CColour( const CColour &col )
: sName( col.sName )
, r( col.r )
, g( col.g )
, b( col.b )
, nColour_ID( col.nColour_ID )
{
}

void CColour::Copy_From( const CColour colour )
{
	sName = colour.sName;
	r = colour.r;
	g = colour.g;
	b = colour.b;
	nColour_ID = colour.nColour_ID;
}

void CColour::set_Colour( int colour )
{
	nColour_ID = GET_VALID_Colour( colour );
	sName = Name_Of_Colour[nColour_ID];
	r = R_Component[nColour_ID];
	g = G_Component[nColour_ID];
	b = B_Component[nColour_ID];
	
	if( !isValid_Colour( colour ) ) nColour_ID = -1;
}

void CColour::operator=( int colour )
{
	set_Colour( colour );
}

void CColour::operator=( const CColour colour )
{
	Copy_From( colour );
}

void CColour::operator=( cv::Scalar colour )
{
	r = static_cast<int>( colour.val[2] );
	g = static_cast<int>( colour.val[1] );
	b = static_cast<int>( colour.val[0] );
}

cv::Scalar CColour::get_cvScalar( void )
{
	return cv::Scalar( b, g, r );
}
