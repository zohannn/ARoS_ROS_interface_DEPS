#include "ProcessImagePlayPiano.h"

#include <opencv2/highgui/highgui.hpp>

CProcessImagePlayPiano::CProcessImagePlayPiano( int width, int height )
: CProcessImage( width, height )
, uColoursTask( 6 )
, histogram()
, vColourTask( uColoursTask, 0 )
{
	cv::namedWindow( histogram.strWindowName_Histogram );

	//sSegFile = "Videre/Segmentation_Parameters (Play Piano).ini";
	sSegFile = "Videre/Segmentation_Parameters (Piano_nanoPAD2).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_PlayPiano;

	vColourTask.at( 0 ) = COLOUR_R;
	vColourTask.at( 1 ) = COLOUR_G;
	vColourTask.at( 2 ) = COLOUR_B;
	vColourTask.at( 3 ) = COLOUR_C;
	vColourTask.at( 4 ) = COLOUR_M;
	vColourTask.at( 5 ) = COLOUR_Y;
}

// Process the image and retrieve data
void CProcessImagePlayPiano::ProcessTask( cv::Mat * mat_ptr )
{	
	//Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
	
	size_t nColours = vColourTask.size();
	// Apply segmentation, perform morphological operations and Detect spots.
	for( size_t iColour=0 ; iColour<nColours ; iColour++ )
	{
		SegmentColour_cv( vColourTask.at( iColour ) );
		vSegmentedImage.at( vColourTask.at( iColour ) ).doMorphologicalOperations();
		vSegmentedImage.at( vColourTask.at( iColour ) ).DetectSpots_Contours();
	}

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
}

bool CProcessImagePlayPiano::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Other objects used to identify columns
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_RED );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_GREEN );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BLUE );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_CYAN );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_MAGENTA );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_YELLOW );

	return bFound;
}

void CProcessImagePlayPiano::ObjectPreProcessing( void )
{
	std::vector<int> v_hist( HIST_LENGTH_PIANO, 0 );

	cv::Rect obj_roi;

	const unsigned int 
		uObjects = 6,
		aObjects[uObjects] = { OBJECT_RED, OBJECT_GREEN, OBJECT_BLUE, OBJECT_CYAN, OBJECT_MAGENTA, OBJECT_YELLOW };
	
	bool bObjectExists = false;

	for( unsigned int i=0 ; i<uObjects ; i++ )
	{
		if( pDataSource->vObjects.Obj.at( aObjects[i] ).size() > 0 )
		{
			obj_roi = pDataSource->vObjects.Obj.at( aObjects[i] ).front().Region;
			bObjectExists = true;
			break;
		}
	}

	histogram.CalcHistogram( &mat_ImageHSV( obj_roi ) );
	histogram.CopyData( v_hist );

	if( bObjectExists )
		pDataSource->set_HistogramData( v_hist );
	else
		pDataSource->set_HistogramData( std::vector<int>( HIST_LENGTH_PIANO, 0 ) );

	histogram.ShowHistogram();
}

void CProcessImagePlayPiano::Action_Interpretation( void )
{
}
