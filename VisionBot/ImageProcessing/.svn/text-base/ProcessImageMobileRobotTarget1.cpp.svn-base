#include "ProcessImageMobileRobotTarget1.h"

#include <opencv2/highgui/highgui.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

CProcessImageMobileRobotTarget1::CProcessImageMobileRobotTarget1( int width, int height )
: CProcessImage( width, height )
, mask_mirror( height, width, CV_8UC3 )
{
	sSegFile += "Omnidirectional/Segmentation_Parameters_(MRT).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_MobileRobotTarget1;

	cv::Point2i center( width/2, height/2 );
	int radius_white( height/2 );
	int radius_black( height/4 );
	cv::circle( mask_mirror, center, radius_white, cv::Scalar::all(255), -1 );
	cv::circle( mask_mirror, center, radius_black, cv::Scalar::all(0), -1 );
}

CProcessImageMobileRobotTarget1::~CProcessImageMobileRobotTarget1(void)
{
	Clean_TaskSpecific();
}

void CProcessImageMobileRobotTarget1::Clean_TaskSpecific( void )
{
	if( !mask_mirror.empty() )
		mask_mirror.release();
}

bool CProcessImageMobileRobotTarget1::SearchAllObjects( void )
{
	bool success( false );

	success |= Associate_Colour_to_Object( COLOUR_R, OBJECT_RED, 5 );
	success |= Associate_Colour_to_Object( COLOUR_Y, OBJECT_YELLOW, 5 );
	success |= Associate_Colour_to_Object( COLOUR_G, OBJECT_GREEN, 5 );
	success |= Associate_Colour_to_Object( COLOUR_C, OBJECT_CYAN, 5 );
	success |= Associate_Colour_to_Object( COLOUR_B, OBJECT_BLUE, 5 );
	success |= Associate_Colour_to_Object( COLOUR_M, OBJECT_MAGENTA, 5 );

	return success;
};

void CProcessImageMobileRobotTarget1::ProcessTask( cv::Mat * mat_ptr )
{
	cv::bitwise_and( *mat_ptr, mask_mirror, mat_ImageProcessed.Obj );
	
	//Convert source image to HSV
	//cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
	cv::cvtColor( mat_ImageProcessed.Obj, mat_ImageHSV, CV_BGR2HSV );

	SegmentAllColours();

	size_t nColours = vSegmentedImage.size();
	for( size_t nColour = 0; nColour < nColours; nColour++ )
	{
		vSegmentedImage.at( nColour ).doMorphologicalOperations();
	}

	DetectAllColourSpots();

	//-----------------------------------------
	// Search for objects
	SearchAllObjects();
};

void CProcessImageMobileRobotTarget1::ObjectPreProcessing( void )
{
	bool success( false );
	
	float angle, distance;

	success = ComputeTarget( OBJECT_RED, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_RED, angle, distance, success );

	success = ComputeTarget( OBJECT_YELLOW, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_YELLOW, angle, distance, success );

	success = ComputeTarget( OBJECT_GREEN, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_GREEN, angle, distance, success );

	success = ComputeTarget( OBJECT_CYAN, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_CYAN, angle, distance, success );

	success = ComputeTarget( OBJECT_BLUE, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_BLUE, angle, distance, success );

	success = ComputeTarget( OBJECT_MAGENTA, mat_ImageHSV.cols, mat_ImageHSV.rows, &angle, &distance );
	pDataSource->set_MobileRobotTarget( OBJECT_MAGENTA, angle, distance, success );
}

float CProcessImageMobileRobotTarget1::get_PhysicalDistance( int pixels )
{
	const float ke(   0.0000004531f );
	const float kd(  -0.0002768784f );
	const float kc(   0.0634554293f );
	const float kb(  -6.4391470186f );
	const float ka( 244.3328942094f );

	return ke*pow( (float)pixels, 4 ) + kd*pow( (float)pixels, 3 ) + kc*pow( (float)pixels, 2 ) + kb*(float)pixels + ka;
}

bool CProcessImageMobileRobotTarget1::ComputeTarget(int type, int width, int height, float *angle, float *distance)
{
	bool success( false );

	int xImg = 0, yImg = 0;
	int xCentre = 0, yCentre = 0, distCentre = 0;
	
	if( pDataSource->vObjects.Obj.at( type ).size() )
	{
		std::sort( pDataSource->vObjects.Obj.at( type ).begin(), pDataSource->vObjects.Obj.at( type ).end() );

		xImg = pDataSource->vObjects.Obj.at( type ).back().Region.x + pDataSource->vObjects.Obj.at( type ).back().Region.width/2;
		yImg = pDataSource->vObjects.Obj.at( type ).back().Region.y + pDataSource->vObjects.Obj.at( type ).back().Region.height/2;

		xCentre = -( width/2 - xImg );
		yCentre = -( height/2 - yImg );
		distCentre = (int)sqrt( pow( (float)xCentre, 2 ) + pow( (float)yCentre, 2 ) );
		
		*angle = atan2( (float)yCentre, (float)xCentre ) - (float)M_PI/2.0f;
		*distance = this->get_PhysicalDistance( distCentre );

		success = true;
	}
	else
	{
		bool tmpExist;
		pDataSource->get_MobileRobotTarget(OBJECT_MAGENTA, angle, distance, &tmpExist);
	}

	return success;
}