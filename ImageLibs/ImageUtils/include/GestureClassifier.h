#ifndef _GESTURE_CLASSIFIER_H_
#define _GESTURE_CLASSIFIER_H_

#pragma once

#include <opencv2/core/core.hpp>
#include "AttachMutex.hpp"
#include "ConfigFile.h"
#include "ObjectBasic.h"

class CGestureClassifier
	: public CConfigFile
{
public:
	CGestureClassifier( bool detect_ag = true, bool detect_sg = true, bool detect_pt = true );
	~CGestureClassifier( void );

	unsigned int detect( cv::Rect RoI_hand, cv::Mat &mask, cv::Mat &img_Original, cv::Mat &img_HSV_hand = cv::Mat() );

	void mark_detected_gesture( unsigned int gesture, cv::Rect RoI_hand, cv::Mat &img_Original );

	// Reset gestures and pointed objects from the previous iteration
	void Reset( void );

	//-----------------------------------------------------------------------
protected:
	// Load parameters from file
	bool Parse_Parameters_Load( void );
	
	// Write parameter to file
	void Parse_Parameters_Save( void );

public:
	// Flag to execute or not gesture detection
	CAttachMutex_n<bool> bActive;

	// Parameters for morphological operations detecting hand colour
	CAttachMutex_n<unsigned int>
		Hand_hue[2],	// 0 - Min ; 1 - Max
		Hand_sat[2],	// 0 - Min ; 1 - Max
		Hand_val[2],	// 0 - Min ; 1 - Max
		Hand_morph[3];	// 0 - Open(1)/Close(0) ; 1 - Erode Iterations ; 2 - Dilate Iterations

	// Parameter used to distinguish side grip from pointing.
	CAttachMutex_n<int> nWidthGesture;

	CAttachMutex_n<bool>
		bDetect_AboveGrip,
		bDetect_SideGrip,
		bDetect_Pointing;
	
	CAttachMutex_o< std::vector<double> >
		dPower,	// 10^power used for each moment
		vMoments_Above,	// Hu moments for above grip
		vMoments_Side,	// Hu moments for side grip
		vMoments_Point;	// Hu moments for pointing

	// Vector containing all the recognized gestures.
	CAttachMutex_o< std::vector<unsigned int> > vGestures;

	// Vector containing informations on all the objects that will be tested.
	CAttachMutex_o< std::vector< CObjectBasic > > vObjects;

	// The pointed objects, one for each hand
	CAttachMutex_o< std::vector< CObjectBasic > > PointedObject;
};

#endif // _GESTURE_CLASSIFIER_H_
