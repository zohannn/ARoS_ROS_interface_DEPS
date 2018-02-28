#include "GrabberOpenCVCapture.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/version.hpp>

CGrabberOpenCVCapture::CGrabberOpenCVCapture( int width, int height )
: CGrabber( width, height )
{
}

CGrabberOpenCVCapture::~CGrabberOpenCVCapture( void )
{
	Close();
}

bool CGrabberOpenCVCapture::CloseGrabber( void )
{
    if( isOpened() )
		release();

	return true;
}

bool CGrabberOpenCVCapture::get_Frame( void )
{
	return grab();
}

bool CGrabberOpenCVCapture::get_Image( cv::Mat * mat_ptr )
{
	//Lock the image access
	matImage.lock();

	if( mat_ptr == nullptr )
	{
		if( mat_ptr->empty() )
			mat_ptr->create( cv::Size( Width.get(), Height.get() ), CV_8UC3 );
	}

	cv::Size size_dst( mat_ptr->cols, mat_ptr->rows );

	cv::Mat 
		the_Image,
		mat_aux( size_dst, mat_ptr->type() );

	if( !retrieve( the_Image ) )
	{
		the_Image.release();
		mat_aux.release();
		return false;
	}

	cv::Mat * pMat = &the_Image;
	
	int 
		nSize_src = static_cast<int>( *the_Image.step.p ) * the_Image.rows,
		nSize_dst = static_cast<int>( *mat_ptr->step.p ) * mat_ptr->rows;

	if( nSize_src != nSize_dst )
	{
		cv::resize( the_Image, mat_aux, size_dst );
		pMat = &mat_aux;
	}

	pMat->copyTo( *mat_ptr );

	the_Image.release();
	mat_aux.release();

	//Unlock the image access
	matImage.unlock();

	return true;
}

//---------------------------------------------------------------------------

void CGrabberOpenCVCapture::set_Camera_Video_Parameter( int param, int value )
{
	set( param, static_cast<double>( value ) );
}

int CGrabberOpenCVCapture::get_Camera_Video_Parameter( int param )
{
	return static_cast<int>( get( param ) );
}

//---------------------------------------------------------------------------

void CGrabberOpenCVCapture::set_Device_Exposure( int exp )
{
// CV_CAP_PROP_EXPOSURE only available in OpenCV version 2.0 or above
#if( CV_VERSION_EPOCH >= 2 )
	// OpenCV 2
	set_Camera_Video_Parameter( CV_CAP_PROP_EXPOSURE, exp );
#elif( CV_VERSION_MAJOR >= 3 )
	// OpenCV 3.0+
	set_Camera_Video_Parameter( cv::CAP_PROP_EXPOSURE, exp );
#endif
}

void CGrabberOpenCVCapture::set_Device_Exposure_Auto( bool autoexp )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	set_Camera_Video_Parameter( cv::CAP_PROP_AUTO_EXPOSURE, autoexp ? 1 : 0 );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) )
	// CV_CAP_PROP_AUTO_EXPOSURE only available in OpenCV version 2.3 or above
	set_Camera_Video_Parameter( CV_CAP_PROP_AUTO_EXPOSURE, autoexp ? 1 : 0 );
#endif
}

void CGrabberOpenCVCapture::set_Device_Gain(  int gain )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	set_Camera_Video_Parameter( cv::CAP_PROP_GAIN, gain );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 2 ) )
	// CV_CAP_PROP_GAIN only available in OpenCV version 2.2 or above
	set_Camera_Video_Parameter( CV_CAP_PROP_GAIN, gain );
#endif
}

void CGrabberOpenCVCapture::set_Device_Brightness( int bright )
{
	set_Camera_Video_Parameter( 
#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		// OpenCV 3.0+
		cv::CAP_PROP_BRIGHTNESS
#else
		CV_CAP_PROP_BRIGHTNESS
#endif
		, bright );
}

void CGrabberOpenCVCapture::set_Device_Saturation( int sat )
{
	set_Camera_Video_Parameter( 
#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		// OpenCV 3.0+
		cv::CAP_PROP_SATURATION
#else
		CV_CAP_PROP_SATURATION
#endif
		, sat );
}

void CGrabberOpenCVCapture::set_Device_WhiteBalance_Red( int red )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
#if( CV_VERSION_MINOR >= 1 )
	set_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_RED_V, red );
#else
	set_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_V, red );
#endif

#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) && ( CV_VERSION_MAJOR <= 4 ) && ( CV_VERSION_MINOR < 11 ) )
	// CV_CAP_PROP_WHITE_BALANCE_RED_V only available in OpenCV version 2.3 or above
	set_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_RED_V, red );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 4 ) && ( CV_VERSION_MINOR >= 11 ) )
	// In OpenCV 2.4.11 this property was changed
	set_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_V, red );
#endif
}

void CGrabberOpenCVCapture::set_Device_WhiteBalance_Blue( int blue )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
#if( CV_VERSION_MINOR >= 1 )
	set_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_BLUE_U, blue );
#else
	set_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_U, blue );
#endif

#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) && ( CV_VERSION_MAJOR <= 4 ) && ( CV_VERSION_MINOR < 11 ) )
	// CV_CAP_PROP_WHITE_BALANCE_RED_V only available in OpenCV version 2.3 or above
	set_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_BLUE_U, blue );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 4 ) && ( CV_VERSION_MINOR >= 11 ) )
	// In OpenCV 2.4.11 this property was changed
	set_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_U, blue );
#endif
}

int CGrabberOpenCVCapture::get_Device_Exposure( void )
{
#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	return get_Camera_Video_Parameter( cv::CAP_PROP_EXPOSURE );
#elif( CV_VERSION_EPOCH >= 2 )
	// CV_CAP_PROP_EXPOSURE only available in OpenCV version 2.0 or above
	return get_Camera_Video_Parameter( CV_CAP_PROP_EXPOSURE );
#else
	return 0;
#endif
}

bool CGrabberOpenCVCapture::get_Device_Exposure_Auto( void )
{
#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	return ( get_Camera_Video_Parameter( cv::CAP_PROP_AUTO_EXPOSURE ) == 1 );
#elif( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) )
	// CV_CAP_PROP_AUTO_EXPOSURE only available in OpenCV version 2.3 or above
	return ( get_Camera_Video_Parameter( CV_CAP_PROP_AUTO_EXPOSURE ) == 1 );
#else
	return false;
#endif
}

int CGrabberOpenCVCapture::get_Device_Gain( void )
{
#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	return get_Camera_Video_Parameter( cv::CAP_PROP_GAIN );
#elif( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 2 ) )
	// CV_CAP_PROP_GAIN only available in OpenCV version 2.2 or above
	return get_Camera_Video_Parameter( CV_CAP_PROP_GAIN );
#else
	return 0;
#endif
}

int CGrabberOpenCVCapture::get_Device_Brightness( void )
{
#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	return get_Camera_Video_Parameter( cv::CAP_PROP_BRIGHTNESS );
#else
	return get_Camera_Video_Parameter( CV_CAP_PROP_BRIGHTNESS );
#endif
}

int CGrabberOpenCVCapture::get_Device_Saturation( void )
{
#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
	return get_Camera_Video_Parameter( cv::CAP_PROP_SATURATION );
#else
	return get_Camera_Video_Parameter( CV_CAP_PROP_SATURATION );
#endif
}

int CGrabberOpenCVCapture::get_Device_WhiteBalance_Red( void )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
#if( CV_VERSION_MINOR >= 1 )
	return get_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_RED_V );
#else
	return get_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_V );
#endif
	
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) && ( CV_VERSION_MAJOR <= 4 ) && ( CV_VERSION_MINOR < 11 ) )
	// CV_CAP_PROP_WHITE_BALANCE_RED_V only available in OpenCV version 2.3 or above
	return get_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_RED_V );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 4 ) && ( CV_VERSION_MINOR >= 11 ) )
	// In OpenCV 2.4.11 this property was changed
	return get_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_V );
#else
	return 0;
#endif
}

int CGrabberOpenCVCapture::get_Device_WhiteBalance_Blue( void )
{
#if( ( CV_VERSION_MAJOR >=3 ) && !defined( CV_VERSION_EPOCH ) )
	// OpenCV 3.0+
#if( CV_VERSION_MINOR >= 1 )
	return get_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_BLUE_U );
#else
	return get_Camera_Video_Parameter( cv::CAP_PROP_WHITE_BALANCE_U );
#endif

#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 3 ) && ( CV_VERSION_MAJOR <= 4 ) && ( CV_VERSION_MINOR < 11 ) )
	// CV_CAP_PROP_WHITE_BALANCE_RED_V only available in OpenCV version 2.3 or above
	return get_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_BLUE_U );
#elif ( ( CV_VERSION_EPOCH >= 2 ) && ( CV_VERSION_MAJOR >= 4 ) && ( CV_VERSION_MINOR >= 11 ) )
	// In OpenCV 2.4.11 this property was changed
	return get_Camera_Video_Parameter( CV_CAP_PROP_WHITE_BALANCE_U );
#else
	return 0;
#endif
}
