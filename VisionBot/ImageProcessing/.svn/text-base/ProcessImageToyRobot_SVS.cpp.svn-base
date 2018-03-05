#include "ProcessImageToyRobot_SVS.h"

#include <opencv2/highgui/highgui.hpp>

CProcessImageToyRobot_SVS::CProcessImageToyRobot_SVS( int width, int height )
: CProcessImage( width, height )
, Hand_ROI( cvRect( 0, 0, 100, 100 ) )
{
	sSegFile += "Videre/Segmentation_Parameters (Construction Task).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ToyRobot_SVS;
}

bool CProcessImageToyRobot_SVS::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Detect the objects

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BASE );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_WHEEL, 1200 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BASE_TIP, 0, 1199 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_NUT, 100 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_COLUMN );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_HAND );

	Define_New_Region_of_Interest( OBJECT_HAND, cv::Rect( -30, -30, 130, 130 ) );

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageToyRobot_SVS::ObjectPreProcessing( void )
{
}

void CProcessImageToyRobot_SVS::ObjectPostProcessing( void )
{
	LookForNearObjects( OBJECT_BASE, OBJECT_WHEEL );
	
	LookForNearObjects( OBJECT_BASE, OBJECT_NUT );

	LookForNearObjects( OBJECT_NUT, OBJECT_WHEEL );
		
	LookForNearObjects( OBJECT_HAND, OBJECT_WHEEL );
	
	LookForNearObjects( OBJECT_HAND, OBJECT_NUT );

	LookForNearObjects( OBJECT_HAND, OBJECT_COLUMN );

	Action_Interpretation();
}

// Process the image and retrieve data
void CProcessImageToyRobot_SVS::ProcessTask( cv::Mat * mat_ptr )
{
	//Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
		
	SegmentAllColours();
		
	size_t nColours = vSegmentedImage.size();
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		vSegmentedImage.at( iColour ).doMorphologicalOperations();
	}
		
	DetectAllColourSpots();

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
		
	size_t num_hands = pDataSource->vObjects.Obj.at( OBJECT_HAND ).size();

	cv::Mat img_hand;

#if 0
	if( num_hands > 0 )
	{
		CvRect region;
		mat_ImageSource.lock();
		region = pDataSource->vObjects.Obj.at( OBJECT_HAND ).front().RoI;

		int 
			size_hand = img_hand->height*img_hand->width,
			size_src = region.width*region.height;

		if( size_hand != size_src )
		{
			if( !img_hand.empty() ) img_hand.release();
			img_hand.create( region.height, region.width, CV_8UC3 );
			img_hand.setTo( 0 );
		}
		
		mat_ImageSource.Obj( region ).copyTo( img_hand );
		mat_ImageSource.unlock();
	}
#endif

	if( num_hands > 0 )	Hand_ROI = pDataSource->vObjects.Obj.at( OBJECT_HAND ).front().RoI;

	SaveHandImage();
}
