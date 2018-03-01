#include "GrabberFileImage.h"

#include <opencv2/highgui/highgui.hpp>

CGrabberFileImage::CGrabberFileImage( std::string file_name )
: CGrabber()
{
	sFileName = file_name;
}

CGrabberFileImage::~CGrabberFileImage()
{
	Close();
}

bool CGrabberFileImage::OpenGrabber( void )
{
	matImage.lock();

	matImage.Obj = cv::imread( sFileName, -1 );

	Width.set( matImage.Obj.cols );
	Height.set( matImage.Obj.rows );

	matImage.unlock();

	return true;
}

bool CGrabberFileImage::CloseGrabber( void )
{
	return true;
}

bool CGrabberFileImage::get_Image( cv::Mat * mat_ptr )
{
	if( mat_ptr == nullptr ) return false;

	if( mat_ptr->empty() ) return false;

	matImage.lock();
	matImage.Obj.copyTo( *mat_ptr );
	matImage.unlock();

	return true;
}

bool CGrabberFileImage::get_Frame( void )
{
	return true;
}
