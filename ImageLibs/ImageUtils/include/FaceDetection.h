#ifndef _FACE_DETECTION_H_
#define _FACE_DETECTION_H_

#pragma once

#include "HaarClassifier.h"

class CFaceDetection
{
public:
	CFaceDetection( bool frontal_detection = true, bool profile_detection = true );
	
	// Accessors
public:
	// Returns the number of detected faces
	size_t get_NumberOfFaces( void );
	
	// Copies to rect an array with regions of all detected faces, memory for rect maust be preallocated
	void get_RegionOfFaces( cv::Rect * rect );
	
	// Returns the region of face 'iFace', passing by reference to rect
	void get_RegionOfFace( size_t iFace, cv::Rect * rect );
	
	// Returns the region of face 'iFace'
	cv::Rect get_RegionOfFace( size_t iFace );
	
	// Returns area of face 'iFace'
	int get_FaceArea( unsigned int iFace );

	// Classifiers and data storage
protected:
	CHaarClassifier
		Face_frontal,
		Face_profile;

	std::vector< cv::Rect > vFaces;

	bool
		bFrontal,
		bProfile;

public:
	// Pass an image to this function to trigger a face detection, returns true if there are faces present, false otherwise
	bool Detect( cv::Mat * img, double scale = 0.7 );

	// Draws a rectangle around each detected face in image 'img'
	void Draw( cv::Mat * img, bool bText = false );
};

#endif // _FACE_DETECTION_H_
