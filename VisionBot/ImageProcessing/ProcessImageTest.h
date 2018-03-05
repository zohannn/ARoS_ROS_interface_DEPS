#ifndef _PROCESS_IMAGE_TEST_H_
#define _PROCESS_IMAGE_TEST_H_

#pragma once

#include "ProcessImage.h"

#include "ObjectTracker.h"
#include "TimeCountOpenCV.h"

class CProcessImageTest
	: public CProcessImage
{
public:
	CProcessImageTest( int width = 640, int height = 480 );

public:	
	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	bool SearchAllObjects( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

	void Post_Process( void );

protected:
	void SetupProcessing_TaskSpecific( int width, int height );

	void Clean_TaskSpecific( void );

protected:
	std::string strWinName;

// Temporary test functions and members
public:
	bool Search_Hands( void );

	std::string strLog;

	CTimeCountBasicOpenCV time_count;
};

#endif // _PROCESS_IMAGE_TEST_H_
