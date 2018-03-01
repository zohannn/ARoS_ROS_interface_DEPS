#ifndef _HAAR_CLASSIFIER_H_
#define _HAAR_CLASSIFIER_H_

#pragma once

//---------------------------------------------------------------------------
// It seems there is a conflict between OpenCV-2.3.x+ and ACE-6.0.x+
// macros 'max' and 'min' are defined in ACE and this conflicts
// with a some of the code used in OpenCV
// undefining those macros solves the conflict.

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

//---------------------------------------------------------------------------

#ifdef USE_NVIDIA_CUDA
	#include <opencv2/gpu/gpu.hpp>
	#define IMAGE_TYPE cv::gpu::GpuMat
	#define CASCADE_TYPE cv::gpu::CascadeClassifier_GPU
#else
	#include <opencv2/objdetect/objdetect.hpp>
	#define IMAGE_TYPE cv::Mat
	#define CASCADE_TYPE cv::CascadeClassifier
#endif

//---------------------------------------------------------------------------

#include <string>

class CHaarClassifier
{
public:
	// If isPathRelative is true, the initial path is in "{OpenCV_ROOT}/data/haarcascades" folder
	CHaarClassifier( std::string cascade_path, bool isPathRelative = true );
	~CHaarClassifier( void );

protected:
	IMAGE_TYPE matImageGray;
	CASCADE_TYPE Haar_Cascade;

    std::string strCascadeName;

	int Number_Of_Objects;
	
	std::vector< cv::Rect > vObjects;

public:
	bool Detect( cv::Mat * img, double scale=0.7 );
	void Draw( cv::Mat * img, bool bText = false );

	size_t get_NumberOfObjects( void );

	void get_RegionOfObjects( std::vector<cv::Rect> &vRect );
	void get_RegionOfObject( int iObj, cv::Rect * rect );
	cv::Rect get_RegionOfObject( int iObj );
	
	int get_AreaOfObject( int iObj );

	static void Remove_Duplicates( std::vector<cv::Rect> &vRect );
};

#endif // _HAAR_CLASSIFIER_H_
