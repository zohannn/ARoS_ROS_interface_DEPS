#include "ImageStream.h"
#include <opencv2/core/core_c.h>
#include <yarp/os/Network.h>

#include "YarpCommunication.h"

CImageStream::CImageStream( std::string port_name, int width, int height )
: yarp::os::RateThread( 100 )
, sPort_Name( port_name )
{
	set_Resolution( width, height );
}

CImageStream::~CImageStream( void )
{
	matImage.lock();

	if( !matImage.Obj.empty() )
		matImage.Obj.release();

	matImage.unlock();
	
	Width.set( 0 );
	Height.set( 0 );
}

void CImageStream::set_Resolution( int width, int height )
{
	Width.set( width );
	Height.set( height );
	
	matImage.lock();

	if( !matImage.Obj.empty() ) matImage.Obj.release();

	matImage.Obj.create( width, height, CV_8UC3 );
	matImage.Obj.setTo( 0 );

	matImage.unlock();
}

void CImageStream::set_PortName( std::string port_name )
{ 
	if( !isRunning() ) 
		sPort_Name = port_name; 
}

std::string CImageStream::get_PortName( void )
{
	return sPort_Name;
}

bool CImageStream::threadInit( void )
{
	if( CYarpCommunication::isPortAlive( sPort_Name ) ) return false;

	if( CYarpCommunication::isPortRegistered( sPort_Name ) )
	{
		yarp::os::Network::unregisterName( sPort_Name.c_str() );
	}

	return outPort.open( sPort_Name.c_str() );
}

void CImageStream::run( void )
{
	outPort.lock();
	outPort.write( yarp_image.Obj );
	outPort.unlock();
}

void CImageStream::threadRelease( void )
{
	outPort.lock();
	outPort.close();
	outPort.unlock();
}

void CImageStream::beforeStart()
{
	matImage.lock();
	yarp_image.lock();
	
	yarp_image.Obj.wrapIplImage( &( (IplImage) matImage.Obj ) );
	yarp_image.Obj.setTopIsLowIndex( false );
	
	yarp_image.unlock();
	matImage.unlock();
}

void CImageStream::copyImage( cv::Mat * mat_ptr )
{
	matImage.lock();
	
	if( mat_ptr->empty() )
	{
		matImage.unlock();
		return;
	}
	
	if( !matImage.Obj.empty() ) matImage.Obj.release();
	
	matImage.Obj.create( mat_ptr->rows, mat_ptr->cols, CV_MAKETYPE( mat_ptr->depth(), mat_ptr->channels() ) );
	matImage.Obj.setTo( 0 );

	mat_ptr->copyTo( matImage.Obj );
	
	yarp_image.lock();

	yarp_image.Obj.wrapIplImage( &( (IplImage) matImage.Obj ) );
	yarp_image.Obj.setTopIsLowIndex( false );

	yarp_image.unlock();
	matImage.unlock();
}

void CImageStream::copyImage( cv::Mat * mat_ptr, cv::Rect region )
{
	if( mat_ptr == nullptr ) return;

	matImage.lock();
	
	int 
		img_size = matImage.Obj.rows * matImage.Obj.cols,
		external_size = mat_ptr->rows * mat_ptr->cols;
	
	if( ( img_size != external_size ) && ( !matImage.Obj.empty() ) )
	{
		matImage.Obj.release();
	}

	matImage.Obj.create( region.height, region.width, CV_MAKETYPE( mat_ptr->depth(), mat_ptr->channels() ) );
	matImage.Obj.setTo( 0 );

	(*mat_ptr)( region ).copyTo( matImage.Obj );
	
	yarp_image.lock();

	yarp_image.Obj.wrapIplImage( &( (IplImage) matImage.Obj ) );
	yarp_image.Obj.setTopIsLowIndex( false );

	matImage.unlock();
	yarp_image.unlock();
}

void CImageStream::copyImage_direct( cv::Mat * mat_ptr )
{
	matImage.lock();
	mat_ptr->copyTo( matImage.Obj );
	matImage.unlock();
}
