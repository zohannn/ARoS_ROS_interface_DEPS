#include "GrabberSVSFileImage.h"

#include <opencv2/highgui/highgui.hpp>

#include "EnvironmentVars.h"
#include "Files_Directories.h"

CGrabberSVSFileImage::CGrabberSVSFileImage( std::string file_name )
: CGrabberSVS()
{
	set_FileName( file_name );
}

CGrabberSVSFileImage::~CGrabberSVSFileImage()
{
	Close();
}

bool CGrabberSVSFileImage::ValidMode( CGrabberMode mode )
{
	return true;
}

bool CGrabberSVSFileImage::OpenGrabber( void )
{
#ifndef USE_SVS
	return false;
#else
	if( !imageObject.Open( const_cast<char*>( sFileName.c_str() ) ) ) return false;

	//-----------------------------------------------------------------------

	std::vector< std::string > 
		vDir,
		vVar;

	vDir.emplace_back( get_Env_Var_ANSI( "VISIONBOT_DIR" ) );
	vDir.emplace_back( IMAGE_IO_DIR );
	vDir.emplace_back( "ImageIO" );

	vVar.emplace_back( "/Config/Stereo/" );
	vVar.emplace_back( "/config/Videre/Stereo" );

	std::string sFileCalib = "";

	bool bSuccess = false;

	if( Find_File( "Videre_STH-DCS-VAR_C_broad.ini", sFileCalib, vDir, vVar ) )
	{
		// Load Parameters to configure SVS device
		// Sets stereo processing parameters, always set parameters after opening the device
		bSuccess = imageObject.ReadParams( const_cast<char*>( sFileCalib.c_str() ) );
		if( !bSuccess ) return false;
	
		bSuccess = imageObject.SetRect( true );
		if( !bSuccess ) return false;
	}

	if( !imageObject.Start() ) return false;

	//-----------------------------------------------------------------------
	// Open the BMP file to get width and height of the image

	cv::Mat test_img = cv::imread( sFileName, -1 );

	const int
		width = test_img.cols,
		height = test_img.rows;

	test_img.release();

	Width.set( width );
	Height.set( height );

	//-----------------------------------------------------------------------

	CreateAuxImage( width, height );

	matImage.Obj.create( height, width, CV_8UC3 );
	matImage.Obj.setTo( 0 );

	return true;
#endif
}

bool CGrabberSVSFileImage::CloseGrabber( void )
{
#ifdef USE_SVS
	if( !imageObject.Close() ) return false;
#endif

	DestroyAuxImage();
	return true;
}

bool CGrabberSVSFileImage::get_Frame( void )
{
#ifdef USE_SVS
	stereo_image.lock();

	stereo_image.CopyFrom( imageObject.GetImage( 500 ) );

	stereo_image.unlock();
#endif
	
	return true;
}
