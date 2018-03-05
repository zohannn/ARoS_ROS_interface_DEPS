#ifndef _PROCESS_IMAGE_FACE_TRACKING_H_
#define _PROCESS_IMAGE_FACE_TRACKING_H_

#pragma once

#include "ProcessImage.h"
#include "FaceDetection.h"

#if USE_AmariField2D // RuiSilva had USE_AmariField2D==TRUE
#include "ARoSGazeControl.hpp"
#endif

class CProcessImageFaceTracking
	: public CProcessImage
{
public:
	CProcessImageFaceTracking( int width = 640, int height = 480 );

	//--------------------------------------------------------------------------
	// Variable functions acording to the task
public:

	// Search for objects for the robot task
	bool SearchAllObjects( void );

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

private:
	bool SearchFaces( void );

private:
	// The Face Detection Object
	CFaceDetection Face;

#if USE_AmariField2D // RuiSilva had USE_AmariField2D==TRUE
	CARoSGazeControl_f DynField;
#endif

protected:
	void ProcessAttention( void );
};

#endif // _PROCESS_IMAGE_FACE_TRACKING_H_
