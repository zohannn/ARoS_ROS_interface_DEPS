#include "ProcessImageHRTransport.h"

CProcessImageHRTransport::CProcessImageHRTransport( int width, int height )
{
	sSegFile = "Videre/Segmentation_Parameters (HR Transport Dumbo).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_HRTransport;

	vColourTask.emplace_back( COLOUR_G );
}

bool CProcessImageHRTransport::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Other objects used to identify columns
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_GRASP_POINT );

	return bFound;
}

// Process the image and retrieve data
void CProcessImageHRTransport::ProcessTask( cv::Mat * mat_ptr )
{	
	//Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
	
	size_t colour_size = vColourTask.size();
	// Apply segmentation, perform morphological operations and Detect spots.
	for( size_t nColour=0 ; nColour<colour_size ; nColour++ )
	{
		SegmentColour_cv( vColourTask.at( nColour ) );
		vSegmentedImage.at( vColourTask.at( nColour ) ).doMorphologicalOperations();
		vSegmentedImage.at( vColourTask.at( nColour ) ).DetectSpots_Contours();
	}

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
}

void CProcessImageHRTransport::ObjectPreProcessing( void )
{
}

void CProcessImageHRTransport::Action_Interpretation( void )
{
}