#include "Grabber.h"

CGrabber::CGrabber( int width, int height )
: CGrabberProperties( width, height )
, isOpen( false )
, bHasStereo( false )
, sFileName( "" )
{
	Width.set( width );
	Height.set( height );
}

CGrabber::CGrabber( CGrabber &obj )
: CGrabberProperties( static_cast<CGrabberProperties>( obj ) )
, isOpen( obj.isOpen )
, bHasStereo( obj.bHasStereo )
, sFileName( obj.sFileName )
{
	if( !matImage.Obj.empty() )
		matImage.Obj.release();

	matImage.Obj.create( obj.matImage.Obj.rows, obj.matImage.Obj.cols, obj.matImage.Obj.type() );

	obj.matImage.Obj.copyTo( matImage.Obj );
}

CGrabber::~CGrabber()
{
	Close();
	sFileName.clear();
}

bool CGrabber::Open( void )
{
	bool bOpen = OpenGrabber();
	isOpen.set( bOpen );
	return bOpen;
}

bool CGrabber::Close( void )
{
	bool bSuccess = true;
	
	if( isOpen.get() )
	{
		bSuccess = CloseGrabber();
		if( bSuccess ) isOpen.set( false );
	}

	matImage.lock();

	if( !matImage.Obj.empty() )
		matImage.Obj.release();

	matImage.unlock();

	return bSuccess;
}

bool CGrabber::has_Stereo( void )
{
	return bHasStereo.get();
}

bool CGrabber::is_GrabberOpen( void )
{
	return isOpen.get();
}

void CGrabber::set_FileName( std::string file_name )
{
	sFileName = file_name;
}
