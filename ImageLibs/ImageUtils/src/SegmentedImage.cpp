#include "SegmentedImage.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageOperations.h"
#include "MathOperations.h"
#include "vector_operations.hpp"

CSegmentedImage::CSegmentedImage( void )
{
}

CSegmentedImage::CSegmentedImage( const CSegmentedImage & image_seg )
: the_Colour( image_seg.the_Colour )
, vSpots( image_seg.vSpots.begin(), image_seg.vSpots.end() )
, vContours( image_seg.vContours.begin(), image_seg.vContours.end() )
, vHierarchy( image_seg.vHierarchy.begin(), image_seg.vHierarchy.end() )
, vSpots_bkp( image_seg.vSpots_bkp.begin(), image_seg.vSpots_bkp.end() )
, vSpotsIn( image_seg.vSpotsIn.begin(), image_seg.vSpotsIn.end() )
{
	if( !image_seg.Image.empty() )
	{
		Image.release();
		Image.create( cv::Size( image_seg.Image.cols, image_seg.Image.rows ), image_seg.Image.type() );
		image_seg.Image.copyTo( Image );
	}
	
	const int n = sizeof( image_seg.uHue ) / sizeof( image_seg.uHue[0] );
	
	for( unsigned int i=0 ; i<n ; i++ )
	{
		uHue[i].set( const_cast< CAttachMutexPtr_n<unsigned int>& >( image_seg.uHue[i] ).get() );
		uSat[i].set( const_cast< CAttachMutexPtr_n<unsigned int>& >( image_seg.uSat[i] ).get() );
		uVal[i].set( const_cast< CAttachMutexPtr_n<unsigned int>& >( image_seg.uVal[i] ).get() );
		uMorph[i].set( const_cast< CAttachMutexPtr_n<unsigned int>& >( image_seg.uMorph[i] ).get() );
	}

	uMorph[n].set( const_cast< CAttachMutexPtr_n<unsigned int>& >( image_seg.uMorph[n] ).get() );
}

CSegmentedImage::~CSegmentedImage( void )
{
}

void CSegmentedImage::Setup( int width, int height, int nColour )
{
	if( !Image.empty() )
		Image.release();

	//-----------------------------------------------------------------------
	// Instantiate images

	Image.create( height, width, CV_8UC1 );
	Image.setTo( 0 );

	// Initialize colour
	the_Colour = nColour;
	
	//-----------------------------------------------------------------------
	// Prepare synchronism objects
	for( unsigned int i=0 ; i<2 ; i++ )
	{
		uHue[i].alloc();
		uHue[i].set( 0 );

		uSat[i].alloc();
		uSat[i].set( 0 );

		uVal[i].alloc();
		uVal[i].set( 0 );

		uMorph[i].alloc();
		uMorph[i].set( 0 );
	}
	
	uMorph[2].alloc();
	uMorph[2].set( 0 );
}

void CSegmentedImage::Clean( void )
{
	if( !Image.empty() )
		Image.release();

	//-----------------------------------------------------------------------
	// Clean synchronism objects
	for( unsigned int i=0 ; i<2 ; i++ )
	{
		uHue[i].free();
		uSat[i].free();
		uVal[i].free();
		uMorph[i].free();
	}
	
	uMorph[2].free();

	vContours.clear();
	vHierarchy.clear();
	vSpots.clear();
	vSpotsIn.clear();
	vSpots_bkp.clear();
}

bool CSegmentedImage::Parse_Parameters_Load( void )
{
	bool bSuccess = true;

	yarp::os::Bottle bot = prop_ConfigFile.findGroup( the_Colour.sName.c_str() ).findGroup( "Hue" );
	
	if( bot.size() > 0 )
	{
		uHue[Seg::MIN].set( u_round( bot.get( 1 ).asInt() / 2.0f ) );
		uHue[Seg::MAX].set( u_round( bot.get( 2 ).asInt() / 2.0f ) );
	}
	else bSuccess = false;

	bot = prop_ConfigFile.findGroup( the_Colour.sName.c_str() ).findGroup( "Saturation" );

	if( bot.size() > 0 )
	{
		uSat[Seg::MIN].set( u_round( bot.get( 1 ).asInt() * 2.55f ) );
		uSat[Seg::MAX].set( u_round( bot.get( 2 ).asInt() * 2.55f ) );
	}
	else bSuccess = false;

	bot = prop_ConfigFile.findGroup( the_Colour.sName.c_str() ).findGroup( "Value" );
	
	if( bot.size() > 0 )
	{
		uVal[Seg::MIN].set( u_round( bot.get( 1 ).asInt() * 2.55f ) );
		uVal[Seg::MAX].set( u_round( bot.get( 2 ).asInt() * 2.55f ) );
	}
	else bSuccess = false;

	bot = prop_ConfigFile.findGroup( the_Colour.sName.c_str() ).findGroup( "Morph" );
	
	if( bot.size() > 0 )
	{
		std::string str = bot.get( 1 ).asString();

		if( str == "open" ) uMorph[0].set( MORPH_OPERATION_OPEN );
		else if( str == "close" ) uMorph[0].set( MORPH_OPERATION_CLOSE );
		else uMorph[Seg::OP].set( 0 );

		uMorph[Seg::ERODE].set( static_cast<unsigned int>( bot.get( 2 ).asInt() ) );
		uMorph[Seg::DILATE].set( static_cast<unsigned int>( bot.get( 3 ).asInt() ) );
	}
	else bSuccess = false;

	return bSuccess;
}

void CSegmentedImage::DetectSpots_FloodFill( bool bAppend )
{
	unsigned char pixel = 0;

	cv::Scalar 
		new_colour( 254.0 ),
		diff_lower( 0.0 ),
		diff_upper( 0.0 );

	cv::Rect rect( 0, 0, 0, 0 );
	cv::Point pt( 0, 0 );

	if( !bAppend ) vSpots.clear();

	for( pt.x=0 ; pt.x<Image.cols ; pt.x++ )	// Here I can use Leap-Frog algorithm
	{
		for( pt.y=0 ; pt.y<Image.rows ; pt.y++ ) // Here I can use Leap-Frog algorithm
		{
			pixel = Image.at<unsigned char>( pt );

			if( pixel == 255 )
			{
				cv::floodFill( 
					Image,		// The image
					pt,			// Starting point
					new_colour,	// Colour to repaint
					&rect,		// Rectangle where roi will be returned
					diff_lower,	// Lower tolerance
					diff_upper,	// Upper tolerance
					8 );		// Pixel connectivity
					
				// Save object properties
				vSpots.emplace_back( rect );
			}
		}
	}

	// Backup the spots
	Spots_Store();
}

void CSegmentedImage::DetectSpots_Contours( bool bAppend )
{
	if( !bAppend ) 
	{
		vSpots.clear();
		vSpotsIn.clear();
		vSpots_bkp.clear();
		
		vContours.clear();
		vHierarchy.clear();
	}

	// Ensure the image has contents
	if( Image.empty() )	return;

	// Use a copy from the original segmented image to prevent that cv::findContours changes it.
	cv::Mat img_aux( Image.rows, Image.cols, Image.type() );
	Image.copyTo( img_aux );

	cv::findContours(
		img_aux,
		vContours,
		vHierarchy,
		CV_RETR_TREE,
		CV_CHAIN_APPROX_SIMPLE );

	img_aux.release();

	if( vContours.size() < 1 ) return;

	int iContour = 0;

	cv::Rect rect( 0, 0, 0, 0 );

	// Create spots with external contours only
	do
	{
		rect = cv::boundingRect( cv::Mat( vContours.at( iContour ) ) );

		if( ( rect.x >= 0 ) && ( rect.y >= 0 ) )
			vSpots.emplace_back( rect );

		iContour = vHierarchy.at( iContour )[0]; // Next contour in the same level

	}while( iContour >= 0 ); // When there are no more contours, iContour is supposed to be '-1'
}

void CSegmentedImage::doMorphologicalOperations( void )
{
	morphology( Image, Image, uMorph[Seg::ERODE].get(), uMorph[Seg::DILATE].get(), uMorph[Seg::OP].get() );
}

void CSegmentedImage::Spots_Store( void )
{
	vector_copy( vSpots, vSpots_bkp );
}

void CSegmentedImage::Spots_Retrieve( void )
{
	vector_copy( vSpots_bkp, vSpots );
}
