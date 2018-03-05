#include "ProcessImageTest.h"

#include <opencv2/highgui/highgui.hpp>

#ifndef ENABLE_TEST_WINDOW
#define ENABLE_TEST_WINDOW FALSE
#endif

CProcessImageTest::CProcessImageTest( int width, int height )
: CProcessImage( width, height )
, strWinName( "Test" )
{
	sSegFile = "Videre/Segmentation_Parameters (Test).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_Test;

#if ENABLE_TEST_WINDOW
	cv::namedWindow( strWinName );
#endif
}

void CProcessImageTest::SetupProcessing_TaskSpecific( int width, int height )
{
}

void CProcessImageTest::Clean_TaskSpecific( void )
{
#if ENABLE_TEST_WINDOW
	cv::destroyWindow( strWinName );
#endif
}

void CProcessImageTest::ObjectPreProcessing( void )
{
}
	
void CProcessImageTest::ObjectPostProcessing( void )
{
}

void CProcessImageTest::ProcessTask( cv::Mat * mat_ptr )
{
	// Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
}

void CProcessImageTest::Post_Process( void )
{
}

bool CProcessImageTest::SearchAllObjects( void )
{
	return true;
}

