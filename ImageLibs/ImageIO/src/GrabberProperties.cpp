#include "GrabberProperties.h"

CGrabberProperties::CGrabberProperties( void )
: Width( 0 )
, Height( 0 )
, FrameRate( 0.0f )
, Depth( 8 )
, Channels( 3 )
, Valid( false )
{
}

CGrabberProperties::CGrabberProperties( int width, int height )
: Width( width )
, Height( height )
, FrameRate( 0.0f )
, Depth( 8 )
, Channels( 3 )
, Valid( false )
{
}

CGrabberProperties::CGrabberProperties( CGrabberProperties &obj )
: Width( obj.Width.get() )
, Height( obj.Height.get() )
, FrameRate( obj.FrameRate )
, Depth( obj.Depth )
, Channels( obj.Channels )
, Valid( obj.Valid )
{
}

bool CGrabberProperties::ValidMode( CGrabberMode mode )
{
	// this function should be implemented in each grabber and define wich modes are supported
	// by the grabber
	return true;
}

void CGrabberProperties::set_GrabberMode( CGrabberMode mode )
{
	if( !ValidMode( mode ) ) return;

	Mode = mode;

	Valid.set( true );
	
	Width.set( Mode.get_Width() );
	Height.set( Mode.get_Height() );
	FrameRate.set( Mode.get_FrameRate() );
}

void CGrabberProperties::set_GrabberMode( int width, int height, float framerate, int depth, int channels )
{
	set_GrabberMode( CGrabberMode( width, height, framerate, depth, channels ) );
}

int CGrabberProperties::get_Width( void )
{
	return Width.get();
}

int CGrabberProperties::get_Height( void )
{
	return Height.get();
}

float CGrabberProperties::get_FrameRate( void )
{
	return FrameRate.get();
}

int CGrabberProperties::get_Depth( void )
{
	return Depth.get();
}

int CGrabberProperties::get_Channels( void )
{
	return Channels.get();
}

int CGrabberProperties::get_CVImageType( void )
{
	return Mode.get_cvImageType();
}

bool CGrabberProperties::is_ModeValid( void )
{
	return Valid.get();
}
