#include "GrabberCMU1394.h"

#include <opencv2/imgproc/imgproc.hpp>

CGrabberCMU1394::CGrabberCMU1394( int width, int height, _FRAME_RATE FrameRate )
: CGrabber( width, height )
, frame_rate( FrameRate )
, lengthOfBitmap( width * height * 3 )
{
	pBitmap = new 
	#ifndef _DEBUG 
		(std::nothrow) 
	#endif
		unsigned char[lengthOfBitmap];

	matImage.Obj.create( height, width, CV_8UC3 );
	matImage.Obj.setTo( 0 );

	matImage_Aux.create( height, width, CV_8UC3 );
	matImage_Aux.setTo( 0 );
}

CGrabberCMU1394::~CGrabberCMU1394( void )
{
	delete[] pBitmap;

	matImage_Aux.release();
	
#ifdef USE_CMU1394

	if( camera.IsAcquiring() )
		camera.StopImageAcquisition();
		
#endif //USE_CMU1394
	
	Close();
}

bool CGrabberCMU1394::OpenGrabber( void )
{
#ifdef USE_CMU1394

	int error( 0 );

	//----------

	int numOfCameraAvailables = camera.RefreshCameraList();
	if( numOfCameraAvailables == 0 ) return false;

	error = camera.SelectCamera( 0 );
	if( error != CAM_SUCCESS ) return false;

	error = camera.InitCamera( TRUE );
	if( error != CAM_SUCCESS ) return false;

	int format( 0 );
	int mode( 0 );

	int nWidth = get_Width();
	int nHeight = get_Height();
	if( nWidth == 640 && nHeight == 480 ) {
		format = 0;
		mode = 4;
	} else if( nWidth == 800 && nHeight == 600 ) {
		format = 1;
		mode = 1;
	} else if( nWidth == 1024 && nHeight == 768 ) {
		format = 1;
		mode = 4;
	} else if( nWidth == 1280 && nHeight == 960 ) {
		format = 2;
		mode = 1;
	} else if( nWidth == 1600 && nHeight == 1200 ) {
		format = 2;
		mode = 4;
	} else return false;
	error = camera.SetVideoFormat( format );
	if( error != CAM_SUCCESS ) return false;
	error = camera.SetVideoMode( mode );
	if( error != CAM_SUCCESS ) return false;

	unsigned long w, h;
	camera.GetVideoFrameDimensions( &w, &h );
	if( ( int )w != nWidth || ( int )h != nHeight ) return false;

	error = camera.SetVideoFrameRate( frame_rate );
	if( error != CAM_SUCCESS ) return false;

	/*
	unsigned long ulFlags( 0 );
	ulFlags |= ACQ_START_VIDEO_STREAM;		// useContinuous
	ulFlags |= ACQ_SUBSCRIBE_ONLY;			// subscribeOnly
	ulFlags |= ACQ_ALLOW_PGR_DUAL_PACKET;	// dualPacketSupport
	error = camera.StartImageAcquisitionEx( 6, 500, ulFlags );
	*/
	// in ACER_TravelMate_370 computer the commented code above does not work, so, it was replaced by the following code line
	error = camera.StartImageAcquisition();
	if( error != CAM_SUCCESS ) return false;

	return true;

#else //USE_CMU1394
	
	return false;

#endif //USE_CMU1394
}

bool CGrabberCMU1394::CloseGrabber( void )
{
#ifdef USE_CMU1394

	int error;
	error = camera.StopImageAcquisition();
	if( error != CAM_SUCCESS ) return false;

	return true;

#else //USE_CMU1394

	return false;

#endif //USE_CMU1394

}

bool CGrabberCMU1394::get_Frame( void )
{
#ifdef USE_CMU1394

	/*
	int nDroppedFrames;
	camera.AcquireImageEx( TRUE, &nDroppedFrames );
	*/
	// in ACER_TravelMate_370 computer the commented code above does not work, so, it was replaced by the following code line
	return ( camera.AcquireImage() == CAM_SUCCESS );
#else
	return false;
#endif //USE_CMU1394
}

bool CGrabberCMU1394::get_Image( cv::Mat *mat_ptr )
{
#ifdef USE_CMU1394
	if( camera.getRGB( pBitmap, lengthOfBitmap ) != CAM_SUCCESS )
		return false;

	matImage_Aux.data = pBitmap;
	cv::cvtColor( matImage_Aux, matImage.Obj, CV_RGB2BGR );

	matImage.Obj.copyTo( *mat_ptr );
#endif //USE_CMU1394
	
	return true;
}
