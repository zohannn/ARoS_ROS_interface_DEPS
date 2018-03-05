#include "ProcessImage.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ColourOperations.h"
#include "EnvironmentVars.h"
#include "Files_Directories.h"
#include "MutexUtils.hpp"
#include "Object_T.h"
#include "range.hpp"
#include "RectEx.hpp"
#include "RectOperations.hpp"
#include "vector_operations.hpp"

CProcessImage::CProcessImage( int width, int height )
: segmented( static_cast<unsigned char>( 255 ) )
, nProcessImageID( ProcessImageID::PI_BASE )
, bFeedback( false )
, bImageProcessing( true )
, bFrameSaving( false )
, bFeedbackWindowsReady( false )
, bDisplayImage( true )
, bUseBodyMovement( false )
, bUseHandTracker( false )
, bPrintHandMovement( false )
, bPrintBodyMovement( false )
, pDataSource( nullptr )
, uFrame( 0 )
, uDir( 0 )
, bAcquireHand( false )
, nObjectArea_Min( 0 )
, nObjectArea_Max( width*height )
, strWindowName( "Image from Grabber" )
, sBasePath_FrameSaving( "C:/Work/Frames/Seq_" )
, sPath_FrameSaving( "C:/Work/Frames/Seq_" )
, sSegmentationSearchDir( "/Config/Segmentation/" )
, BodyMovementQuantifier()
, HandTracker( OBJECT_HAND )
, vSegValues( NUMBER_OF_COLOURS, std::vector<unsigned int>( 9, 0 ) )
{
	ptMousePos.lock();
	ptMousePos.x = 0;	// X position of the mouse over the image
	ptMousePos.y = 0;	// Y position of the mouse over the image
	ptMousePos.z = 0;	// Used to flag the desired text to appear on the status bar ( 0: default, 1: custom text )
	ptMousePos.unlock();

	if( get_ImageDisplayStatus() ) 
		cv::namedWindow( strWindowName );
	
#if SHOW_CANNY_IMAGE
	cv::namedWindow( "Canny" );
#endif
	
#if SHOW_DEBUG_IMAGE
	cv::namedWindow( DEBUG_WINDOW_NAME );
#endif
}

CProcessImage::~CProcessImage( void )
{
	Clean();
}

void CProcessImage::SetupProcessing( int width, int height )
{
	nWidth = width;
	nHeight = height;

	SegmentationValues_Store();

	vSegmentedImage.lock();

	vSegmentedImage.clear();

	vSegmentedImage.assign( NUMBER_OF_COLOURS, CSegmentedImage() );

	size_t nColours = vSegmentedImage.size();
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		vSegmentedImage.at( iColour ).Setup( nWidth, nHeight, static_cast<int>( iColour ) );
	}

	vSegmentedImage.unlock();
	
	SegmentationValues_Load();

	//--------------------------------------------------------------------------
	// Allocate memory to the images
	
	if( !mat_ImageProcessed.Obj.empty() ) mat_ImageProcessed.Obj.release();
	if( !mat_ImageHSV.empty() ) mat_ImageHSV.release();

	cv::Size size( width, height );

	mat_ImageProcessed.Obj.create( size, CV_8UC3 );
	mat_ImageProcessed.Obj.setTo( 0 );

	mat_ImageHSV.create( size, CV_8UC3 );
	mat_ImageHSV.setTo( 0 );

	// Auxiliary images to be used on Overlap checking
	int nImages = sizeof( aImg_gray_aux ) / sizeof( cv::Mat );
	for( int i = 0; i < nImages; i++ )
	{
		aImg_gray_aux[i].create( size, CV_8UC1 );
		aImg_gray_aux[i].setTo( 0 );
	}
	//--------------------------------------------------------------------------

	std::vector< std::string >
		vDir,
		vVar;

	vDir.emplace_back( get_Env_Var_ANSI( "VISIONBOT_DIR" ) );
	vVar.emplace_back( sSegmentationSearchDir );

	std::string file_path = "";

	if( Find_File( sSegFile, file_path, vDir, vVar ) )
	{
		LoadSegmentationParameters( file_path );
	}

	SegmentationValues_Store();

	uFrame = 0;

	bUseBodyMovement.set( false );
	bUseHandTracker.set( false );

	if( pDataSource != nullptr )
	{
		if( pDataSource->get_UseBodyMovement() )
		{
			bUseBodyMovement.set( true );

			BodyMovementQuantifier.Setup( nWidth, nHeight );
		}

		if( pDataSource->get_UseHandTracker() )
		{
			bUseHandTracker.set( true );

			HandTracker.Setup( nWidth, nHeight );
		}

		SetupProcessing_TaskSpecific( nWidth, nHeight );

		if( BodyMovementQuantifier.isReady() )
		{
			BodyMovementQuantifier.set_SegmentedImage_Pointer_Movement( &vSegmentedImage.at( COLOUR_W ) );
			BodyMovementQuantifier.set_SegmentedImage_Pointer_Stereo( &vSegmentedImage.at( COLOUR_K ) );
		}
	}
}

void CProcessImage::Clean( void )
{
	vSegmentedImage.lock();

	size_t nColours = vSegmentedImage.size();
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		vSegmentedImage.at( iColour ).Clean();
	}

	vSegmentedImage.clear();

	vSegmentedImage.unlock();
	
	//--------------------------------------------------------------------------
	// Release memory to the images
	
	if( !mat_ImageProcessed.Obj.empty() )
		mat_ImageProcessed.Obj.release();

	if( !mat_ImageSource.Obj.empty() )
		mat_ImageSource.Obj.release();

	if( !mat_ImageHSV.empty() )
		mat_ImageHSV.release();

	if( !mat_ImageGray.empty() )
		mat_ImageGray.release();

	int nImages = sizeof( aImg_gray_aux ) / sizeof( cv::Mat );
	for( int i=0 ; i<nImages ; i++ )
	{
		if( !aImg_gray_aux[i].empty() )
			aImg_gray_aux[i].release();
	}

	Clean_TaskSpecific();

	if( BodyMovementQuantifier.isReady() )
	{
		BodyMovementQuantifier.Clear();
	}
}

void CProcessImage::doImageProcessing( bool doProc )
{
	bImageProcessing.set( doProc );
}

bool CProcessImage::useImageProcessing( void )
{
	return bImageProcessing.get();
}

void CProcessImage::set_DataSource( CData * pData )
{
	pDataSource = pData;
}

void CProcessImage::Process_Movement( void )
{
	if( BodyMovementQuantifier.isReady() )
	{
		BodyMovementQuantifier.Process_Frame( &mat_ImageSource.Obj );

		pDataSource->HumanInfo.ClassifyBodyMovement( BodyMovementQuantifier.fMovement.get() );
	}
}

void CProcessImage::SegmentationValues_Store( void )
{
	vSegmentedImage.lock();

	size_t size = vSegmentedImage.size();
	for( size_t i=0 ; i<size ; i++ )
	{
		vSegValues.at( i ).at( 0 ) = vSegmentedImage.at( i ).uHue[Seg::MIN].get();
		vSegValues.at( i ).at( 1 ) = vSegmentedImage.at( i ).uHue[Seg::MAX].get();
		vSegValues.at( i ).at( 2 ) = vSegmentedImage.at( i ).uSat[Seg::MIN].get();
		vSegValues.at( i ).at( 3 ) = vSegmentedImage.at( i ).uSat[Seg::MAX].get();
		vSegValues.at( i ).at( 4 ) = vSegmentedImage.at( i ).uVal[Seg::MIN].get();
		vSegValues.at( i ).at( 5 ) = vSegmentedImage.at( i ).uVal[Seg::MAX].get();
		vSegValues.at( i ).at( 6 ) = vSegmentedImage.at( i ).uMorph[Seg::OP].get();
		vSegValues.at( i ).at( 7 ) = vSegmentedImage.at( i ).uMorph[Seg::ERODE].get();
		vSegValues.at( i ).at( 8 ) = vSegmentedImage.at( i ).uMorph[Seg::DILATE].get();
	}

	vSegmentedImage.unlock();
}

void CProcessImage::SegmentationValues_Load( void )
{
	vSegmentedImage.lock();

	size_t size = vSegmentedImage.size();
	for( size_t i=0 ; i<size ; i++ )
	{
		vSegmentedImage.at( i ).uHue[Seg::MIN].set( vSegValues.at( i ).at( 0 ) );
		vSegmentedImage.at( i ).uHue[Seg::MAX].set( vSegValues.at( i ).at( 1 ) );
		vSegmentedImage.at( i ).uSat[Seg::MIN].set( vSegValues.at( i ).at( 2 ) );
		vSegmentedImage.at( i ).uSat[Seg::MAX].set( vSegValues.at( i ).at( 3 ) );
		vSegmentedImage.at( i ).uVal[Seg::MIN].set( vSegValues.at( i ).at( 4 ) );
		vSegmentedImage.at( i ).uVal[Seg::MAX].set( vSegValues.at( i ).at( 5 ) );
		vSegmentedImage.at( i ).uMorph[Seg::OP].set( vSegValues.at( i ).at( 6 ) );
		vSegmentedImage.at( i ).uMorph[Seg::ERODE].set( vSegValues.at( i ).at( 7 ) );
		vSegmentedImage.at( i ).uMorph[Seg::DILATE].set( vSegValues.at( i ).at( 8 ) );
	}

	vSegmentedImage.unlock();
}

void CProcessImage::get_SourceImage( cv::Mat * mat_ptr )
{
	mat_ImageSource.lock();
	mat_ImageSource.Obj.copyTo( *mat_ptr );
	mat_ImageSource.unlock();
}	

void CProcessImage::get_ProcessedImage( cv::Mat * mat_ptr )
{
	mat_ImageProcessed.lock();
	mat_ImageProcessed.Obj.copyTo( *mat_ptr );
	mat_ImageProcessed.unlock();
}

void CProcessImage::SaveProcessedImageToFile( std::string file_name )
{
	mat_ImageProcessed.lock();
	cv::imwrite( file_name, mat_ImageProcessed.Obj );
	mat_ImageProcessed.unlock();
}

void CProcessImage::SaveSourceImageToFile( std::string file_name )
{
	mat_ImageSource.lock();
	cv::imwrite( file_name, mat_ImageSource.Obj );
	mat_ImageSource.unlock();
}

void CProcessImage::SaveResizedImageToFile( std::string file_name )
{
	mat_ImageSource.lock();
	
	cv::Mat mat_resized( mat_ImageSource.Obj.rows/2, mat_ImageSource.Obj.cols/2, mat_ImageSource.Obj.type() );
	cv::resize( mat_ImageSource.Obj, mat_resized, mat_resized.size(), 0, 0, cv::INTER_CUBIC );
	
	cv::imwrite( file_name, mat_resized );
	mat_resized.release();

	mat_ImageSource.unlock();
}

void CProcessImage::ShowSourceImageOnCVWindow( std::string strName )
{
	mat_ImageSource.lock();
	cv::imshow( strName, mat_ImageSource.Obj );
	mat_ImageSource.unlock();

	// Display on status bar information about current pixel
	Display_Status_Bar_Information();
}

void CProcessImage::ShowProcessedImageOnCVWindow( std::string strName )
{
	mat_ImageProcessed.lock();
	cv::imshow( strName, mat_ImageProcessed.Obj );
	mat_ImageProcessed.unlock();

	// Display on status bar information about current pixel
	Display_Status_Bar_Information();
}

bool CProcessImage::isFeedbackActive( void )
{
	return bFeedback.get();
}

void CProcessImage::set_ImageDisplayStatus( bool bStatus )
{
	bDisplayImage.set( bStatus );
	
	if( bStatus ) cv::namedWindow( strWindowName );
	else cv::destroyWindow( strWindowName );
}

bool CProcessImage::get_ImageDisplayStatus( void )
{
	return bDisplayImage.get();
}

void CProcessImage::Display_Processed_Image( void )
{
	if( !bDisplayImage.get() ) return;
	
	if( bImageProcessing.get() )
	{
		std::stringstream strmText( "" );
	
		cv::Scalar text_colour( 255.0, 255.0, 255.0 );

		float fVal = 0.0f;

		int text_line = 450;

		if( bPrintBodyMovement.get() && BodyMovementQuantifier.isReady() )
		{
			strmText.str( "" );
			strmText.clear();
			strmText.precision( 4 );

			fVal = pDataSource->HumanInfo.BodyMovement.get();

			strmText << "Body: " << fVal;

			cv::putText( mat_ImageProcessed.Obj, strmText.str(), cv::Point( 0, text_line ), cv::FONT_HERSHEY_DUPLEX, 1.0, text_colour );

			text_line -= 50;
		}

		if( bPrintHandMovement.get() && HandTracker.isReady() )
		{
			strmText.str( "" );
			strmText.clear();
			strmText.precision( 4 );

			fVal = pDataSource->HumanInfo.HandMovement.get();
			
			strmText << "Hand: " << fVal;

			cv::putText( mat_ImageProcessed.Obj, strmText.str(), cv::Point( 0, text_line ), cv::FONT_HERSHEY_DUPLEX, 1.0, text_colour );
		}
	}

	ShowProcessedImageOnCVWindow( strWindowName );
}

void CProcessImage::Display_Status_Bar_Information( void )
{
#ifdef USE_OPENCV_QT
	ptMousePos.lock();
	cv::Point pt( ptMousePos.x, ptMousePos.y );
	int mode = ptMousePos.z;
	ptMousePos.unlock();

	if( mode == 0 ) return;

	// Lock image access
	mat_ImageSource.lock();
	
	// Get the clicked pixel
	Pixel3 pix_src = mat_ImageSource.Obj.at< Pixel3 >( pt );
	
	// Unlock image access
	mat_ImageSource.unlock();

	// Convert pixel from BGR to HSV
	Pixel3 pix_dst = convert_Pixel( pix_src, CV_BGR2HSV );

	std::stringstream strmText( "" );

	// Apply corrections to conversion to normalize the HSV values
	// Format text to be shown in status bar
	strmText << 
		"(x=" << pt.x << ", y=" << pt.y << ") ~ " <<
		"R:" << static_cast<unsigned int>( pix_src.z ) << " " <<
		"G:" << static_cast<unsigned int>( pix_src.y ) << " " <<
		"B:" << static_cast<unsigned int>( pix_src.x ) << " <=> " <<
		"H:" << static_cast<unsigned int>( pix_dst.x * 2.0f ) <<  " " <<
		"S:" << static_cast<unsigned int>( pix_dst.y / 2.55f ) <<  " " <<
		"V:" << static_cast<unsigned int>( pix_dst.z / 2.55f );

	// Display the text
	cv::displayStatusBar( strWindowName, strmText.str(), 0 );
#endif
}

std::string CProcessImage::get_MainWindowName( void )
{
	return strWindowName;
}

int CProcessImage::get_ProcessImageID( void )
{
	return nProcessImageID;
}

void CProcessImage::LoadSegmentationParameters( std::string file )
{
	size_t nColours = vSegmentedImage.size();
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
		vSegmentedImage.at( iColour ).Load_parameters_from_file( file );
}

void CProcessImage::SaveSegmentationParameters( std::string file )
{
	const float conv = 100.0f/255.0f;
	
	unsigned int op = 0;

	std::string str_morph = "";

	std::ofstream stream_file( file, std::fstream::out );

	if( !stream_file.is_open() ) return;

	stream_file << "#Segmentation parameters" << std::endl << std::endl;

	size_t nColours = vSegmentedImage.size();
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		op = vSegmentedImage.at( iColour ).uMorph[Seg::OP].get();

		switch( op )
		{
		case MORPH_OPERATION_CLOSE:
			str_morph = "close ";
		    break;

		default:
		case MORPH_OPERATION_OPEN:
			str_morph = "open ";
			break;
		}

		stream_file << "[" << vSegmentedImage.at( iColour ).the_Colour.sName << "]" << std::endl <<
			"Hue " <<
			vSegmentedImage.at( iColour ).uHue[Seg::MIN].get()*2 << " " <<
			vSegmentedImage.at( iColour ).uHue[Seg::MAX].get()*2 << std::endl <<
			"Saturation " <<
			static_cast<unsigned int>( vSegmentedImage.at( iColour ).uSat[Seg::MIN].get()*conv + 0.5f ) << " " << 
			static_cast<unsigned int>( vSegmentedImage.at( iColour ).uSat[Seg::MAX].get()*conv + 0.5f ) << std::endl <<
			"Value " <<
			static_cast<unsigned int>( vSegmentedImage.at( iColour ).uVal[Seg::MIN].get()*conv + 0.5f ) << " " << 
			static_cast<unsigned int>( vSegmentedImage.at( iColour ).uVal[Seg::MAX].get()*conv + 0.5f ) << std::endl <<
			"Morph " <<
			str_morph <<
			vSegmentedImage.at( iColour ).uMorph[Seg::ERODE].get() << " " <<
			vSegmentedImage.at( iColour ).uMorph[Seg::DILATE].get() << std::endl << std::endl;
	}
	
	stream_file.close();
}

void CProcessImage::SegmentColour( int nColour )
{
	std::pair< Pixel3, Pixel3 > limit;

	limit.first.x = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uHue[Seg::MIN].get() );
	limit.second.x = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uHue[Seg::MAX].get() );
	limit.first.y = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uSat[Seg::MIN].get() );
	limit.second.y = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uSat[Seg::MAX].get() ),
	limit.first.z = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uVal[Seg::MIN].get() );
	limit.second.z = static_cast<unsigned char>( vSegmentedImage.at( nColour ).uVal[Seg::MAX].get() );

	cv::Size size = vSegmentedImage.at( nColour ).Image.size();
	
	int 
		nRow = 0, 
		nCol = 0,
		type = CV_MAKETYPE( vSegmentedImage.at( nColour ).Image.depth(), vSegmentedImage.at( nColour ).Image.channels() );

	vSegmentedImage.at( nColour ).Image.zeros( size, type );
	
	Pixel3 pixel( 0, 0, 0 );
	
	for( nRow=0 ; nRow<mat_ImageHSV.rows ; nRow++ )
	{
		for( nCol=0 ; nCol<mat_ImageHSV.cols ; nCol++ )
		{
			pixel = mat_ImageHSV.at<Pixel3>( nRow, nCol );

			if( in_range( pixel, limit) )
			{
				vSegmentedImage.at( nColour ).Image.at<unsigned char>( nRow, nCol ) = segmented;
			}
		}
	}
}

void CProcessImage::SegmentAllColours( void )
{
	// Vector of pairs (min,max) where 'min' and 'max' contain the HSV components to be tested in x,y,z variables
	std::vector< Pixel_Pair >
		limit( NUMBER_OF_COLOURS, Pixel_Pair( Pixel3( 0, 0, 0 ), Pixel3( 0, 0, 0 ) ) );

	cv::Size size( 0, 0 );
	int type = 0;

	size_t nColours = vSegmentedImage.size();
	for( size_t i=0 ; i<nColours ; i++ )
	{
		vSegmentedImage.at( i ).Image.setTo( 0 );

		limit.at( i ).first.x = static_cast<unsigned char>( vSegmentedImage.at( i ).uHue[Seg::MIN].get() );
		limit.at( i ).second.x = static_cast<unsigned char>( vSegmentedImage.at( i ).uHue[Seg::MAX].get() );
		limit.at( i ).first.y = static_cast<unsigned char>( vSegmentedImage.at( i ).uSat[Seg::MIN].get() );
		limit.at( i ).second.y = static_cast<unsigned char>( vSegmentedImage.at( i ).uSat[Seg::MAX].get() );
		limit.at( i ).first.z = static_cast<unsigned char>( vSegmentedImage.at( i ).uVal[Seg::MIN].get() );
		limit.at( i ).second.z = static_cast<unsigned char>( vSegmentedImage.at( i ).uVal[Seg::MAX].get() );
	}

	Pixel3 pixel( 0, 0, 0 );

	for( int nRow=0 ; nRow<mat_ImageHSV.rows ; nRow++ )
	{
		for( int nCol=0 ; nCol<mat_ImageHSV.cols ; nCol++ )
		{
			pixel = mat_ImageHSV.at<Pixel3>( nRow, nCol );
			
			bool bColourFound = false;
			int iColour = 0;
			
			while( ( iColour < NUMBER_OF_COLOURS ) && !bColourFound )
			{
				if( in_range( pixel, limit.at( iColour) ) )
				{
					vSegmentedImage.at( iColour ).Image.at<unsigned char>( nRow, nCol ) = segmented;
					bColourFound = true;
				}
			
				iColour++;
			}
		}
	}
}

void CProcessImage::SegmentColour_cv( int iColour )
{
	cv::Scalar 
		lower = cv::Scalar( 
			vSegmentedImage.at( iColour ).uHue[Seg::MIN].get(),
			vSegmentedImage.at( iColour ).uSat[Seg::MIN].get(), 
			vSegmentedImage.at( iColour ).uVal[Seg::MIN].get() ),
		// Add up 1.0 to take into account the way cv::inRange works, min <= val < max
		upper = cv::Scalar( 
			vSegmentedImage.at( iColour ).uHue[Seg::MAX].get() + 1.0, 
			vSegmentedImage.at( iColour ).uSat[Seg::MAX].get() + 1.0, 
			vSegmentedImage.at( iColour ).uVal[Seg::MAX].get() + 1.0 );

	cv::inRange( mat_ImageHSV, lower, upper, vSegmentedImage.at( iColour ).Image );
}

void CProcessImage::SegmentMainColours_cv( void )
{
	for( int iColour=0 ; iColour<COLOUR_W ; iColour++ )
	{
		SegmentColour_cv( iColour );
	}
}

void CProcessImage::SegmentAllColours_cv( void )
{
	size_t nColours = vSegmentedImage.size();
	for( register size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		SegmentColour_cv( static_cast<int>( iColour ) );
	}
}

bool CProcessImage::get_SegmentationWindowsStatus( void )
{
	return bFeedbackWindowsReady.get();
}

void CProcessImage::set_SegmentationWindowsStatus( bool bStatus )
{
	bFeedbackWindowsReady.set( bStatus );
}

void CProcessImage::CreateWindows( void )
{
	if( !bFeedback.get() ) return;
	
	if( get_SegmentationWindowsStatus() ) return;

	bFeedback.lock();

	size_t 
		nW = static_cast<size_t>( nWidth/2 ),
		nH = static_cast<size_t>( nHeight/2 ),
		pos_x = 0, 
		pos_y = 0,
		inc_x = 15, 
		inc_y = 34;

#ifdef USE_OPENCV_QT
	// Compensate the extra size of the Qt windows
	inc_x += 73;
	inc_y += 60;
#endif

	size_t 
		nIndex = 0,
		nColours = vSegmentedImage.size();

	std::string strName = "";

	for( int i=0 ; i<3 ; i++ )
	{
		for( int j=0 ; j<3 ; j++ )
		{
			nIndex = i*3+j;
			if( nIndex >= nColours ) break;

			strName = vSegmentedImage.at( nIndex ).the_Colour.sName;

			// Create window
			cv::namedWindow( strName, 0 );
			
			// Force the size
			cv::resizeWindow( strName, nW, nH );
			
			// Move it to it's place
			cv::moveWindow( strName, pos_x, pos_y );
			
			pos_y += ( nH + inc_y );
		}

		pos_y = 0;
		pos_x += ( nW + inc_x );
	}
	
	strName.clear();

	set_SegmentationWindowsStatus( true );

	bFeedback.unlock();
}

void CProcessImage::DestroyWindows( void )
{
	if( !bFeedback.get() ) return;

	if( !get_SegmentationWindowsStatus() ) return;

	bFeedback.lock();

	size_t nColours = vSegmentedImage.size();
	for( size_t i=0 ; i<nColours ; i++ )
		cv::destroyWindow( vSegmentedImage.at( i ).the_Colour.sName );

	set_SegmentationWindowsStatus( false );
	
	bFeedback.unlock();
}

void CProcessImage::ShowSegmentationFeedback( void )
{
	bFeedback.set( true );
	CreateWindows();
}

void CProcessImage::HideSegmentationFeedback( void )
{
	DestroyWindows();
	bFeedback.set( false );
}

void CProcessImage::ShowSegmentedImages( void )
{
	if( !bFeedback.get() ) return;

	if( !get_SegmentationWindowsStatus() ) return;

	bFeedback.lock();

	size_t nColours = vSegmentedImage.size();
	for( size_t i=0 ; i<nColours ; i++ )
	{
		cv::imshow( vSegmentedImage.at( i ).the_Colour.sName, vSegmentedImage.at( i ).Image );
	}

	bFeedback.unlock();
}

void CProcessImage::DetectAllColourSpots( void )
{
	size_t nColours = vSegmentedImage.size();
	for( size_t i=0 ; i<nColours ; i++ )
	{
		vSegmentedImage.at( i ).DetectSpots_Contours();
	}
}

void CProcessImage::DetectMainColourSpots( void )
{
	for( int i=0 ; i<=COLOUR_Y ; i++ )
	{
		vSegmentedImage.at( i ).DetectSpots_Contours();
	}
}

bool CProcessImage::CheckOverlapSpot( cv::Mat * img1, cv::Rect * spot1, cv::Mat * img2, cv::Rect * spot2 )
{
	if( ( img1 == nullptr ) || ( img2 == nullptr ) || ( spot1 == nullptr ) || ( spot2 == nullptr ) ) return false;

	// Calculate the number of images in the array 'aImg_gray_aux'
	int nImages = sizeof( aImg_gray_aux ) / sizeof( cv::Mat );

	// if less than 3 images, exit
	if( nImages < 3 ) return false;

	// Set the images to zero
	for( int i=0 ; i<nImages ; i++ )
	{
		if( aImg_gray_aux[i].empty() ) return false;
		aImg_gray_aux[i].setTo( 0 );
	}
	
	(*img1)( *spot1 ).copyTo( aImg_gray_aux[0]( *spot1 ) );

	(*img2)( *spot2 ).copyTo( aImg_gray_aux[1]( *spot2 ) );

	// This way, the following operations exclude non interest regions
	cv::Rect region = MaxRect( *spot1, *spot2 );
	
	// This will tell were are overlapped regions on the segmented image
	cv::bitwise_and( aImg_gray_aux[0]( region ), aImg_gray_aux[1]( region ), aImg_gray_aux[2]( region ) );

	// Counts the common pixels, resulting from 'And' operation
	int nPixels = cv::countNonZero( aImg_gray_aux[2]( region ) );

	//If there are non zero pixels, this means the spots are overlapped
	return ( nPixels > 0 );
}

bool CProcessImage::Associate_Colour_to_Object( int nColour, int nObject, unsigned int nMinArea, unsigned int nMaxArea )
{
	unsigned int nObjArea = 0;
	pDataSource->vObjects.Obj.at( nObject ).clear();

	size_t n = vSegmentedImage.at( nColour ).vSpots.size();

	if(nObject==OBJECT_HAND && n>1)
		int a=1;

	for( size_t i=0 ; i<n ; i++ )	
	{
		nObjArea = static_cast<unsigned int>( vSegmentedImage.at( nColour ).vSpots.at( i ).area() );
		if( ( nObjArea >= nMinArea ) && ( nObjArea <= nMaxArea ) )
		{
			pDataSource->vObjects.Obj.at( nObject ).emplace_back( CObject_f( nObject, vSegmentedImage.at( nColour ).vSpots.at( i ) ) );
			pDataSource->vObjects.Obj.at( nObject ).back().Colour_Primary = nColour;
		}
	}

	return ( pDataSource->vObjects.Obj.at( nObject ).size() > 0 );
}

bool CProcessImage::Detect_Object_From_Primary_Colour( int nObject, unsigned int nMinArea, unsigned int nMaxArea )
{
	return Associate_Colour_to_Object( GET_VALID_PrimaryColour( nObject ), nObject, nMinArea, nMaxArea );
}

void CProcessImage::Define_New_Region_of_Interest( int nObject, cv::Rect rect )
{
	cv::Rect new_roi;

	size_t obj_size = pDataSource->vObjects.Obj.at( nObject ).size();
	for( size_t i=0 ; i<obj_size ; i++ )
	{
		new_roi = pDataSource->vObjects.Obj.at( nObject ).at( i ).RoI;
		
		new_roi.x += rect.x;
		new_roi.y += rect.y;
		new_roi.width += rect.width;
		new_roi.height += rect.height;

		FitRegionToLimits( cv::Rect( 0, 0, nWidth, nHeight ), &new_roi );

		pDataSource->vObjects.Obj.at( nObject ).at( i ).RoI = new_roi;
	}
}

void CProcessImage::Generate_Object_Masks( void )
{
	size_t 
		iObjType = 0,
		iObj = 0,
		iSubObj = 0,
		num_Objects = 0,
		num_SubObjects = 0;

	CObject_f obj_base( OBJECT_BASE );

	int Base_MinArea = static_cast<int>( obj_base.uMinArea );

	for( iObjType=0 ; iObjType<NUMBER_OF_OBJECT_TYPES ; iObjType++ )
	{
		num_Objects = pDataSource->vObjects.Obj.at( iObjType ).size();
		for( iObj=0 ; iObj<num_Objects ; iObj++ )
		{
			CObject_f * pObj = &pDataSource->vObjects.Obj.at( iObjType ).at( iObj );

			// Generate mask for object from corresponding binary image
			Generate_Mask_for_Object( pObj );

			// Generate mask for eventual sub objects
			num_SubObjects = pObj->vSubObjects.size();
			for( iSubObj=0 ; iSubObj<num_SubObjects ; iSubObj++ )
			{
				Generate_Mask_for_Object( &pObj->vSubObjects.at( iSubObj ) );
			}
		}
	}
}

void CProcessImage::Generate_Mask_for_Object( CObject_f * pObj )
{
	// If the mask contains something, clean it
	if( !pObj->imgMask.empty() ) pObj->imgMask.release();
			
	// Get the region were the spot is
	// Use the RoI instead of Region
	cv::Rect RoI = pObj->RoI;

	// Allocate memory for mask
	pObj->imgMask.create( RoI.height, RoI.width, CV_8UC1 );
	// Initialize the mask to '0'
	pObj->imgMask.setTo( 0 );
			
	// What is the object's primary colour?
	int 
		iColour = pObj->Colour_Primary.nColour_ID,
		iSecColour = pObj->Colour_Secondary.nColour_ID;

	switch( pObj->get_Type() )
	{
	case OBJECT_COLUMN_1:
	case OBJECT_COLUMN_2:
	case OBJECT_COLUMN_3:
	case OBJECT_COLUMN_4:
		// Create a mask that takes into account sub objects and main colour
		// performing an 'or' between both colours
		cv::bitwise_or(
			vSegmentedImage.at( iColour ).Image( RoI ),
			vSegmentedImage.at( iSecColour ).Image( RoI ),
			pObj->imgMask );
		break;

	default:
	// Copy the region were the spot is on the segmented image' main colour to the mask
		vSegmentedImage.at( iColour ).Image( RoI ).copyTo( pObj->imgMask );
		break;
	}

	// Erode the mask just a little to reduce the effect of outter pixels that don't belong to the object
	cv::erode( 
		pObj->imgMask,
		pObj->imgMask, 
		cv::Mat(), 
		cv::Point( -1, -1 ), 
		2 );
}

void CProcessImage::LookForNearObjects( int nObjectTypeSrc, int nObjectTypeDst )
{
	bool 
		isSrcBase = ( nObjectTypeSrc == OBJECT_BASE ),
		isSrcHand = ( nObjectTypeSrc == OBJECT_HAND ),
		isSrcWheelDstNut = ( nObjectTypeSrc == OBJECT_WHEEL ) && ( nObjectTypeDst == OBJECT_NUT );

	cv::Rect 
		rect_src, 
		rect_dst;
	
	size_t
		obj_src_size = pDataSource->vObjects.Obj.at( nObjectTypeSrc ).size(),
		obj_dst_size = 0;

	for( size_t iSrc=0 ; iSrc<obj_src_size ; iSrc++ )
	{
		rect_src = pDataSource->vObjects.Obj.at( nObjectTypeSrc ).at( iSrc ).RoI;
		obj_dst_size = pDataSource->vObjects.Obj.at( nObjectTypeDst ).size();

		for( size_t iDst=0 ; iDst<obj_dst_size ; iDst++ )
		{
			rect_dst = pDataSource->vObjects.Obj.at( nObjectTypeDst ).at( iDst ).RoI;
			
			bool bOverlap = CheckOverlapRegion( &rect_src, &rect_dst );

			//check to see if there is a common region between the two objects
			if( bOverlap )
			{
				// An object is inserted in the base
				// A nut is inserted in a wheel
				if( isSrcBase || isSrcWheelDstNut )
				{
					// Mark the destination object as inserted
					pDataSource->vObjects.Obj.at( nObjectTypeDst ).at( iDst ).bInserted = true;
					
					// Set the type of object where it is inserted
					pDataSource->vObjects.Obj.at( nObjectTypeDst ).at( iDst ).vInsertedAt.emplace_back( nObjectTypeSrc );
				}

				// Add an object of type 'nObjectTypeDst' to the object of type 'nObjectTypeSrc'
				// If an hand is approaching an inserted object, don't count it as a near object
				if( !isSrcHand || ( isSrcHand && !pDataSource->vObjects.Obj.at( nObjectTypeDst ).at( iDst ).bInserted ) )
					pDataSource->vObjects.Obj.at( nObjectTypeSrc ).at( iSrc ).vNearObjects.emplace_back( pDataSource->vObjects.Obj.at( nObjectTypeDst ).at( iDst ) );
			}
		}
	}
}

void CProcessImage::MarkFoundObjects( bool bText, cv::Mat * img )
{
	if( img == nullptr )
		img = &mat_ImageProcessed.Obj;

	size_t
		i = 0, 
		j = 0, 
		k = 0, 
		size = 0, 
		size2 = 0;

	cv::Rect rect( 0, 0, 0, 0 );

	cv::Point pt_text( 0, 0 );
	
	cv::Scalar colour( 0.0 );
	
	std::stringstream sname( "" );
	
	int 
		index = 0,
		obj_id = 0;

	for( j=0 ; j<NUMBER_OF_OBJECT_TYPES ; j++ )
	{
		size = pDataSource->vObjects.Obj.at( j ).size();
		for( i=0 ; i<size ; i++ )
		{
			if( !pDataSource->vObjects.Obj.at( j ).at( i ).bVisible ) continue;

			rect = pDataSource->vObjects.Obj.at( j ).at( i ).RoI;
			colour = pDataSource->vObjects.Obj.at( j ).at( i ).Colour_Primary.get_cvScalar();
			
			pt_text.x = rect.x;
			pt_text.y = rect.y - 3;
			
			cv::rectangle( *img, rect, colour );
						
			if( bText )
			{
				obj_id = pDataSource->vObjects.Obj.at( j ).at( i ).nObjectID;

				index = ( obj_id != -1 ) ? obj_id : static_cast<int>( i+1 );

				sname.str( "" );
				sname.clear();
				sname << pDataSource->vObjects.Obj.at( j ).at( i ).get_Name() << " [" << index << "]";

				cv::putText( *img, sname.str(), pt_text, cv::FONT_HERSHEY_SIMPLEX, 0.5, colour );
			}

			if( j != OBJECT_BASE ) continue;

			size2 = pDataSource->vObjects.Obj.at( j ).at( i ).vSubObjects.size();

			for( k=0 ; k<size2 ; k++ )
			{
				rect = pDataSource->vObjects.Obj.at( j ).at( i ).vSubObjects.at( k ).RoI;
				colour = pDataSource->vObjects.Obj.at( j ).at( i ).vSubObjects.at( k ).Colour_Primary.get_cvScalar();
				
				pt_text = cvPoint( rect.x, rect.y-3 );
				
				cv::rectangle( *img, rect, colour );
			}
		}
	}
}

void CProcessImage::set_HandAcquisition( bool bAcquire )
{
	if( bAcquire )
	{
		if( !bAcquireHand.get() )
		{
			uFrame = 0;
			bAcquireHand.set( true );
		}
	}

	else
	{
		bAcquireHand.set( false );
	}
}

bool CProcessImage::get_HandAcquisition( void )
{
	return bAcquireHand.get();
}

void CProcessImage::SaveHandImage( void )
{
	if( !get_HandAcquisition() ) return;
	
	size_t num_hands = pDataSource->vObjects.Obj.at( OBJECT_HAND ).size();
	cv::Rect region;
	
	std::stringstream file_name;
	
	file_name << std::setw( 4 );

	mat_ImageSource.lock();

	for( size_t i=0 ; i<num_hands ; i++ )
	{
		region = pDataSource->vObjects.Obj.at( OBJECT_HAND ).at( i ).RoI;

		file_name 
			<< "C:/Work/Images/img_" 
			<< uFrame << '_' << i << ".png";
	
		cv::imwrite( file_name.str(), mat_ImageSource.Obj( region ) );
	}

	uFrame++;

	mat_ImageSource.unlock();
}

unsigned char CProcessImage::CalculateMode( unsigned char * val )
{
	if( val == nullptr ) return 0;

	unsigned char cIndex = 0;

	for( register unsigned int i=0 ; i<=255 ; i++ )
	{
		if( val[i]>val[cIndex] ) cIndex = i;
	}

	return cIndex;
}

//---------------------------------------------------------------------------
// Frame saving
void CProcessImage::FrameSaving_On( void )
{
	uFrame = 0;
	bFrameSaving.set( true );

	bFrameSaving.lock();

	std::stringstream strmDir;

	do
	{
		uDir++;
		strmDir.str( "" );
		strmDir.clear();
		strmDir << sBasePath_FrameSaving;
		strmDir << uDir;

	}while( DirectoryExists( strmDir.str() ) );

	sPath_FrameSaving = strmDir.str();

	// Create the directory
	std::string command = "md \"";
	command += sPath_FrameSaving;
	command += "\"";
	system( command.c_str() ); 

	bFrameSaving.unlock();
}

void CProcessImage::FrameSaving_Off( void )
{
	uFrame = 0;
	bFrameSaving.set( false );
}

void CProcessImage::SaveFrame( std::string ext, bool bSave_dt, double dt, bool bSave_PanTilt, float pan, float tilt )
{
	std::ostringstream file_name;
	
	// Set fixed prexision
	file_name << std::fixed;

	// Configure base file name
	file_name << sPath_FrameSaving << "/frame_" << std::setw( 5 ) << std::setfill( '0' ) << uFrame;

	// Add dt to frame name?
	if( bSave_dt )
		file_name << std::setprecision( 5 ) << "_dt" << dt;
	
	// Add Pan and Tilt to frame name
	if( bSave_PanTilt )
		file_name << std::setprecision( 1 ) << "_pan" << pan << "_tilt" << tilt;

	// Add file extension
	file_name << "." << ext;

	SaveSourceImageToFile( file_name.str() );
	uFrame++;
}

void CProcessImage::set_FrameSavingStatus( bool bSave )
{
	if( bSave ) FrameSaving_On();
	else FrameSaving_Off();
}

bool CProcessImage::get_FrameSavingStatus( void )
{
	return bFrameSaving.get();
}

void CProcessImage::ToggleFrameSaving( void )
{
	bool bStatus = get_FrameSavingStatus();

	set_FrameSavingStatus( !bStatus );
}

void CProcessImage::set_New_Image_for_Processing( cv::Mat * mat_ptr )
{
	//Take the image
	mat_ImageSource.lock();
	mat_ptr->copyTo( mat_ImageSource.Obj );

	mat_ImageProcessed.lock();
	mat_ImageSource.Obj.copyTo( mat_ImageProcessed.Obj );
	mat_ImageSource.unlock();
	mat_ImageProcessed.unlock();
}

void CProcessImage::Process( cv::Mat * mat_ptr )
{
	set_New_Image_for_Processing( mat_ptr );

	Process();
}

void CProcessImage::Process( void )
{
	ProcessTask( &mat_ImageProcessed.Obj );

	ShowSegmentedImages();
	//cv::waitKey( 1 );
}

void CProcessImage::Update_Coordinates_on_Tracked_Hands( void )
{
	if( HandTracker.isReady() )
	{
		HandTracker.Push_New_Coordinates( pDataSource->vObjects.Obj.at( OBJECT_HAND ) );

		pDataSource->HumanInfo.ClassifyHandMovement( 
			pDataSource->vObjects.Obj.at( OBJECT_HAND ).size(),
			HandTracker.get_MaxMovement() );
	}
}

void CProcessImage::Detect_Gestures( void )
{
	size_t 
		num_obj = 0,
		num_obj_prev = 0,
		num_hands = pDataSource->vObjects.Obj.at( OBJECT_HAND ).size();

	// Reset gestures and pointed objects from the previous iteration
	Gesture.Reset();
	pDataSource->vGestures.lock();
	pDataSource->vGestures.Obj.clear();
	pDataSource->vGestures.unlock();

	pDataSource->vObjPointed.lock();
	pDataSource->vObjPointed.Obj.clear();
	pDataSource->vObjPointed.unlock();

	// The code inside the 'if' segment sets up the objects that will be processed for the pointing
	// if pointing detection is disabled there is no need to execute this
	if (num_hands > 0 && Gesture.bDetect_Pointing.get())
	{
		size_t num_CSG_Objects = sizeof(CSG_Objects) / sizeof(unsigned int);

		Gesture.vObjects.lock();

		Gesture.vObjects.Obj.clear();

		cv::Rect objRegion(0, 0, 0, 0);

		for (size_t i = 0; i < num_CSG_Objects; i++)
		{
			num_obj = pDataSource->vObjects.Obj.at(CSG_Objects[i]).size();
			num_obj_prev = Gesture.vObjects.Obj.size();

			// Add to the current vector more 'num_obj' objects, of type 'CSG_Objects[i]'
			Gesture.vObjects.Obj.resize(num_obj_prev + num_obj, CObjectBasic(CSG_Objects[i]));

			for (size_t j = 0; j < num_obj; j++)
			{
				objRegion = pDataSource->vObjects.Obj.at(CSG_Objects[i]).at(j).Region;

				// Calculate the coordinate for the object center
				Gesture.vObjects.Obj.at(num_obj_prev + j).Coord_Image.x = objRegion.x + static_cast<int>(objRegion.width / 2);
				Gesture.vObjects.Obj.at(num_obj_prev + j).Coord_Image.y = objRegion.y + static_cast<int>(objRegion.height / 2);

				pDataSource->vObjects.Obj.at(CSG_Objects[i]).at(j).get_Coordinates_World(&Gesture.vObjects.Obj.at(num_obj_prev + j).Coord_World);
			}
		}

		Gesture.vObjects.unlock();
	}
	
	unsigned int gesture = 0;

	CObject_f * pHand = nullptr;

	for( size_t i = 0; i < num_hands; i++ )
	{
		pHand = &pDataSource->vObjects.Obj.at( OBJECT_HAND ).at( i );

		gesture = Gesture.detect(
			pHand->RoI,
			pHand->imgMask,
			mat_ImageProcessed.Obj,
			mat_ImageHSV( pHand->RoI ) );

		if( gesture == Gesture::POINTING )
		{
			pHand->uGestureType = gesture;
		}
		else
		{
			// Determine Hand Out gesture
			#if 0

			float hand_coord[3] = { 0.0f, 0.0f, 0.0f };
			pHand->get_Coordinates_World( hand_coord );

			// Hand position higher than 1.3m
			if( hand_coord[2] > 1300.0f && gesture == Gesture::NONE )
				pHand->uGestureType = Gesture::HAND_OUT;
			else
			#endif
			if( pHand->vNearObjects.size() > 0 )
				pHand->uGestureType = gesture;

			Gesture.mark_detected_gesture( pHand->uGestureType, pHand->RoI, mat_ImageProcessed.Obj );
		}

		pHand = nullptr;
	}

	mutex_vector_copy( Gesture.vGestures, pDataSource->vGestures );

	mutex_vector_copy( Gesture.PointedObject, pDataSource->vObjPointed );
}

bool CProcessImage::isHand_with_Gesture_Near_Object(unsigned int gesture, unsigned int object_type)
{
	size_t
		numHands = pDataSource->vObjects.Obj.at( OBJECT_HAND ).size(),
		numObjs = 0;

	CObject_f * pHand = nullptr;

	bool bHandNear = false;

	for( size_t iHand = 0; iHand < numHands; iHand++ )
	{
		pHand = &pDataSource->vObjects.Obj.at( OBJECT_HAND ).at( iHand );

		if( pHand->uGestureType == gesture )
		{
			numObjs = pHand->vNearObjects.size();

			for (size_t iObj = 0; iObj < numObjs; iObj++)
			{
				if( pHand->vNearObjects.at( iObj ).get_Type() == object_type )
					bHandNear = true;
			}
		}
	}

	return bHandNear;
}