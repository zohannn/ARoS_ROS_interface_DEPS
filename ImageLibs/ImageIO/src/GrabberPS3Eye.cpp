#include "GrabberPS3Eye.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "EnvironmentVars.h"
#include "Files_Directories.h"

CGrabberPS3Eye::CGrabberPS3Eye( int width, int height, bool colour )
: CGrabber( width, height )
, ps3eye_cam( nullptr )
#ifdef USE_CLEye
, ps3eye_mode( colour ? CLEYE_COLOR_PROCESSED : CLEYE_MONO_PROCESSED )
, ps3eye_resolution( CLEYE_VGA )
#else
, ps3eye_mode( 0 )
, ps3eye_resolution( 0 )
#endif
{
	// Define properties for this grabber

	// Allows video parameters control?
	bControlParameters.set( true );

	// Which parameters are controlable?
	bControl_Exposure.set( true );
	bControl_Gain.set( true );
	bControl_WhiteBalance.set( true );
	bControl_WB_Blue.set( true );
	bControl_WB_Green.set( true );
	bControl_WB_Red.set( true );
	bControl_Flip_Horizontal.set( true );
	bControl_Flip_Vertical.set( true );
}

CGrabberPS3Eye::~CGrabberPS3Eye()
{
	Close();
}

bool CGrabberPS3Eye::ValidMode( CGrabberMode mode )
{
	if( mode.equal_to( 640, 480, 75.0f ) ) return true;
	if( mode.equal_to( 640, 480, 60.0f ) ) return true;
	if( mode.equal_to( 640, 480, 50.0f ) ) return true;
	if( mode.equal_to( 640, 480, 40.0f ) ) return true;
	if( mode.equal_to( 640, 480, 30.0f ) ) return true;
	if( mode.equal_to( 640, 480, 15.0f ) ) return true;
	if( mode.equal_to( 320, 240, 125.0f ) ) return true;
	if( mode.equal_to( 320, 240, 100.0f ) ) return true;
	if( mode.equal_to( 320, 240, 75.0f ) ) return true;
	if( mode.equal_to( 320, 240, 60.0f ) ) return true;
	if( mode.equal_to( 320, 240, 30.0f ) ) return true;
	if( mode.equal_to( 320, 240, 15.0f ) ) return true;

	return false;
}

bool CGrabberPS3Eye::OpenGrabber( void )
{
#ifndef USE_CLEye
	return false;
#else
	if( !is_ModeValid() )
		return false;

	// Test if there are cameras connected
	if( CLEyeGetCameraCount() < 1 )
		return false;

	// Configure resolution
	int mode_width = Mode.get_Width();

	if( mode_width == 640 )
		ps3eye_resolution = CLEYE_VGA;
	else if( mode_width == 320 )
		ps3eye_resolution = CLEYE_QVGA;
	else
		return false;

	// Get GUID of first camera
	GUID ps3eye_GUID = CLEyeGetCameraUUID( 0 );

	// Create camera instance
	ps3eye_cam = CLEyeCreateCamera( ps3eye_GUID, ps3eye_mode, ps3eye_resolution, Mode.get_FrameRate() );
	
	if( ps3eye_cam == nullptr )
		return false;
	
	int 
		width = 0,
		height = 0,
		img_type = 0;

	// Get camera frame dimensions
	if( !CLEyeCameraGetFrameDimensions( ps3eye_cam, width, height ) )
		return false;

	Width.set( width );
	Height.set( height );

	if( !matImage.Obj.empty() )
		matImage.Obj.release();

	// Depending on colour mode chosen, create the appropriate OpenCV image
	switch( ps3eye_mode )
	{
	case CLEYE_COLOR_PROCESSED:
	case CLEYE_COLOR_RAW:
		img_type = CV_8UC4;
		break;

	case CLEYE_MONO_PROCESSED:
	case CLEYE_MONO_RAW:
	default:
		img_type = CV_8UC1;
		break;
	}

	matImage.Obj.create( height, width, img_type );
	matImage.Obj.setTo( 0 );

	LoadDefaultVideoParameters();

	// Flip the image horizontally to appear correct.
	set_FlipHorizontal( true );

	// Start capturing
	return CLEyeCameraStart( ps3eye_cam );
#endif //USE_CLEye
}

bool CGrabberPS3Eye::CloseGrabber( void )
{
#ifdef USE_CLEye
	if( ps3eye_cam != nullptr )
	{
		// Stop camera capture
		if( !CLEyeCameraStop( ps3eye_cam ) ) return false;

		// Destroy camera object
		if( !CLEyeDestroyCamera( ps3eye_cam ) ) return false;
	}
#endif

	// if object destruction was successful, make it null
	ps3eye_cam = nullptr;
	return true;
}

inline bool CGrabberPS3Eye::get_Frame( void )
{
	matImage.lock();
	bool bSuccess = get_Frame_toPointer( &matImage.Obj );
	matImage.unlock();
	return bSuccess;
}

bool CGrabberPS3Eye::get_Image( cv::Mat * mat_ptr )
{
	bool bSuccess = false;

	matImage.lock();

	if( !matImage.Obj.empty() && !mat_ptr->empty() )
	{
		int
			channels_src = matImage.Obj.channels(),
			channels_dst = mat_ptr->channels();

		if( channels_src == channels_dst )
		{
			matImage.Obj.copyTo( *mat_ptr );
		}
		else if( ( channels_src == 4 ) && ( channels_dst == 3 ) )
		{
			cv::cvtColor( matImage.Obj, *mat_ptr, 
				#if( ( CV_VERSION_MAJOR>=3 ) && !defined( CV_VERSION_EPOCH ) )
				cv::COLOR_BGRA2BGR
				#else
				CV_BGRA2BGR 
				#endif
				);
		}

		bSuccess = true;
	}
	
	matImage.unlock();

	return bSuccess;
}

inline bool CGrabberPS3Eye::get_Frame_toPointer( cv::Mat * mat_ptr )
{
	bool bSuccess = false;

#ifdef USE_CLEye
	if( ( ps3eye_cam != nullptr ) && ( mat_ptr != nullptr ) )
		bSuccess = CLEyeCameraGetFrame( ps3eye_cam, mat_ptr->data );
#endif

	return bSuccess;
}

void CGrabberPS3Eye::LoadDefaultVideoParameters( void )
{
	std::vector< std::string > 
		vDir,
		vVar;

	vDir.emplace_back( get_Env_Var_ANSI( "VISIONBOT_DIR" ) );
	vDir.emplace_back( IMAGE_IO_DIR );
	vDir.emplace_back( "ImageIO" );
	
	vVar.emplace_back( "/Config/Video/" );
	vVar.emplace_back( "/config/PS3Eye" );

	std::string file_path;

	if( Find_File( "Video_Parameters_PS3Eye.ini", file_path, vDir, vVar ) )
	{
		Load_parameters_from_file( file_path );
	}
}

bool CGrabberPS3Eye::set_CameraLED( bool on )
{
#ifdef USE_CLEye
	if( ps3eye_cam != nullptr )
		return CLEyeCameraLED( ps3eye_cam, on );
#endif
	
	return false;
}

int CGrabberPS3Eye::NormalizeVideoParameter( PS3EYE_CAM_PARAMETER param, int value )
{
	float 
		fNorm = 1.0f,
		fOffset = 0.0f,
		fVal = static_cast<float>( value );

#ifdef USE_CLEye
	switch( param )
	{
	// [0, 79]
	case CLEYE_GAIN:
		fNorm = 100.0f/79.0f;
		break;

	// [0, 511]
	case CLEYE_EXPOSURE:
		fNorm = 100.0f/511.0f;
		break;

	// [0, 255]
	case CLEYE_WHITEBALANCE_RED:
	case CLEYE_WHITEBALANCE_GREEN:
	case CLEYE_WHITEBALANCE_BLUE:
		fNorm = 100.0f/255.0f;
		break;

	// [-500, 500]
	case CLEYE_HKEYSTONE:
	case CLEYE_VKEYSTONE:
	case CLEYE_XOFFSET:
	case CLEYE_YOFFSET:
	case CLEYE_ROTATION:
	case CLEYE_ZOOM:
	case CLEYE_LENSCORRECTION1:
	case CLEYE_LENSCORRECTION2:
	case CLEYE_LENSCORRECTION3:
	case CLEYE_LENSBRIGHTNESS:
		fNorm = 0.1f;
		fOffset = 500.0f;
		break;
	}
#endif // USE_CLEye

	return static_cast<int>( ( fVal + fOffset ) * fNorm );
}
	
int CGrabberPS3Eye::FitVideoParameterToDevice( PS3EYE_CAM_PARAMETER param, int value )
{
	float 
		fNorm = 1.0f,
		fOffset = 0.0f,
		fVal = static_cast<float>( value );

#ifdef USE_CLEye
	switch( param )
	{
	// [0, 79]
	case CLEYE_GAIN:
		fNorm = 0.79f;
		break;

	// [0, 511]
	case CLEYE_EXPOSURE:
		fNorm = 5.11f;
		break;

	// [0, 255]
	case CLEYE_WHITEBALANCE_RED:
	case CLEYE_WHITEBALANCE_GREEN:
	case CLEYE_WHITEBALANCE_BLUE:
		fNorm = 2.55f;
		break;

	// [-500, 500]
	case CLEYE_HKEYSTONE:
	case CLEYE_VKEYSTONE:
	case CLEYE_XOFFSET:
	case CLEYE_YOFFSET:
	case CLEYE_ROTATION:
	case CLEYE_ZOOM:
	case CLEYE_LENSCORRECTION1:
	case CLEYE_LENSCORRECTION2:
	case CLEYE_LENSCORRECTION3:
	case CLEYE_LENSBRIGHTNESS:
		fNorm = 10.0f;
		fOffset = -500.0f;
		break;
	}
#endif // USE_CLEye

	return static_cast<int>( fVal * fNorm + fOffset );
}

//---------------------------------------------------------------------------

void CGrabberPS3Eye::set_Camera_Video_Parameter( PS3EYE_CAM_PARAMETER param, int value )
{
#ifdef USE_CLEye
	int val = FitVideoParameterToDevice( param, value );
	set_Camera_Parameter( param, val );
#endif
}

int CGrabberPS3Eye::get_Camera_Video_Parameter( PS3EYE_CAM_PARAMETER param )
{
#ifdef USE_CLEye
	int val = get_Camera_Parameter( param );
	return NormalizeVideoParameter( param, val );
#else
	return 0;
#endif
}

//---------------------------------------------------------------------------

int CGrabberPS3Eye::get_Camera_Parameter( PS3EYE_CAM_PARAMETER par )
{
#ifdef USE_CLEye
	if( ps3eye_cam == nullptr )
		return 0;
	else
		return CLEyeGetCameraParameter( ps3eye_cam, par );
#else
	return 0;
#endif
}

void CGrabberPS3Eye::set_Camera_Parameter( PS3EYE_CAM_PARAMETER par, int val )
{
#ifdef USE_CLEye
	if( ps3eye_cam == nullptr ) return;

	CLEyeSetCameraParameter( ps3eye_cam, par, val );
#endif
}

//---------------------------------------------------------------------------

void CGrabberPS3Eye::set_Device_Exposure( int exp )
{
#ifdef USE_CLEye
	set_Camera_Video_Parameter( CLEYE_EXPOSURE, exp );
#endif
}

void CGrabberPS3Eye::set_Device_Exposure_Auto( bool autoexp )
{
#ifdef USE_CLEye
	set_Camera_Parameter( CLEYE_AUTO_EXPOSURE, autoexp ? 1 : 0 );
#endif
}

void CGrabberPS3Eye::set_Device_Gain(  int gain )
{
#ifdef USE_CLEye
	set_Camera_Video_Parameter( CLEYE_GAIN, gain );
#endif
}

void CGrabberPS3Eye::set_Device_Gain_Auto( bool autogain )
{
#ifdef USE_CLEye
	set_Camera_Parameter( CLEYE_AUTO_GAIN, autogain ? 1 : 0 );
#endif
}

void CGrabberPS3Eye::set_Device_WhiteBalance_Red( int red )
{
#ifdef USE_CLEye
	set_Camera_Video_Parameter( CLEYE_WHITEBALANCE_RED, red );
#endif
}

void CGrabberPS3Eye::set_Device_WhiteBalance_Green( int green )
{
#ifdef USE_CLEye
	set_Camera_Video_Parameter( CLEYE_WHITEBALANCE_GREEN, green );
#endif
}

void CGrabberPS3Eye::set_Device_WhiteBalance_Blue( int blue )
{
#ifdef USE_CLEye
	set_Camera_Video_Parameter( CLEYE_WHITEBALANCE_BLUE, blue );
#endif
}

void CGrabberPS3Eye::set_Device_WhiteBalance_Auto( bool autobalance )
{
#ifdef USE_CLEye
	set_Camera_Parameter( CLEYE_AUTO_WHITEBALANCE, autobalance ? 1 : 0 );
#endif
}

void CGrabberPS3Eye::set_Device_FlipHorizontal( bool flip )
{
#ifdef USE_CLEye
	set_Camera_Parameter( CLEYE_HFLIP, flip ? 1 : 0 );
#endif
}

void CGrabberPS3Eye::set_Device_FlipVertical( bool flip )
{
#ifdef USE_CLEye
	set_Camera_Parameter( CLEYE_VFLIP, flip ? 1 : 0 );
#endif
}

int CGrabberPS3Eye::get_Device_Exposure( void )
{
#ifdef USE_CLEye
	return get_Camera_Video_Parameter( CLEYE_EXPOSURE );
#else
	return 0;
#endif
}

bool CGrabberPS3Eye::get_Device_Exposure_Auto( void )
{
#ifdef USE_CLEye
	return ( get_Camera_Parameter( CLEYE_AUTO_EXPOSURE ) == 1 );
#else
	return false;
#endif
}

int CGrabberPS3Eye::get_Device_Gain( void )
{
#ifdef USE_CLEye
	return get_Camera_Video_Parameter( CLEYE_GAIN );
#else
	return 0;
#endif
}

bool CGrabberPS3Eye::get_Device_Gain_Auto( void )
{
#ifdef USE_CLEye
	return ( get_Camera_Parameter( CLEYE_AUTO_GAIN ) == 1 );
#else
	return false;
#endif
}

int CGrabberPS3Eye::get_Device_WhiteBalance_Red( void )
{
#ifdef USE_CLEye
	return get_Camera_Video_Parameter( CLEYE_WHITEBALANCE_RED );
#else
	return 0;
#endif
}

int CGrabberPS3Eye::get_Device_WhiteBalance_Green( void )
{
#ifdef USE_CLEye
	return get_Camera_Video_Parameter( CLEYE_WHITEBALANCE_GREEN );
#else
	return 0;
#endif
}

int CGrabberPS3Eye::get_Device_WhiteBalance_Blue( void )
{
#ifdef USE_CLEye
	return get_Camera_Video_Parameter( CLEYE_WHITEBALANCE_BLUE );
#else
	return 0;
#endif
}

bool CGrabberPS3Eye::get_Device_WhiteBalance_Auto( void )
{
#ifdef USE_CLEye
	return ( get_Camera_Parameter( CLEYE_AUTO_WHITEBALANCE ) == 1 );
#else
	return false;
#endif

}

bool CGrabberPS3Eye::get_Device_FlipHorizontal( void )
{
#ifdef USE_CLEye
	return ( get_Camera_Parameter( CLEYE_HFLIP ) == 1 );
#else
	return false;
#endif
}

bool CGrabberPS3Eye::get_Device_FlipVertical( void )
{
#ifdef USE_CLEye
	return ( get_Camera_Parameter( CLEYE_VFLIP ) == 1 );
#else
	return false;
#endif
}
