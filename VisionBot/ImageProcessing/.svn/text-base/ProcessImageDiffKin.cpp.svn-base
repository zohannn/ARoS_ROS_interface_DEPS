#include "ProcessImageDiffKin.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "MathDefines.h"

#ifndef MAX_AREA_OBJECT_MAGENTA_SMALL
#define MAX_AREA_OBJECT_MAGENTA_SMALL 6000ul
#endif

CProcessImageDiffKin::CProcessImageDiffKin( int width, int height )
: CProcessImageToyRobotFull_SVS( width, height )
{
	sSegFile = "Videre/Segmentation_Parameters (Differential Kinematics).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_DiffKin;
}

bool CProcessImageDiffKin::SearchAllObjects( void )
{
	bool bFound = SearchObjects_Red();

	//bool bFound = SearchObjects_Handle();

	bFound |= SearchObjects_Green();

	bFound |= SearchObjects_Magenta();

	////bFound = SearchObjects_Nut();

	////bFound |= SearchObjects_Wheel();

	bFound |= SearchObjects_Yellow();

	return bFound;
}

bool CProcessImageDiffKin::SearchObjects_Red( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_RED );
}

bool CProcessImageDiffKin::SearchObjects_Yellow( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_YELLOW );
}

bool CProcessImageDiffKin::SearchObjects_Green( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_GREEN );
}

bool CProcessImageDiffKin::SearchObjects_Magenta( void )
{
#if 0
	bool 
		bSmall = Detect_Object_From_Primary_Colour( OBJECT_MAGENTA_SMALL, 0ul, MAX_AREA_OBJECT_MAGENTA_SMALL ),
		bBig = Detect_Object_From_Primary_Colour( OBJECT_MAGENTA_BIG, MAX_AREA_OBJECT_MAGENTA_SMALL + 1ul );
	
	return bSmall || bBig;
#else
	return Detect_Object_From_Primary_Colour( OBJECT_MAGENTA );
#endif
}

bool CProcessImageDiffKin::SearchObjects_Handle( void )
{
	//--------------------------------------------------------------------------
	// Handle detection parameters

	const int 
		OBJ_MAIN_COLOUR = COLOUR_Y,
		OBJ_TYPE = OBJECT_HANDLE,
		SEC_COLOUR = COLOUR_G,
		SEC_OBJECT = OBJECT_SPOT_GREEN;

	//--------------------------------------------------------------------------
	// Auxiliary variables
	bool 
		bFound = false,
		bOverlapped = false,
		bSpotUsed = false;

	int 
		iColour = 0,
		iSecColour = 0;

	cv::Rect rect_max;

	// Create one auxiliary object with no area.
	CObject_f 
		obj( OBJECT_HANDLE ),
		obj_spot;

	std::vector< cv::Rect >::iterator 
		itRect, 
		itRect2;

	//--------------------------------------------------------------------------
	// Clear objects from previous iteration
	pDataSource->vObjects.Obj.at( OBJECT_HANDLE ).clear();

	//--------------------------------------------------------------------------
	itRect = vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.begin();

	while( itRect != vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.end() )
	{
		bSpotUsed = false;

		//Clear auxiliary object properties
		obj.ClearObject();

		// Set the handle area to the rectangle occupied by the yellow spot.
		obj.vSpot_Primary.emplace_back( *itRect );
		obj.set_Region( *itRect );

		iSecColour = SEC_COLOUR;

		itRect2 = vSegmentedImage.at( iSecColour ).vSpots.begin();
	
		// Go through all the spots of the secondary colour.
		while( itRect2 != vSegmentedImage.at( iSecColour ).vSpots.end() )
		{
			bOverlapped = CheckOverlapSpot(
				&vSegmentedImage.at( OBJ_MAIN_COLOUR ).Image,	//Image 1
				&( *itRect ),							//Spot in image 1
				&vSegmentedImage.at( iSecColour ).Image,		//Image 2
				&( *itRect2 )							//Spot in image 2
				);

			if( bOverlapped )
			{
				bSpotUsed = true;

				// The column area is the sum of all the spots that compose it.
				rect_max = MaxRect( obj.Region, *itRect2 );
				obj.set_Region( rect_max );
				obj.set_Type( OBJ_TYPE );
					
				// Define the sub object
				obj.vSubObjects.emplace_back( CObject_f( SEC_OBJECT, *itRect2 ) );

				itRect2 = vSegmentedImage.at( iSecColour ).vSpots.erase( itRect2 );
			}
			else
			{
				itRect2++;
			}
		}

		//-------------------------------------------------------------------

		// Test to see if the current Yellow spot was used.
		if( bSpotUsed )
		{
			// It was used, erase it, and go to the next one.
			itRect = vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.erase( itRect );

			// Sort the sub objects by size. Smaller first.
			std::sort( obj.vSubObjects.begin(), obj.vSubObjects.end() );

			// Reverse the objects order, larger first
			std::reverse( obj.vSubObjects.begin(), obj.vSubObjects.end() );

			//Add the identified object to the Data Source.
			pDataSource->vObjects.Obj.at( obj.get_Type() ).emplace_back( obj );

			bFound = true;
		}
		else
		{
			// It wasn't, go to the next one.
			itRect++;
		}
	}

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageDiffKin::ObjectPreProcessing( void )
{
	Generate_Object_Masks();

#if SHOW_DEBUG_IMAGE
	if( pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).size() > 0 )
		cv::imshow( "Debug", pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).front().imgMask );
#endif
}

void CProcessImageDiffKin::CalculateOrientation_Handle( CObject_f * pHandle )
{
	// if there's no 3D data, then there's no point in continuing.
	if( !pHandle->has3D() ) return;

	// How many spots of the secondary colour this object has?
	size_t uSpots = pHandle->vSubObjects.size();

	// Are there enough spots to calculate orientation
	if( uSpots <= 0 ) return;

	cv::Point3f
		point_spot_1( 0.0f, 0.0f, 0.0f ),
		point_spot_2( 0.0f, 0.0f, 0.0f ),
		point_ori( 0.0f, 0.0f, 0.0f );		// The orientation

	pHandle->vSubObjects.front().get_Coordinates_World( &point_spot_2 );
	pHandle->get_Coordinates_World( &point_spot_1 );

	point_ori.z = atan2( point_spot_1.y - point_spot_2.y, point_spot_1.x - point_spot_2.x );

	point_ori.z *= RAD_TO_DEG_F;

	while( point_ori.z < 0.0f )
		point_ori.z += 360.0f;

	while( point_ori.z > 360.0f )
		point_ori.z -= 360.0f;

	pHandle->set_Orientation( point_ori );
}

void CProcessImageDiffKin::ObjectPostProcessing( void )
{
	// ----------------------------------------------------------------------
	// Get Handle orientation

	size_t nObj = pDataSource->vObjects.Obj.at( OBJECT_HANDLE ).size();

	for( size_t iObj=0 ; iObj<nObj ; iObj++ )
	{
		CalculateOrientation_Handle( &pDataSource->vObjects.Obj.at( OBJECT_HANDLE ).at( iObj ) );
	}
}
