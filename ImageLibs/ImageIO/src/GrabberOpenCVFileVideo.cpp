#include "GrabberOpenCVFileVideo.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "Files_Directories.h"

CGrabberOpenCVFileVideo::CGrabberOpenCVFileVideo( std::string file_name )
: CGrabberOpenCVCapture()
, bVideoLoop( true )
{
	sFileName = file_name;
}

bool CGrabberOpenCVFileVideo::OpenGrabber( void )
{
	return OpenGrabber( sFileName );
}


bool CGrabberOpenCVFileVideo::OpenGrabber( std::string file_name )
{
	bool bVerifiedFileName = false;

	if( !file_name.empty() )
	{
		if( FileExists( file_name ) )
		{
			sFileName = file_name;
			bVerifiedFileName = true;
		}
	}
	
	// If the passed argument was verified and valid advance, otherwise, test the preconfigured file
	if( !bVerifiedFileName ) 
	{
		if( !FileExists( sFileName ) )
			return false;
	}

	// has the capture been opened sucessfully?
	if( !open( sFileName ) ) return false;
	
	int propId = 0;

#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
	propId = cv::CAP_PROP_FRAME_HEIGHT;
#else
	propId = CV_CAP_PROP_FRAME_HEIGHT;
#endif

	// Get video height
	int val = static_cast<int>( get( propId ) );
	Height.set( val );
	
#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
	propId = cv::CAP_PROP_FRAME_WIDTH;
#else
	propId = CV_CAP_PROP_FRAME_WIDTH;
#endif

	// Get video width
	val = static_cast<int>( get( propId ) );
	Width.set( val );

#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
	propId = cv::CAP_PROP_FRAME_COUNT;
#else
	propId = CV_CAP_PROP_FRAME_COUNT;
#endif


	// Get video total frames
	dFrameCount = get( propId );

	return isOpened();
}

bool CGrabberOpenCVFileVideo::get_Frame( void )
{
	if( bVideoLoop.get() )
	{
		int propId =
		#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
			cv::CAP_PROP_POS_FRAMES
		#else
			CV_CAP_PROP_POS_FRAMES
		#endif
			;

		// If video loop is active, when the end of the video is reached, start over.
		double dFrame = get( propId );

		if( dFrame >= dFrameCount )
			set( propId, 0.0 );
	}

	return grab();
}
