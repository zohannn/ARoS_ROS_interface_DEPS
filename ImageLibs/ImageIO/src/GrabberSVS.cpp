#include "GrabberSVS.h"

CGrabberSVS::CGrabberSVS( int width, int height )
: CGrabber( width, height )
, Original_aux_buffer( nullptr )
{
	bHasStereo.set( true );
}

CGrabberSVS::~CGrabberSVS( void )
{
	DestroyAuxImage();
}

inline void CGrabberSVS::get_SVSImage_Pointer( STEREO_IMAGE_TYPE * svs_ptr )
{
#ifdef USE_SVS
	stereo_image.lock();
	
	// Theorically faster method using a pointer to it. It doesn't work!
	svs_ptr = &stereo_image;
	
	stereo_image.unlock();
#endif
}

inline void CGrabberSVS::get_SVSImage_Copy( STEREO_IMAGE_TYPE * svs_ptr )
{
#ifdef USE_SVS
	stereo_image.lock();
	
	//safer method for acquiring the Stereo Image, by copying it.
	svs_ptr->CopyFrom( &stereo_image );
	
	stereo_image.unlock();
#endif
}

void CGrabberSVS::CreateAuxImage( int width, int height )
{
	if( !matImage_Aux.empty() )
		matImage_Aux.release();
	
	matImage_Aux.create( height, width, CV_8UC4 ); // 4 channels because of videre images
	matImage_Aux.setTo( 0 );

	Original_aux_buffer = matImage_Aux.data;
}

void CGrabberSVS::DestroyAuxImage( void )
{
	if( !matImage_Aux.empty() )
	{
		matImage_Aux.data = Original_aux_buffer;
		matImage_Aux.release();
		Original_aux_buffer = nullptr;
	}
}

bool CGrabberSVS::get_Image( cv::Mat * mat_ptr )
{
#ifdef USE_SVS
	stereo_image.lock();

	//Do I have colour image on left camera?
	if( !stereo_image.haveColor )
	{
		stereo_image.unlock();
		return false;
	}
	
	//Take the image Data and put it into a cv::Mat
	matImage_Aux.data = (unsigned char*) stereo_image.Color();
	  	
	// Create auxiliary images to split channels
	cv::Mat 
		bgr( matImage_Aux.size(), CV_8UC3, cv::Scalar::all( 0.0 ) ),
		alpha( matImage_Aux.size(), CV_8UC1, cv::Scalar::all( 0.0 ) );

	// forming array of matrices is quite efficient operations,
	// because the matrix data is not copied, only the headers
	// rgba[0] -> bgr[2], rgba[1] -> bgr[1],
	// rgba[2] -> bgr[0], rgba[3] -> alpha[0]

	// Define output array
	cv::Mat out[] = { bgr, alpha };
	
	// array order
	int from_to[] = { 0,2,  1,1,  2,0,  3,3 };

	// Split channels and swap Red and Blue
	cv::mixChannels( &matImage_Aux, 1, out, 2, from_to, 4 );

	// Copy result to destiny
	bgr.copyTo( *mat_ptr );
	
	stereo_image.unlock();
#endif

	return true;
}
