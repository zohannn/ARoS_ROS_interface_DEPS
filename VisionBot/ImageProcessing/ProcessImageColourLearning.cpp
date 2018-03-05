#include "ProcessImageColourLearning.h"

#include <opencv2/highgui/highgui.hpp>
#include "RectOperations.hpp"

CProcessImageColourLearning::CProcessImageColourLearning( int width, int height )
: CProcessImage( width, height )
, histogram()
{
	cv::namedWindow( histogram.strWindowName_Histogram );

	sSegFile += "Videre/Segmentation_Parameters (Colour Learning).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ColourLearning;
}

CProcessImageColourLearning::~CProcessImageColourLearning( void )
{
	Clean_TaskSpecific();
}

void CProcessImageColourLearning::SetupProcessing_TaskSpecific( int width, int height )
{
	if( !img_mat_hsv.empty() )
		img_mat_hsv.release();

	img_mat_hsv.create( height, width, CV_8UC3 );
	img_mat_hsv.setTo( 0 );
}

void CProcessImageColourLearning::Clean_TaskSpecific( void )
{
	if( !img_mat_hsv.empty() )
		img_mat_hsv.release();
}

bool CProcessImageColourLearning::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Detect the objects
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_RED );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_GREEN );

	//bFound |= Detect_Object_From_Primary_Colour( OBJECT_BLUE );

	//bFound |= Detect_Object_From_Primary_Colour( OBJECT_MAGENTA );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_YELLOW );

	//bResult |= Detect_Object_From_Primary_Colour( OBJECT_ORANGE );

	bFound |= SearchObjects_Hand();

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageColourLearning::ObjectPreProcessing( void )
{
	const int NOBJECTS = 3;
	
	int
		nTaskObjects[NOBJECTS] = { OBJECT_RED, OBJECT_GREEN, OBJECT_YELLOW },
		iType = 0;

	std::vector<int> v_hist( 360, 0 );

	size_t iObj = 0;

	cv::Rect 
		rObj,
		rHand,
		Hist_RoI;

	bool bObj_Found = false;

	mat_ImageHSV.copyTo( img_mat_hsv );

	// ----------------------------------------------------------------------
	// Get the RoI of the object in the region of the hand.
	size_t
		nHands = pDataSource->vObjects.Obj.at( OBJECT_HAND ).size(),
		nObjects = 0;

	if( nHands > 0 )
	{
		rHand = pDataSource->vObjects.Obj.at( OBJECT_HAND ).front().RoI;

		for( iType=0 ; iType<NOBJECTS ; iType++ )
		{
			nObjects = pDataSource->vObjects.Obj.at( nTaskObjects[iType] ).size();

			if( nObjects <= 0 ) continue;

			for( iObj=0 ; iObj<nObjects ; iObj++ )
			{
				rObj = pDataSource->vObjects.Obj.at( nTaskObjects[iType] ).at( iObj ).Region;
				
				if( CheckOverlapRegion( &rHand, &rObj ) )
				{
					Hist_RoI = rObj;
					bObj_Found = true;
					break;
				}
			}
		}

		// Calculate the histogram with RoI if available or the entire image
		if( bObj_Found )
		{
			histogram.CalcHistogram( &img_mat_hsv( Hist_RoI ) );
		}
	}
	else
	{
		histogram.CalcHistogram( &img_mat_hsv );
	}

	// Show it
	histogram.ShowHistogram();

	histogram.CopyData( v_hist );

	pDataSource->set_HistogramData( v_hist );
}

void CProcessImageColourLearning::ObjectPostProcessing( void )
{
	LookForNearObjects( OBJECT_HAND, OBJECT_RED );
	LookForNearObjects( OBJECT_HAND, OBJECT_GREEN );
	LookForNearObjects( OBJECT_HAND, OBJECT_YELLOW );
}

bool CProcessImageColourLearning::SearchObjects_Hand( void )
{
	bool bResult = false;

	if( HandTracker.isReady() )
	{
		// Use hand tracker
		HandTracker.TrackObjects( &vSegmentedImage.at( COLOUR_C ).Image );

		HandTracker.get_Tracked_Objects_Vector( pDataSource->vObjects.Obj.at( OBJECT_HAND ) );

		bResult = ( pDataSource->vObjects.Obj.at( OBJECT_HAND ).size() > 0 );
	}
	
	else
	{
		// Use simple colour association
		bResult = Detect_Object_From_Primary_Colour( OBJECT_HAND );
	}

	Define_New_Region_of_Interest( OBJECT_HAND, cvRect( -30, -30, 70, 130 ) );
	
	return bResult;
}

// Process the image and retrieve data
void CProcessImageColourLearning::ProcessTask( cv::Mat * mat_ptr )
{
	//Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
		
	// Segment all colours
	SegmentAllColours();
		
	// Execute Morphological operations
	for( int i=0 ; i<NUMBER_OF_COLOURS ; i++ )
	{
		vSegmentedImage.at( i ).doMorphologicalOperations();
	}

	// Detect colour spots
	DetectAllColourSpots();

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
}
