#include "GrabberOpenCVCamera.h"
#include <opencv2/core/version.hpp>

CGrabberOpenCVCamera::CGrabberOpenCVCamera( int width, int height )
: CGrabberOpenCVCapture( width, height )
{
	bControlParameters.set( true );
	bControl_Brightness.set( true );
	bControl_Exposure.set( true );
	bControl_Gain.set( true );
	bControl_Saturation.set( true );
	bControl_WhiteBalance.set( true );
	bControl_WB_Blue.set( true );
	bControl_WB_Red.set( true );
}

bool CGrabberOpenCVCamera::OpenGrabber( void )
{
	return OpenGrabber( 0 );
}

bool CGrabberOpenCVCamera::OpenGrabber( int nCamera )
{
	// has the capture been opened sucessfully?
	if( !open( nCamera ) ) return false;
	
	int	
#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		// OpenCV 3.0+
		prop_frame_h = cv::CAP_PROP_FRAME_HEIGHT,
		prop_frame_w = cv::CAP_PROP_FRAME_WIDTH,
		prop_fps = cv::CAP_PROP_FPS;
#else
		// OpenCV 2
		prop_frame_h = CV_CAP_PROP_FRAME_HEIGHT,
		prop_frame_w = CV_CAP_PROP_FRAME_WIDTH,
		prop_fps = CV_CAP_PROP_FPS;
#endif

	set( prop_frame_h, Height.get() );
	set( prop_frame_w, Width.get() );
	set( prop_fps, static_cast<double>( FrameRate.get() ) );

	return isOpened();
}
