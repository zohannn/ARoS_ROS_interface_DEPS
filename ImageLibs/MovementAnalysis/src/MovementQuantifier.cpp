#include "MovementQuantifier.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageOperations.h"

CMovementQuantifier::CMovementQuantifier( unsigned int frame_count )
: strWindowName_Movement( "Movement" )
, strWindowName_Stereo( "Spatial Segmentation" )
, iNextFrameDiff( 0 )
, iFrameCurrent( 1 )
, iFramePrevious( 0 )
, bShowImage_Movement( false )
, bShowImage_Stereo( false )
, bWindowCreated_Movement( false )
, bWindowCreated_Stereo( false )
, bReady( false )
, uFrameCount( frame_count )
, ptrSegmentedImage_Movement( nullptr )
, ptrSegmentedImage_Stereo( nullptr )
{
}

CMovementQuantifier::CMovementQuantifier( const CMovementQuantifier &obj )
{
	Clear();

	strWindowName_Movement = obj.strWindowName_Movement;
	strWindowName_Stereo = obj.strWindowName_Stereo;
	iNextFrameDiff = obj.iNextFrameDiff;
	iFrameCurrent = obj.iFrameCurrent;
	iFramePrevious = obj.iFramePrevious;
	uFrameCount = obj.uFrameCount;

	CAttachMutex_n<bool> * pFlag = ( (CAttachMutex_n<bool> *) &obj.bShowImage_Movement );
	bShowImage_Movement.set( pFlag->get() );

	pFlag = ( (CAttachMutex_n<bool> *) &obj.bShowImage_Stereo );
	bShowImage_Stereo.set( pFlag->get() );

	pFlag = ( (CAttachMutex_n<bool> *) &obj.bWindowCreated_Movement );
	bWindowCreated_Movement.set( pFlag->get() );

	pFlag = ( (CAttachMutex_n<bool> *) &obj.bWindowCreated_Stereo );
	bWindowCreated_Stereo.set( pFlag->get() );

	uFrameCount = obj.uFrameCount;
	ptrSegmentedImage_Movement = obj.ptrSegmentedImage_Movement;
	ptrSegmentedImage_Stereo = obj.ptrSegmentedImage_Stereo;

	mat_ImageGray.create( obj.mat_ImageGray.rows, obj.mat_ImageGray.cols, obj.mat_ImageGray.type() );
	mat_ImageGray.setTo( 0 );
	
	mat_Image.create( obj.mat_Image.rows, obj.mat_Image.cols, obj.mat_Image.type() );
	mat_Image.setTo( 0 );

	vFrame.assign( obj.vFrame.begin(), obj.vFrame.end() );
	vFrameDiff.assign( obj.vFrameDiff.begin(), obj.vFrameDiff.end() );

	vUserRoI.assign( obj.vUserRoI.begin(), obj.vUserRoI.end() );

	bReady.set( true );
}

CMovementQuantifier::~CMovementQuantifier( void )
{
	Clear();
}

void CMovementQuantifier::Setup( int width, int height, unsigned int frame_count )
{
	if( bShowImage_Movement.get() ) 
		Create_Window_Movement();
	
	if( bShowImage_Stereo.get() )
		Create_Window_Stereo();

	const int type = CV_8UC1;

	const cv::Size size( width, height );
	
	uFrameCount = frame_count;

	if( !mat_ImageGray.empty() )
		mat_ImageGray.release();

	mat_ImageGray.create( size, type );
	mat_ImageGray.setTo( 0 );

	if( !mat_Image.empty() )
		mat_Image.release();

	mat_Image.create( size, CV_8UC3 );
	mat_Image.setTo( 0 );

	// Initialization with a for instead of assigning directly with assign is to prevent
	// that all images have the same pointer, creating a new object in every cycle
	for( unsigned int i = 0; i < 2; i++ )
		vFrame.emplace_back( cv::Mat( size, type, cv::Scalar::all( 0.0 ) ) );

	for( unsigned int i = 0; i < uFrameCount; i++ )
		vFrameDiff.emplace_back( cv::Mat( size, type, cv::Scalar::all( 0.0 ) ) );

	vUserRoI.assign( uFrameCount, cv::Rect( 0, 0, 0, 0 ) );

	bReady.set( true );
}

void CMovementQuantifier::Clear( void )
{
	bReady.set( false );

	if( !mat_ImageGray.empty() )
		mat_ImageGray.release();

	if( !mat_Image.empty() )
		mat_Image.release();

	size_t 
		i = 0,
		num_frames = vFrame.size();

	for( i=0 ; i<num_frames ; i++ )
	{
		if( !vFrame.at( i ).empty() )
			vFrame.at( i ).release();
	}
	
	num_frames = vFrameDiff.size();
	for( i=0 ; i<num_frames ; i++ )
	{
		if( !vFrameDiff.at( i ).empty() )
			vFrameDiff.at( i ).release();
	}

	vFrame.clear();

	vFrameDiff.clear();
	vUserRoI.clear();

	ptrSegmentedImage_Movement = nullptr;
	ptrSegmentedImage_Stereo = nullptr;

	Destroy_Window_Movement();
	Destroy_Window_Stereo();
}

void CMovementQuantifier::Create_Window_Movement( void )
{
	if( !bWindowCreated_Movement.get() )
	{
		cv::namedWindow( strWindowName_Movement );
		bWindowCreated_Movement.set( true );
	}
}

void CMovementQuantifier::Create_Window_Stereo( void )
{
	if( !bWindowCreated_Stereo.get() )
	{
		cv::namedWindow( strWindowName_Stereo );
		bWindowCreated_Stereo.set( true );
	}
}

void CMovementQuantifier::Destroy_Window_Movement( void )
{
	if( bWindowCreated_Movement.get() )
	{
		cv::destroyWindow( strWindowName_Movement );
		bWindowCreated_Movement.set( false );
	}
}

void CMovementQuantifier::Destroy_Window_Stereo( void )
{
	if( bWindowCreated_Stereo.get() )
	{
		cv::destroyWindow( strWindowName_Stereo );
		bWindowCreated_Stereo.set( false );
	}
}

void CMovementQuantifier::set_SegmentedImage_Pointer_Movement( CSegmentedImage * pSegImg )
{
	ptrSegmentedImage_Movement = pSegImg;
}

void CMovementQuantifier::set_SegmentedImage_Pointer_Stereo( CSegmentedImage * pSegImg )
{
	ptrSegmentedImage_Stereo = pSegImg;
}

bool CMovementQuantifier::isReady( void )
{
	return bReady.get();
}

void CMovementQuantifier::set_Display_Image_Movement( bool state )
{
	if( state && !bWindowCreated_Movement.get() )
		Create_Window_Movement();

	else if( !state && bWindowCreated_Movement.get() )
		Destroy_Window_Movement();

	bShowImage_Movement.set( state );
}

void CMovementQuantifier::set_Display_Image_Stereo( bool state )
{
	if( state && !bWindowCreated_Stereo.get() )
		Create_Window_Stereo();

	else if( !state && bWindowCreated_Stereo.get() )
		Destroy_Window_Stereo();

	bShowImage_Stereo.set( state );
}

bool CMovementQuantifier::get_Display_Image_Movement( void )
{
	return bShowImage_Movement.get();
}

bool CMovementQuantifier::get_Display_Image_Stereo( void )
{
	return bShowImage_Stereo.get();
}

void CMovementQuantifier::Process_Frame( cv::Mat * mat_ptr )
{
	if( mat_ptr == nullptr ) return;

	mat_ptr->copyTo( mat_Image );

	Perform_MultiFrame_Difference();

	Estimate_User_Movement();

	Cycle_Frames_Index();

	////-------------------------------------------------------------------------
	//// Display images
	
	if( bShowImage_Movement.get() && bWindowCreated_Movement.get() )
	{
		cv::imshow( strWindowName_Movement, mat_ImageGray );
	}

	if( bShowImage_Stereo.get() && bWindowCreated_Stereo.get() && ( ptrSegmentedImage_Stereo != nullptr ) )
	{
		cv::imshow( strWindowName_Stereo, ptrSegmentedImage_Stereo->Image );
	}
}

void CMovementQuantifier::Cycle_Frames_Index( void )
{
	// Toggle index of current and previous frame
	iFrameCurrent = !iFrameCurrent;
	iFramePrevious = !iFramePrevious;

	// Index where the next frame difference will be stored
	unsigned int iFrame = iNextFrameDiff + 1;

	// If index reaches the end of vector, point to the beginning
	iNextFrameDiff = ( iFrame < vFrameDiff.size() ) ? iFrame : 0;
}

void CMovementQuantifier::Perform_1xFrame_Difference( void )
{
	cv::cvtColor( mat_Image, vFrame.at( iFrameCurrent ), CV_BGR2GRAY );

	cv::subtract( vFrame.at( iFrameCurrent ), vFrame.at( iFramePrevious ), mat_ImageGray );
	
	cv::threshold( mat_ImageGray, mat_ImageGray, 20.0, 255.0, CV_THRESH_BINARY );
}

void CMovementQuantifier::Perform_MultiFrame_Difference( void )
{
	// Convert current frame to grayscale
	cv::cvtColor( mat_Image, vFrame.at( iFrameCurrent ), CV_BGR2GRAY );

	// Compute frame difference
	cv::subtract( vFrame.at( iFrameCurrent ), vFrame.at( iFramePrevious ), vFrameDiff.at( iNextFrameDiff ) );

	// Copy the first frame difference to image that will store the accumulate of frames
	// This clears the image for the 'or' operation and processes the first frame, all in one.
	size_t size = vFrameDiff.size();

	if( size < 2 )
	{
		if( size > 0 )
			vFrameDiff.front().copyTo( mat_ImageGray );
	}
	else
	{
		// 'or' first and second frame difference
		cv::bitwise_or( vFrameDiff.at( 0 ), vFrameDiff.at( 1 ), mat_ImageGray );

		// Perform an 'or' operation using all of the frame differences
		size_t num_frames = vFrameDiff.size();
		for( size_t i=2 ; i<num_frames ; i++ )
		{
			cv::bitwise_or( vFrameDiff.at( i ), mat_ImageGray, mat_ImageGray );
		}
	}

	if( ptrSegmentedImage_Movement != nullptr )
	{
		cv::threshold( mat_ImageGray, ptrSegmentedImage_Movement->Image, 20.0, 255.0, CV_THRESH_BINARY );

		ptrSegmentedImage_Movement->doMorphologicalOperations();

		ptrSegmentedImage_Movement->Image.copyTo( mat_ImageGray );
	}
	else
	{
		cv::threshold( mat_ImageGray, mat_ImageGray, 20.0, 255.0, CV_THRESH_BINARY );
	}
}

cv::Rect CMovementQuantifier::Estimate_User_Region( void )
{
	//-----------------------------------------------------------------------
	// Find the region of the user in the image

	// Create a copy of the stereo segmented image to be used in the findContours function

	if( ptrSegmentedImage_Stereo == nullptr ) return cv::Rect( 0, 0, 0, 0 );

	cv::Size matSize( ptrSegmentedImage_Stereo->Image.cols, ptrSegmentedImage_Stereo->Image.rows );

	ptrSegmentedImage_Stereo->doMorphologicalOperations();
	
	cv::RotatedRect bounding_rect_user = get_Region_All_Points_Rotated( &ptrSegmentedImage_Stereo->Image );

	vUserRoI.at( iNextFrameDiff ) = bounding_rect_user.boundingRect();

	cv::Rect max_rect( matSize.width/2, matSize.height/2, 0, 0 );

	size_t num_roi = vUserRoI.size();
	for( size_t i=0 ; i<num_roi ; i++ )
	{
		if( vUserRoI.at( i ).area() <= 0 ) continue;

		if( max_rect.area() <= 0 )
		{
			max_rect = vUserRoI.at( i );
		}
		else
		{
			max_rect = MaxRect( max_rect, vUserRoI.at( i ) );
		}
	}

	// Validate region to fit inside the image
	cv::Rect rect_limits( 0, 0, matSize.width, matSize.height );
	FitRegionToLimits( rect_limits, &max_rect );	

	return max_rect;
}

void CMovementQuantifier::Estimate_User_Movement( void )
{
	// Find the region of the user in the image
	cv::Rect roi_user = Estimate_User_Region();

	float 
		fAreaMovement = 0.0f,
		fAreaUser = static_cast<float>( roi_user.area() ),
		fRatio = -1.0f;

	// Find the movement area in the image
	if( fAreaUser > 1.0f )
	{
		cv::Mat mat_img_aux( mat_ImageGray, roi_user );
	
		// The amount of pixels that aren't zero
		fAreaMovement = static_cast<float>( cv::countNonZero( mat_ImageGray( roi_user ) ) );
		fRatio = fAreaMovement / fAreaUser;
	}

	fMovement.set( fRatio );
	
	//-----------------------------------------------------------------------
	// Visual feedback of the movement classification
	/*std::stringstream str_ratio( "" );
	str_ratio << fRatio;
	
	cv::putText( mat_ImageGray, str_ratio.str(), cv::Point( 0, 100 ), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar::all( 255.0 ), 3 );
	cv::rectangle( mat_ImageGray, roi_user, cv::Scalar::all( 255.0 ) );*/
}
