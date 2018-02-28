#ifndef _STEREO_VIDERE_H_
#define _STEREO_VIDERE_H_

#pragma once

#include <opencv2/core/core.hpp>

// Videre Small Vision System header
#include <svsclass.h>

#include "AttachMutex.hpp"
#include "ConfigFile.h"
#include "Matrix.h"

class CStereoVidere 
	: public CConfigFile
{
public:
	CStereoVidere( void );
	~CStereoVidere( void );

	//-----------------------------------------------------------------------
public:
	// Objects or functions that depend on SVS system
	
	svsStereoProcess * stereoProcess;
	
	svsStereoImage * pStereoImage;

	// Give a pointer to the disparity image
	void set_StereoImage( svsStereoImage * img );

protected:
	bool bUseMultiScale;

	//-----------------------------------------------------------------------
protected:
	// Instantiate the SVS object responsible for applying the stereo
	void Instantiate_SVS_Object( bool multiscale = false );
	void Destroy_SVS_Object( void );

	//-----------------------------------------------------------------------
public:
	// Apply the Stereo
	void Process_Stereo( void );
	
	//-----------------------------------------------------------------------
	// Funtions to generate segmented images from the stereo disparity image
public:
	void Create_3D_Segmentation_Image_Coord_Limits( cv::Mat * mat_ptr, cv::Point3f limit_min, cv::Point3f limit_max );

	void Create_3D_Segmentation_Image_Coord_Limits( cv::Mat * mat_ptr );

	void Create_3D_Segmentation_Image_Distance_Limits( cv::Mat * mat_ptr, float limit_min, float limit_max );

	void Generate_Disparity_Image( unsigned char gray_min=16, unsigned int gray_max=255 );

	CAttachMutex_t< cv::Point3f >
		ptLimitMax,
		ptLimitMin;

	//-----------------------------------------------------------------------
	// Functions to handle the stereo disparity image
public:
	CAttachMutex_o< cv::Mat > matDisparity;

	std::string strDisparityWindow;

	void set_DisplayDisparityWindow( bool display );
	bool get_DisplayDisparityWindow( void );

	void Save_Disparity_Image_to_File( std::string strFile );

protected:
	CAttachMutex_n<bool> 
		bDisplayDisparity,
		bDisplayWindowCreated;

	void CreateDisparityWindow( void );
	void DestroyDisparityWindow( void );

	//-----------------------------------------------------------------------
	// Specific functions that sets up parameters that depend of the robot
protected:
	// Variable parameters (pan and tilt)
	virtual void Setup_Transformation_Parameters( void );

	// Constant parameters
	virtual void Load_Constant_Transformation_Parameters( void ) = 0;

	void Clear_Constant_Transformation_Parameters( void );

	// Resize the arrays containing the parameters used to build the transformation matrices
	void Resize_Transformation_Arrays( unsigned int num_transf );

	//-----------------------------------------------------------------------
	// Coordinate transformation
public:
	// Calculate the transformation matrix
	void CalculateTransfMatrix( void );

	// Transform coordinates 'Original' on the vision axis in the world axis
	void TransformCoordinates( float Original[3], float * pResult );
	
	void TransformCoordinates( cv::Point3f Original, cv::Point3f * pResult );
	
	// Calculates the transformation matrix using the pan and tilt values, and performs the coordinate transformation
	void TransformCoordinatesFull( float Original[3], float pan, float tilt, float * pResult );

	void TransformCoordinatesFull( cv::Point3f Original, float pan, float tilt, cv::Point3f * pResult );

	// Compensate the coordinates calculated in stereo if required (Vision frame)
	virtual void Compensate_Coordinates_Vision( float *tX, float *tY, float *tZ ) {};

	void Compensate_Coordinates_Vision( cv::Point3f * pt );

	// Compensate the coordinates calculated in stereo if required (World frame)
	virtual void Compensate_Coordinates_World( float *tX, float *tY, float *tZ ) {};

	void Compensate_Coordinates_World( cv::Point3f * pt );

protected:
	//Matrixes to do coordinate transformation
	CMatrix<float> 
		T, 
		T_1, 
		T_2,
		mOriginal, 
		mTransf;

public:
	CAttachMutex_n<float>
		Pan,
		Tilt;

protected:
	// Number of Matrix transformations required to transform coordinates from
	// the vision axes to the robot axes
	unsigned int uNum_Transf;

	float Offset[3];

	// Variables that have important information to perform coordinate transformation
	float 
		Baseline, // in milimeters
		K1, 
		K2, 
		K3,
		K4;

	// Set of values for coordinate transformation using the Denavit-Hartenberg convention
	std::vector<float>
		Alpha,
		A,
		D,
		Theta;

	//-----------------------------------------------------------------------
	// Get coordinates from picture
protected:
	bool Calculate_3D_Point( int nX, int nY, float * fX, float * fY, float * fZ );
	bool Calculate_3D_Point( cv::Point2i pt_img, cv::Point3f * pt_vision );

public:
	bool CalculateCoordinates( int nX, int nY, int nWidth, int nHeight, float * pResult_world, float * pResult_vision = nullptr );
	
	bool CalculateCoordinates( cv::Rect region, float * pResult_world, float * pResult_vision = nullptr );

	bool CalculateCoordinates_Mask( int nX, int nY, int nWidth, int nHeight, cv::Mat mask, float * pResult_world, float * pResult_vision = nullptr );

	bool CalculateCoordinates_Mask( cv::Rect region, cv::Mat mask, float * pResult_world, float * pResult_vision = nullptr );

	//-----------------------------------------------------------------------
protected:
	// Parse parameters from a configuration file
	bool Parse_Parameters_Load( void );

};

#endif // _STEREO_VIDERE_H_
