#include "GrabberSVSVideo.h"

#include "EnvironmentVars.h"
#include "Files_Directories.h"

CGrabberSVSVideo::CGrabberSVSVideo( int width, int height )
: CGrabberSVS( width, height )
, videoObject( nullptr )
{
	// Define properties for this grabber

	// Allows video parameters control?
	bControlParameters.set( true );

	// Which parameters are controlable?
	bControl_Brightness.set( true );
	bControl_Exposure.set( true );
	bControl_Gain.set( true );
	bControl_Gamma.set( true );
	bControl_Saturation.set( true );
	bControl_WhiteBalance.set( true );
	bControl_WB_Blue.set( true );
	bControl_WB_Red.set( true );
}

CGrabberSVSVideo::~CGrabberSVSVideo()
{
	Close();
}

bool CGrabberSVSVideo::ValidMode( CGrabberMode mode )
{
	if( mode.equal_to( 640, 480, 30.0f ) ) return true;
	if( mode.equal_to( 640, 480, 15.0f ) ) return true;
	if( mode.equal_to( 640, 480, 7.5f  ) ) return true;
	if( mode.equal_to( 640, 480, 3.75f ) ) return true;

	if( mode.equal_to( 320, 240, 30.0f ) ) return true;
	if( mode.equal_to( 320, 240, 15.0f ) ) return true;
	if( mode.equal_to( 320, 240, 7.5f  ) ) return true;
	if( mode.equal_to( 320, 240, 3.75f ) ) return true;

	return false;
}

bool CGrabberSVSVideo::OpenGrabber( void )
{
#ifndef USE_SVS
	return false;
#else
	int
		width = Width.get(),
		height = Height.get();

	CreateAuxImage( width, height );

	matImage.Obj.create( height, width, CV_8UC3 );
	matImage.Obj.setTo( 0 );

	videoObject = getVideoObject();

	if( !videoObject->Open() ) return false;

	// Frame size and sampling modes can be set here
	// Each device has a default mode
	// videoObject->FrameDiv(1);			// Full frame
	videoObject->SetSize( width, height );	// if not set. 320x240 image by default.
		
	//-----------------------------------------------------------------------
	std::vector< std::string >
		vDir,
		vVar;

	vDir.emplace_back( get_Env_Var_ANSI( "VISIONBOT_DIR" ) );
	vDir.emplace_back( IMAGE_IO_DIR );
	vDir.emplace_back( "ImageIO" );

	vVar.emplace_back( "/config/Videre/Stereo" );
	vVar.emplace_back( "/Config/" );
	vVar.emplace_back( "/Stereo/" );
	vVar.emplace_back( "/Video/" );
	vVar.emplace_back( "/Config/Stereo/" );
	vVar.emplace_back( "/Config/Video/" );

	std::string 
		sFileCalib = "",
		file = "";

	if( videoObject->is_proc_capable )
	{
		file = "Videre_STOC-6cm-colour.ini";
		videoObject->SetProcMode( PROC_MODE_DISPARITY );
	}
	else
	{
		//file = "Videre_STH-DCS-VAR_C.ini";
		file = "Videre_STH-DCS-VAR_C_broad.ini";
	}

	// Load Parameters to configure SVS device
	// Sets stereo processing parameters, always set parameters after opening the device
	// These are valid only to our Videre STH-DCS-VAR-C stereo system
	if( Find_File( file, sFileCalib, vDir, vVar ) )
	{
		videoObject->ReadParams( const_cast<char*>( sFileCalib.c_str() ) );
	}

	// Set image rectification
	// NOTE: to do rectification, it has to be turned on...
	videoObject->SetRect( true );	// Here, acquisition is set up to rectify the image

	// Set up to get colour, if we can, from hardware device.
	videoObject->SetColorAlg( COLOR_ALG_BEST );

	// I only need colour on the left image.
	videoObject->SetColor( true, false );

	return videoObject->Start();
#endif
}

bool CGrabberSVSVideo::CloseGrabber( void )
{
#ifndef USE_SVS
	return true;
#else
	if( !videoObject->Stop() ) return false;
	
	if( !videoObject->Close() ) return false;
	
	closeVideoObject();
	videoObject = nullptr;

	DestroyAuxImage();
	return true;
#endif
}

//---------------------------------------------------------------------------

inline bool CGrabberSVSVideo::get_Frame( void )
{
#ifdef USE_SVS
	stereo_image.lock();

	stereo_image.CopyFrom( videoObject->GetImage( 100 ) );	//100 ms timeout

	stereo_image.unlock();
#endif

	return true;
}

//---------------------------------------------------------------------------

void CGrabberSVSVideo::set_Device_Exposure( int exp )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetExposure( exp, videoObject->gain, videoObject->use_autoexposure, videoObject->use_autogain );
#endif
}

void CGrabberSVSVideo::set_Device_Exposure_Auto( bool autoexp )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetExposure( videoObject->exposure, videoObject->gain, autoexp, videoObject->use_autogain );
#endif
}

void CGrabberSVSVideo::set_Device_Gain(  int gain )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetExposure( videoObject->exposure, gain, videoObject->use_autoexposure, videoObject->use_autogain );
#endif
}

void CGrabberSVSVideo::set_Device_Gain_Auto( bool autogain )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetExposure( videoObject->exposure, videoObject->gain, videoObject->use_autoexposure, autogain );
#endif
}

void CGrabberSVSVideo::set_Device_Brightness( int bright )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetBrightness( videoObject->use_autobrightness, bright );
#endif
}

void CGrabberSVSVideo::set_Device_Brightness_Auto( bool autobright )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetBrightness( autobright, videoObject->brightness );
#endif
}

void CGrabberSVSVideo::set_Device_WhiteBalance_Red( int red )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;

	videoObject->SetBalance( videoObject->use_autowhite, red, videoObject->blue	);
#endif
}

void CGrabberSVSVideo::set_Device_WhiteBalance_Blue( int blue )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;

	videoObject->SetBalance( videoObject->use_autowhite, videoObject->red, blue	);
#endif
}

void CGrabberSVSVideo::set_Device_WhiteBalance_Auto( bool autobalance )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;

	videoObject->SetBalance( autobalance, videoObject->red, videoObject->blue	);
#endif
}
	
void CGrabberSVSVideo::set_Device_Gamma( int gamma )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetGamma( gamma );
#endif
}

void CGrabberSVSVideo::set_Device_Saturation( int sat )
{
#ifdef USE_SVS
	if( videoObject == nullptr ) return;
	
	videoObject->SetSaturation( sat );
#endif
}

int CGrabberSVSVideo::get_Device_Exposure( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->exposure;
#endif

	return 0;
}

bool CGrabberSVSVideo::get_Device_Exposure_Auto( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->use_autoexposure;
#endif

	return false;
}

int CGrabberSVSVideo::get_Device_Gain( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->gain;
#endif

	return 0;
}
bool CGrabberSVSVideo::get_Device_Gain_Auto( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->use_autogain;
#endif

	return false;
}

int CGrabberSVSVideo::get_Device_Brightness( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->brightness;
#endif

	return 0;
}
bool CGrabberSVSVideo::get_Device_Brightness_Auto( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->use_autobrightness;
#endif

	return false;
}

int CGrabberSVSVideo::get_Device_WhiteBalance_Red( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->red;
#endif

	return 0;
}

int CGrabberSVSVideo::get_Device_WhiteBalance_Blue( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->blue;
#endif

	return 0;
}
bool CGrabberSVSVideo::get_Device_WhiteBalance_Auto( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->use_autowhite;
#endif

	return false;
}

int CGrabberSVSVideo::get_Device_Gamma( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->gamma;
#endif

	return 0;
}
int CGrabberSVSVideo::get_Device_Saturation( void )
{
#ifdef USE_SVS
	if( videoObject != nullptr ) 
		return videoObject->saturation;
#endif
	
	return 0;
}
