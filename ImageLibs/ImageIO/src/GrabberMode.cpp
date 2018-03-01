#include "GrabberMode.h"
#include <opencv2/core/core.hpp>

CGrabberMode::CGrabberMode( int width, int height, float framerate, int depth, int channels )
{
	set_Mode( width, height, framerate, depth, channels );
}

CGrabberMode::CGrabberMode( const CGrabberMode& copy )
{
	set_Mode( copy.nWidth, copy.nHeight, copy.fFrameRate, copy.nDepth, copy.nChannels );
}

void CGrabberMode::set_Mode( int width, int height, float framerate, int depth, int channels )
{
	nWidth = width;
	nHeight = height;
	fFrameRate = framerate;
	nDepth = depth;
	nChannels = channels;

	cvImageType = CV_MAKETYPE( nDepth, nChannels );
}

int CGrabberMode::get_Width( void )
{
	return nWidth;
}

int CGrabberMode::get_Height( void )
{
	return nHeight;
}

float CGrabberMode::get_FrameRate( void )
{
	return fFrameRate;
}

int CGrabberMode::get_Depth( void )
{
	return nDepth;
}

int CGrabberMode::get_Channels( void )
{
	return nChannels;
}

int CGrabberMode::get_cvImageType( void )
{
	return cvImageType;
}

bool CGrabberMode::equal_to( int width, int height, float framerate )
{
	return 
		( nWidth == width ) && 
		( nHeight == height ) && 
		( fFrameRate == framerate );
}

bool CGrabberMode::equal_to( int width, int height, float framerate, int depth, int channels )
{
	return 
		( nWidth == width ) && 
		( nHeight == height ) && 
		( fFrameRate == framerate ) &&
		( nDepth == depth ) &&
		( nChannels == channels );
}

bool CGrabberMode::operator==( CGrabberMode op )
{
	return equal_to( op.nWidth, op.nHeight, op.fFrameRate, op.nDepth, op.nChannels );
}

CGrabberMode CGrabberMode::operator=( const CGrabberMode op )
{
	if( this != &op )
		set_Mode( op.nWidth, op.nHeight, op.fFrameRate, op.nDepth, op.nChannels );
	
	return *this;
}
