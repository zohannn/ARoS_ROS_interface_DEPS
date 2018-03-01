#include "GrabberYARPPort.h"

#include <yarp/os/Network.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core_c.h>

#include "YarpCommunication.h"

CGrabberYARPPort::CGrabberYARPPort( void )
: strPortName_Local( "/aros/vision/grabber" )
, strPortName_Remote( "/grabber" )
{
}

CGrabberYARPPort::~CGrabberYARPPort( void )
{
	Close();
}

void CGrabberYARPPort::set_PortNameLocal( std::string name )
{
	if( !is_GrabberOpen() )
		strPortName_Local = name;
}

void CGrabberYARPPort::set_PortNameRemote( std::string name )
{
	if( !is_GrabberOpen() )
		strPortName_Remote = name;
}

std::string CGrabberYARPPort::get_PortNameLocal( void )
{
	return strPortName_Local;
}

std::string CGrabberYARPPort::get_PortNameRemote( void )
{
	return strPortName_Remote;
}

bool CGrabberYARPPort::OpenGrabber( void )
{
	if( CYarpCommunication::isPortAlive( strPortName_Local ) ) return false;

	if( CYarpCommunication::isPortRegistered( strPortName_Local ) )
	{
		yarp::os::Network::unregisterName( strPortName_Local.c_str() );
	}

	if( !yPort_in.open( strPortName_Local.c_str() ) ) return false;
	
	bool bConnect = yarp::os::Network::connect( strPortName_Remote.c_str(), strPortName_Local.c_str() );

	if( !bConnect ) 
	{
		yarp::os::Network::unregisterName( strPortName_Local.c_str() );
	}

	return bConnect;
}

bool CGrabberYARPPort::CloseGrabber( void )
{
	// Remove connection
	bool bDisconnect = yarp::os::Network::disconnect( strPortName_Remote.c_str(), strPortName_Local.c_str() );
	
	if( bDisconnect )
	{
		// Close port
		yPort_in.close();
		yarp::os::Network::unregisterName( strPortName_Local.c_str() );
		return true;
	}

    return false;
}

bool CGrabberYARPPort::get_Frame( void )
{
	//no one else touches the Image pointer
	matImage.lock();

	const yarp::sig::ImageOf<yarp::sig::PixelRgb> &newImage = *yPort_in.read();

	cv::Size sz( newImage.width(), newImage.height() );
	
	int 
		depth = newImage.getPixelSize() * 8,
		step = newImage.getRowSize(),
		channels = step/( depth*sz.width ),
		type = CV_MAKETYPE( depth, channels );

	if( newImage.getRawImageSize() <= 0 ) return false;

	if( matImage.Obj.empty() )
	{
		matImage.Obj.create( sz, type );
	}
	else
	{
		if( matImage.Obj.cols != newImage.width() || matImage.Obj.rows != newImage.height() )
		{
			matImage.Obj.release();
			matImage.Obj.create( sz, type );
		}
	}
	
	matImage.Obj = cv::Mat( sz, type, newImage.getRawImage(), step );

	//Unlock the image access
	matImage.unlock();

	return true;
}

bool CGrabberYARPPort::get_Image( cv::Mat * mat_ptr )
{
	//Lock the image access
	matImage.lock();

	cv::Mat
		mat_aux,
		*the_Image = &matImage.Obj; // Take the pointer of the source image
	
	if( mat_ptr->empty() )
		mat_ptr->create( Height.get(), Width.get(), CV_8UC3 );

	else
	{
		int 
			nSize_src = static_cast<int>( *matImage.Obj.step.p ) * matImage.Obj.rows,
			nSize_dst = static_cast<int>( *mat_ptr->step.p ) * mat_ptr->rows;

		if( nSize_src != nSize_dst )
		{
			//if the size is different, create a resized version of the captured frame
			mat_aux.create( mat_ptr->rows, mat_ptr->cols, CV_MAKETYPE( mat_ptr->depth(), mat_ptr->channels() ) );

			cv::resize( matImage.Obj, mat_aux, mat_aux.size() );

			// Take the pointer of the resized image
			the_Image = &mat_aux;
		}
	}

	the_Image->copyTo( *mat_ptr );

	if( !mat_aux.empty() ) mat_aux.release();

	//Unlock the image access
	matImage.unlock();

	return true;
}
