#include "ProcessImageToyRobotFull_AVT.h"

CProcessImageToyRobotFull_AVT::CProcessImageToyRobotFull_AVT( int width, int height )
: CProcessImage( width, height )
{
	sSegFile += "AVT/Segmentation_Parameters (Construction Task).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ToyRobotFull_AVT;
}

bool CProcessImageToyRobotFull_AVT::SearchAllObjects( void )
{
	bool 
		bFound = false,
		bResult = false;

	//--------------------------------------------------------------------------
	// Detect the objects

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageToyRobotFull_AVT::ObjectPreProcessing( void )
{
}

void CProcessImageToyRobotFull_AVT::ObjectPostProcessing( void )
{
}

// Process the image and retrieve data
void CProcessImageToyRobotFull_AVT::ProcessTask( cv::Mat * mat_ptr )
{
	// Implementation here!
}
