#ifndef _SEGMENTED_IMAGE_H_
#define _SEGMENTED_IMAGE_H_

#pragma once

#include <opencv2/core/core.hpp>
#include <vector>

#include "AttachMutex.hpp"
#include "Colour.h"
#include "ConfigFile.h"
#include "RectEx.hpp"
#include "VisionDefinitions.h"

class CSegmentedImage
	: public CConfigFile
{
public:
	CSegmentedImage( void );

	// Use copy constructor carefully. Semaphores are not copied
	CSegmentedImage( const CSegmentedImage & image_seg );

	~CSegmentedImage( void );

//------------------------------------------------------------------------------
// Objects
public:
	cv::Mat Image;

	CColour the_Colour;

	std::vector< cv::Rect >
		vSpots,
		vSpotsIn;

	std::vector< std::vector< cv::Point > > vContours;

	std::vector< cv::Vec4i > vHierarchy;

protected:
	std::vector< cv::Rect > vSpots_bkp;

	//-----------------------------------------------------------------------
protected:
	bool Parse_Parameters_Load( void );

public:
	void DetectSpots_FloodFill( bool bAppend = false );

	void DetectSpots_Contours( bool bAppend = false );

	void Setup( int width = 640, int height = 480, int nColour = 0 );

	void Clean( void );

	//-----------------------------------------------------------------------

	void doMorphologicalOperations( void );

	void Spots_Store( void );

	void Spots_Retrieve( void );
	
public:
	CAttachMutexPtr_n<unsigned int> 
		uHue[2],	// 0 - Min ; 1 - Max
		uSat[2],	// 0 - Min ; 1 - Max
		uVal[2],	// 0 - Min ; 1 - Max
		uMorph[3];	// 0 - Open(1)/Close(0) ; 1 - Erode Iterations ; 2 - Dilate Iterations
};

#endif // _SEGMENTED_IMAGE_H_
