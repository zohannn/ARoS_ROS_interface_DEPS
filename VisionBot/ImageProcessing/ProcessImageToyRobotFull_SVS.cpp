#include "ProcessImageToyRobotFull_SVS.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "commands.h"
#include "common.h"
#include "MathDefines.h"
#include "MathOperations.h"
#include "range.hpp"
#include "RectOperations.hpp"

CProcessImageToyRobotFull_SVS::CProcessImageToyRobotFull_SVS( int width, int height )
: CProcessImage( width, height )
{
	sSegFile = "Videre/Segmentation_Parameters (Construction Task Full).ini";
	//sSegFile = "Videre/Segmentation_Parameters (Construction Task Summer School).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ToyRobotFull_SVS;
}

bool CProcessImageToyRobotFull_SVS::SearchAllObjects( void )
{
	bool bFound = false;

	// The order for object detection matters!
	// Base -> Columns -> Wheels -> Nuts
	// The Hand has no specific detection order, the colour Cyan is just for it.

	//--------------------------------------------------------------------------
	// Search for the Base
	bFound |= SearchObjects_Base();
	
	//--------------------------------------------------------------------------
	// Search for the Columns
	bFound |= SearchObjects_Column();

	//--------------------------------------------------------------------------
	// Search for the Wheels
	bFound |= SearchObjects_Wheel();

	//--------------------------------------------------------------------------
	// Search for the Nuts
	bFound |= SearchObjects_Nut();

	//--------------------------------------------------------------------------
	// Search for the Top Floor
	bFound |= SearchObjects_Top_Floor();
	
	//--------------------------------------------------------------------------
	// Search for the Hands
	bFound |= SearchObjects_Hand();

	//--------------------------------------------------------------------------
	// Search auxiliary objects
	bFound |= SearchObjects_Auxiliary();
	
	//--------------------------------------------------------------------------

	return bFound;
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Base( void )
{
	//--------------------------------------------------------------------------
	// Base detection parameters

	const int OBJ_TYPE = OBJECT_BASE;

	const int 
		OBJ_COLOUR = GET_VALID_PrimaryColour( OBJ_TYPE ),
		HOLE_COLOUR[4] = { COLOUR_Y, COLOUR_M, COLOUR_B, COLOUR_R },
		HOLE_TYPE[4] = { OBJECT_BASE_HOLE_1, OBJECT_BASE_HOLE_3, OBJECT_BASE_HOLE_4, OBJECT_BASE_HOLE_2 },
		nMaxBaseTipArea = 1200;

	CObject_f obj_base( OBJ_TYPE );

	const unsigned int nBaseMinArea = obj_base.uMinArea;

	//--------------------------------------------------------------------------

	bool 
		bOverlapped = false,
		bHoleIdentified = false;

	unsigned int 
		i = 0, 
		iObj = 0, 
		iColour = 0, 
		iType = 0,
		nArea = 0;
	
	unsigned int nObjArea = 0;

	cv::Rect
		rect1,
		rect2,
		rect_hole;

	int
		iContour = 0,
		iChild = 0;
	
	// Set an iterator to go through Red spots vector of segmented image
	std::vector<cv::Rect>::iterator 
		itRect = vSegmentedImage.at( OBJ_COLOUR ).vSpots.begin(),
		itRect2;
	
	// Clear objects from previous iteration
	pDataSource->vObjects.Obj.at( OBJ_COLOUR ).clear();

	// If there are no red spots, there are no bases.
	if( vSegmentedImage.at( OBJ_COLOUR ).vSpots.size() < 1 ) return false;

	while( iContour != -1 )
	{
		nObjArea = static_cast<unsigned int>( itRect->area() );

		if( nObjArea < nBaseMinArea )
		{
			//It's not big enough to be a Base, go to next red spot.
			itRect ++;
			iContour = vSegmentedImage.at( OBJ_COLOUR ).vHierarchy.at( iContour )[0];
			continue;
		}

		// Create an object with the region rect and type Base on Object vector in DataSource
		pDataSource->vObjects.Obj.at( OBJ_TYPE ).emplace_back( CObject_f( OBJ_TYPE, *itRect ) );
		pDataSource->vObjects.Obj.at( OBJ_TYPE ).back().Colour_Primary = OBJ_COLOUR;
		
		// Clear vector with sub-objects
		pDataSource->vObjects.Obj.at( OBJ_TYPE ).back().vSubObjects.clear();
		
		// Try to indentify were are the holes
		// Point to the first child contour
		iChild = vSegmentedImage.at( OBJ_COLOUR ).vHierarchy.at( iContour )[2];

		// Go through each hole to identify them.
		while( iChild != -1 )
		{
			rect_hole = cv::boundingRect( cv::Mat( vSegmentedImage.at( OBJ_COLOUR ).vContours.at( iChild ) ) );

			bHoleIdentified = false;

			for( i=0 ; i<3 ; i++ )
			{
				// Compare the hole with each of the colours
				iColour = HOLE_COLOUR[i];
				iType = HOLE_TYPE[i];
				
				itRect2 = vSegmentedImage.at( iColour ).vSpots.begin();

				while( ( itRect2 != vSegmentedImage.at( iColour ).vSpots.end() ) && !bHoleIdentified )
				{
					rect1 = rect_hole;
					rect2 = *itRect2;
					bOverlapped = CheckOverlapRegion( &rect1, &rect2 );
					
					if( !bOverlapped )
					{
						itRect2++;
						continue;
					}

					// Add to the current Base being processed one Hole of type 'iType'
					pDataSource->vObjects.Obj.at( OBJ_TYPE ).back().vSubObjects.emplace_back( 
						CObject_f( iType, rect_hole ) 
						);
					
					// Erase this spot, to prevent being processed again.
					itRect2 = vSegmentedImage.at( iColour ).vSpots.erase( itRect2 );
					
					bHoleIdentified = true;
					
					break;
				}

				if( bHoleIdentified ) break;
			}
			
			if( !bHoleIdentified )
			{
				// if the hole isn't any of the previous colours, then it's red
				pDataSource->vObjects.Obj.at( OBJ_TYPE ).back().vSubObjects.emplace_back( 
					CObject_f( HOLE_TYPE[3], rect_hole ) 
					);
			}

			// Go to next contour (hole)
			iChild = vSegmentedImage.at( OBJ_COLOUR ).vHierarchy.at( iChild )[0];
		}

		// Let's find the base tips.
		// This code is commented because the base tips are no longer used, they are not green anymore
		/*
		itRect2 = vSegmentedImage.at( COLOUR_G ).vSpots.begin();

		while( itRect2 != vSegmentedImage.at( COLOUR_G ).vSpots.end() )
		{
			nArea = itRect2->area();

			if( nArea >= nMaxBaseTipArea )
			{
				itRect2++;
				continue;
			}
			
			rect1 = *itRect;
			rect2 = *itRect2;

			bOverlapped = CheckOverlapRegion( &rect1, &rect2 );

			if( bOverlapped ) 
			{
				pDataSource->vObjects.Obj.at( OBJ_TYPE ).back().vSubObjects.emplace_back(
					CObject_f( OBJECT_BASE_TIP, *itRect2 )
					);

				//Remove the current green spot.
				itRect2 = vSegmentedImage.at( COLOUR_G ).vSpots.erase( itRect2 );
			}

			else
			{
				itRect2++;
			}
		}*/

		//Remove the current red spot, to avoid being processed again by searching other objects.
		itRect = vSegmentedImage.at( OBJ_COLOUR ).vSpots.erase( itRect );
		
		// Point to the next contour on the same level
		iContour = vSegmentedImage.at( OBJ_COLOUR ).vHierarchy.at( iContour )[0];
	}

	return ( pDataSource->vObjects.Obj.at( OBJ_TYPE ).size() > 0 );
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Column( void )
{
	//--------------------------------------------------------------------------
	// Column detection parameters
	const int OBJ_TYPE[4] = { OBJECT_COLUMN_1, OBJECT_COLUMN_2, OBJECT_COLUMN_3, OBJECT_COLUMN_4 };

	const int 
		OBJ_MAIN_COLOUR = GET_VALID_PrimaryColour( OBJECT_COLUMN_1 ),
		SEC_OBJECT[4] = { OBJECT_SPOT_GREEN, OBJECT_SPOT_RED, OBJECT_SPOT_MAGENTA, OBJECT_SPOT_BLUE },
		SEC_COLOUR[4] = { 
			GET_VALID_SecondaryColour( OBJ_TYPE[0] ), 
			GET_VALID_SecondaryColour( OBJ_TYPE[1] ),
			GET_VALID_SecondaryColour( OBJ_TYPE[2] ),
			GET_VALID_SecondaryColour( OBJ_TYPE[3] ) };

	//--------------------------------------------------------------------------
	// Auxiliary variables
	bool 
		bFound = false,
		bIdentified = false,
		bOverlapped = false,
		bSpotUsed = false;

	int 
		iColour = 0,
		iSecColour = 0;

	cv::Rect rect_max;

	// Create one auxiliary object with no area.
	CObject_f 
		obj( OBJECT_COLUMN ),
		obj_spot;

	std::vector< cv::Rect >::iterator 
		itRect, 
		itRect2;

	//--------------------------------------------------------------------------
	// Clear objects from previous iteration
	for( unsigned int iObjType=OBJECT_COLUMN ; iObjType<=OBJECT_COLUMN_4 ; iObjType++ )
		pDataSource->vObjects.Obj.at( iObjType ).clear();

	//--------------------------------------------------------------------------
	itRect = vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.begin();

	while( itRect != vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.end() )
	{
		bSpotUsed = false;

		//Clear object properties
		obj.ClearObject();

		// Set the column area to the rectangle occupied by the yellow spot.
		obj.vSpot_Primary.emplace_back( *itRect );
		obj.set_Region( *itRect );

		// Go through all the secondary colours.
		for( iColour=0 ; iColour<4 ; iColour++ )
		{
			bIdentified = false;

			iSecColour = SEC_COLOUR[iColour];

			itRect2 = vSegmentedImage.at( iSecColour ).vSpots.begin();
	
			// Go through all the spots of the secondary colour.
			while( itRect2 != vSegmentedImage.at( iSecColour ).vSpots.end() )
			{
				bOverlapped = CheckOverlapSpot(
					&vSegmentedImage.at( OBJ_MAIN_COLOUR ).Image,	//Image 1
					&( *itRect ),									//Spot in image 1
					&vSegmentedImage.at( iSecColour ).Image,		//Image 2
					&( *itRect2 )									//Spot in image 2
					);

				if( bOverlapped )
				{
					bIdentified = true;
					bSpotUsed = true;

					// The column area is the sum of all the spots that compose it.
					rect_max = MaxRect( obj.Region, *itRect2 );
					obj.set_Region( rect_max );
					obj.set_Type( OBJ_TYPE[iColour] );
					
					// Define the sub object
					obj.vSubObjects.emplace_back( CObject_f( SEC_OBJECT[iColour], *itRect2 ) );

					itRect2 = vSegmentedImage.at( iSecColour ).vSpots.erase( itRect2 );
				}
				else
				{
					itRect2++;
				}
			}

			// If one colour is identified as being part of this column, skip the others.
			if( bIdentified ) break;
		}

		// Test to see if the current Yellow spot was used.
		if( bSpotUsed )
		{
			// It was used, erase it, and go to the next one.
			itRect = vSegmentedImage.at( OBJ_MAIN_COLOUR ).vSpots.erase( itRect );

			// Sort the columns sub objects by size. Smaller first.
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

bool CProcessImageToyRobotFull_SVS::SearchObjects_Wheel( void )
{
	// I can use simple colour association, other green spots were used already. So what's left are wheels.
	return Detect_Object_From_Primary_Colour( OBJECT_WHEEL, 1200 );
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Nut( void )
{
	// I can use simple colour association, other magenta spots were used already. So what's left are nuts.
	return Detect_Object_From_Primary_Colour( OBJECT_NUT, 100 );
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Hand( void )
{
	bool bResult = false;

	if( HandTracker.isReady() )
	{
		// Use hand tracker
		HandTracker.TrackObjects( &vSegmentedImage.at( COLOUR_C ).Image );

		HandTracker.get_Tracked_Objects_Vector( pDataSource->vObjects.Obj.at( OBJECT_HAND ) );

		bResult = ( pDataSource->vObjects.Obj.at( OBJECT_HAND ).size() > 0 );
	}
	
	else
	{
		// Use simple colour association
		bResult = Detect_Object_From_Primary_Colour( OBJECT_HAND );
	}

	Define_New_Region_of_Interest( OBJECT_HAND, cv::Rect( -40, 5, 80, 90 ) );

	return bResult;
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Top_Floor( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_TOP_FLOOR, 5000 );
}

bool CProcessImageToyRobotFull_SVS::SearchObjects_Auxiliary( void )
{
	//--------------------------------------------------------------------------
	// Search for small red objects (inserted column 2)
	CObject_f obj_aux( OBJECT_BASE );

	bool bFound = Detect_Object_From_Primary_Colour( OBJECT_RED, 0, obj_aux.uMinArea - 1 );

	//--------------------------------------------------------------------------
	// Search for small blue objects (inserted column 4)
	obj_aux.set_Type( OBJECT_TOP_FLOOR );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BLUE, 0, obj_aux.uMinArea - 1 );

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageToyRobotFull_SVS::ObjectPreProcessing( void )
{
	Generate_Object_Masks();

	// Detection of inserted objects.
	LookForNearObjects( OBJECT_BASE, OBJECT_WHEEL );

	LookForNearObjects( OBJECT_BASE, OBJECT_NUT );

	//Detection of hand near an object
	LookForNearObjects( OBJECT_HAND, OBJECT_WHEEL );

	LookForNearObjects( OBJECT_HAND, OBJECT_NUT );

	LookForNearObjects( OBJECT_HAND, OBJECT_COLUMN_1 );

	LookForNearObjects( OBJECT_HAND, OBJECT_COLUMN_2 );

	LookForNearObjects( OBJECT_HAND, OBJECT_COLUMN_3 );

	LookForNearObjects( OBJECT_HAND, OBJECT_COLUMN_4 );

	LookForNearObjects( OBJECT_HAND, OBJECT_TOP_FLOOR );
}

void CProcessImageToyRobotFull_SVS::ObjectPostProcessing( void )
{
	// -------------------------------------------------------------------------
	// Get Column orientation

	const unsigned int 
		nColumnType = 5,
		COLUMNS[nColumnType] = { OBJECT_COLUMN, OBJECT_COLUMN_1, OBJECT_COLUMN_2, OBJECT_COLUMN_3, OBJECT_COLUMN_4 };
	
	size_t
		iColumnType = 0,
		iColumn = 0,
		nColumns = 0,
		iColIndex = 0;

	for( iColumnType=0 ; iColumnType<nColumnType ; iColumnType++ )
	{
		iColIndex = COLUMNS[iColumnType];
		nColumns = pDataSource->vObjects.Obj.at( iColIndex ).size();

		for( iColumn=0 ; iColumn<nColumns ; iColumn++ )
		{
			CalculateOrientation_Column( &pDataSource->vObjects.Obj.at( iColIndex ).at( iColumn ) );
		}
	}
	
	// -------------------------------------------------------------------------
	// Get Base orientation

	size_t
		iBase = 0,
		nBases = pDataSource->vObjects.Obj.at( OBJECT_BASE ).size();
		
	for( iBase=0 ; iBase<nBases ; iBase++ )
	{
		CalculateOrientation_Base( &pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( iBase ) );
	}

	// -------------------------------------------------------------------------
	// Interpret inserted objects
	Action_Interpretation();

	// -------------------------------------------------------------------------
	Detect_Gestures();
}

// Process the image and retrieve data
void CProcessImageToyRobotFull_SVS::ProcessTask( cv::Mat * mat_ptr )
{	
	//Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );
		
	SegmentMainColours_cv();

	for( unsigned int nColour=0 ; nColour<COLOUR_W ; nColour++ )
	{
		vSegmentedImage.at( nColour ).doMorphologicalOperations();
	}
		
	DetectMainColourSpots();

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
}

void CProcessImageToyRobotFull_SVS::CalculateOrientation_Base( CObject_f * pBase )
{
	// if there's no 3D data, then there's no point in continuing.
	if( !pBase->has3D() ) return;

	std::vector<unsigned int> vHoleID;
	vHoleID.emplace_back( OBJECT_BASE_HOLE_1 );
	vHoleID.emplace_back( OBJECT_BASE_HOLE_2 );
	vHoleID.emplace_back( OBJECT_BASE_HOLE_3 );
	vHoleID.emplace_back( OBJECT_BASE_HOLE_4 );

	const size_t number_holes = vHoleID.size();

	std::vector< CObject_f >::iterator itHole;

	// Hole existence flags
	std::vector<bool> vHole( number_holes, false );

	// Hole coordinates
	std::vector< cv::Point3f > vHoleCoord( number_holes, cv::Point3f( 0.0f, 0.0f, 0.0f ) );
	
	cv::Point3f point_ori( 0.0f, 0.0f, 0.0f );

	// Find each hole
	for( size_t iHole = 0; iHole < number_holes; iHole++ )
	{
		itHole = std::find( pBase->vSubObjects.begin(), pBase->vSubObjects.end(), vHoleID.at( iHole ) );

		if( itHole != pBase->vSubObjects.end() )
		{
			itHole->get_Coordinates_World( &vHoleCoord.at( iHole ) );
			vHole.at( iHole ) = ( vHoleCoord.at( iHole ).x != 0.0f ) && ( vHoleCoord.at( iHole ).y != 0.0f ) && ( vHoleCoord.at( iHole ).z != 0.0f );
		}
	}

	float 
		fDeltaX = 0.0f,
		fDeltaY = 0.0f,
		fPhi = 0.0f;

	// Choose how to calculate orientation, according to holes availability

	// Holes 1 and 2
	if( vHole.at( 0 ) && vHole.at( 1 ) )
	{
		fDeltaX = vHoleCoord.at( 1 ).x - vHoleCoord.at( 0 ).x;
		fDeltaY = vHoleCoord.at( 1 ).y - vHoleCoord.at( 0 ).y;

		fPhi = M_PI_2_F - atan2( fDeltaY, fDeltaX );
	}

	// Holes 1 and 3
	else if( vHole.at( 0 ) && vHole.at( 2 ) )
	{
		fDeltaX = vHoleCoord.at( 2 ).x - vHoleCoord.at( 0 ).x;
		fDeltaY = vHoleCoord.at( 2 ).y - vHoleCoord.at( 0 ).y;

		fPhi = M_PI_4_F + atan2( fDeltaY, fDeltaX );
	}

	// Holes 1 and 4
	else if( vHole.at( 0 ) && vHole.at( 3 ) )
	{
		fDeltaX = vHoleCoord.at( 3 ).x - vHoleCoord.at( 0 ).x;
		fDeltaY = vHoleCoord.at( 3 ).y - vHoleCoord.at( 0 ).y;

		fPhi = atan2( fDeltaX, fDeltaY );
	}

	// Holes 2 and 3
	else if( vHole.at( 1 ) && vHole.at( 2 ) )
	{
		fDeltaX = vHoleCoord.at( 2 ).x - vHoleCoord.at( 1 ).x;
		fDeltaY = vHoleCoord.at( 2 ).y - vHoleCoord.at( 1 ).y;

		fPhi = atan2( fDeltaX, fDeltaY );
	}
	
	// Holes 2 and 4
	else if( vHole.at( 1 ) && vHole.at( 3 ) )
	{
		fDeltaX = vHoleCoord.at( 1 ).x - vHoleCoord.at( 3 ).x;
		fDeltaY = vHoleCoord.at( 1 ).y - vHoleCoord.at( 3 ).y;

		fPhi = M_PI_4_F + atan2( fDeltaX, fDeltaY );
	}

	// Holes 3 and 4
	else if( vHole.at( 2 ) && vHole.at( 3 ) )
	{
		fDeltaX = vHoleCoord.at( 2 ).x - vHoleCoord.at( 3 ).x;
		fDeltaY = vHoleCoord.at( 2 ).y - vHoleCoord.at( 3 ).y;

		fPhi = M_PI_2_F - atan2( fDeltaY, fDeltaX );
	}
	
	// orientation is -90 - 'angle' to be compliant with movement planning
	point_ori.z = -90.0f - fPhi*RAD_TO_DEG_F;

	//--------------------------------------------------------------------------
	// Fit the orientation in the range 0º - 360º

	point_ori.x = modAny( point_ori.x, 360.0f );
	point_ori.y = modAny( point_ori.y, 360.0f );
	point_ori.z = modAny( point_ori.z, 360.0f );

	//--------------------------------------------------------------------------
	pBase->set_Orientation( point_ori );
}

void CProcessImageToyRobotFull_SVS::CalculateOrientation_Column( CObject_f * pColumn )
{
	// if there's no 3D data, then there's no point in continuing.
	if( !pColumn->has3D() ) return;

	// How many spots of the secondary colour this column has?
	size_t uSpots = pColumn->vSubObjects.size();

	// Are there enough spots to calculate orientation
	if( uSpots <= 0 ) return;

	cv::Point3f
		point_spot_small( 0.0f, 0.0f, 0.0f ),	// The big colour spot
		point_spot_big( 0.0f, 0.0f, 0.0f ),	// The small colour spot
		point_ori( 0.0f, 0.0f, 0.0f );			// The orientation

	switch( uSpots )
	{
	default:
	case 1: // One spot
		// Use main spot and the secondary spot to calculate orientation
		pColumn->vSubObjects.front().get_Coordinates_World( &point_spot_big );
		pColumn->get_Coordinates_World( &point_spot_small );
		break;

	// This section is commented because it was causing incorrect results,
	// the detection of the small tip on the columns has no effect in calculating the orientation
	//default: // Two or more spots
		// Use the two biggest spots for calculation, the vector is sorted, so, it's the two first.
		//pColumn->vSubObjects.at( 0 ).get_Coordinates_World( &point_spot_big );
		//pColumn->vSubObjects.at( 1 ).get_Coordinates_World( &point_spot_small );
		//break;
	}

	point_ori.x = atan2( point_spot_big.z - point_spot_small.z, point_spot_big.y - point_spot_small.y ) * RAD_TO_DEG_F - 90.0f;

	point_ori.y = atan2( point_spot_big.z - point_spot_small.z, point_spot_big.x - point_spot_small.x ) * RAD_TO_DEG_F;

	// orientation is decremented 90º to be compliant with movement planning
	point_ori.z = atan2( point_spot_small.y - point_spot_big.y, point_spot_small.x - point_spot_big.x ) * RAD_TO_DEG_F - 90.0f;

	//--------------------------------------------------------------------------
	// Fit the orientation in the range 0º - 360º
	point_ori.x = modAny( point_ori.x, 360.0f );
	point_ori.y = modAny( point_ori.y, 360.0f );
	point_ori.z = modAny( point_ori.z, 360.0f );

	//--------------------------------------------------------------------------
	pColumn->set_Orientation( point_ori );
}

void CProcessImageToyRobotFull_SVS::Action_Interpretation( void )
{
	//--------------------------------------------------------------------------
	// Inserted Objects on the base: wheels and nuts
	Check_Objects_Inserted_in_Base();

	//--------------------------------------------------------------------------
	// Inserted Objects on the base: columns
	Check_Columns_Insertion();

	//--------------------------------------------------------------------------
	// Test Top Floor insertion.
	// Inserted when above a certain height
	size_t n = pDataSource->vObjects.Obj.at( OBJECT_TOP_FLOOR ).size();

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	for( size_t i = 0; i<n; i++ )
	{
		pDataSource->vObjects.Obj.at( OBJECT_TOP_FLOOR ).at( i ).get_Coordinates_World( &coord );

		if( coord.z > HEIGHT_SECOND_FLOOR )
			pDataSource->vObjects.Obj.at( OBJECT_TOP_FLOOR ).at( i ).bInserted = true;
	}
}

void CProcessImageToyRobotFull_SVS::Check_Objects_Inserted_in_Base( void )
{
	size_t
		i = 0, 
		j = 0, 
		n = 0, 
		m = 0;

	int
		nSide = 0, 
		type = 0;

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	pDataSource->vInsertedObjects.lock();

	pDataSource->vInsertedObjects.Obj.at( Side::HUMAN-1 ).clear();
	pDataSource->vInsertedObjects.Obj.at( Side::ROBOT-1 ).clear();

	n = pDataSource->vObjects.Obj.at( OBJECT_BASE ).size();
	for( i=0 ; i<n ; i++ )
	{
		m = pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.size();
		for( j=0 ; j<m ; j++ )
		{
			if( !pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).has3D() ) continue;

			pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).get_Coordinates_World( &coord );
					
			nSide = get_Workspace_Position( coord.x );

			type = pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).get_Type();
			
			if( type == OBJECT_WHEEL || type == OBJECT_NUT )
			{
				pDataSource->vInsertedObjects.Obj.at( nSide-1 ).emplace_back( type );
			}
		}
	}

	pDataSource->vInsertedObjects.unlock();
}

void CProcessImageToyRobotFull_SVS::Check_Columns_Insertion( void )
{
	const int
		// Objects that will be treated as columns (1, 2, 3, 4) respectively
		OBJ_COLUMN_FAKE[4] = { OBJECT_WHEEL, OBJECT_RED, OBJECT_NUT, OBJECT_BLUE },
		// The real columns
		OBJ_COLUMN_REAL[4] = { OBJECT_COLUMN_1, OBJECT_COLUMN_2, OBJECT_COLUMN_3, OBJECT_COLUMN_4 },
		// Identification of the base holes
		OBJ_HOLE[4] = { OBJECT_BASE_HOLE_1, OBJECT_BASE_HOLE_2, OBJECT_BASE_HOLE_3, OBJECT_BASE_HOLE_4 };

	const std::pair< cv::Point3f, cv::Point3f > ptMargin(
		cv::Point3f( 50.0f, 50.0f, -100.0f ),	// Lower limits (mm)
		cv::Point3f( 50.0f, 50.0f, 400.0f ) );	// Upper limits (mm)
		
	//--------------------------------------------------------------------------

	pDataSource->Update_LastKnownGoodBase();

	cv::Point3f
		ptCoord_Col( 0.0f, 0.0f, 0.0f ),
		ptCoord_Hole( 0.0f, 0.0f, 0.0f );

	std::vector< cv::Point3f > vCoordHole;

	std::vector< std::pair< cv::Point3f, cv::Point3f > > vCoordHole_limits;

	size_t num_holes = pDataSource->objBase.vSubObjects.size();
	
	// Get coordinates of the base holes
	for( size_t i = 0; i < num_holes; i++ )
	{
		pDataSource->objBase.vSubObjects.at( i ).get_Coordinates_World( &ptCoord_Hole );
		vCoordHole.emplace_back( ptCoord_Hole );
		
		// Define upper and lower limits
		vCoordHole_limits.emplace_back( std::pair< cv::Point3f, cv::Point3f >( ptCoord_Hole, ptCoord_Hole ) );
		vCoordHole_limits.back().first -= ptMargin.first;
		vCoordHole_limits.back().second += ptMargin.second;
	}

	const int nObjects = sizeof( OBJ_COLUMN_FAKE ) / sizeof( const int );

	int col_type = 0;

	pDataSource->Column_InsertedAt.Obj.clear();

	// Go through all types of objects being treated as columns
	for( int iObj = 0; iObj < nObjects; iObj++ )
	{
		col_type = OBJ_COLUMN_FAKE[iObj];

		// How many "columns" do we have of this type?
		// If none, skip to the next type
		if( pDataSource->vObjects.Obj.at( col_type ).size() <= 0 ) continue;

		for( size_t iCol = 0; iCol < pDataSource->vObjects.Obj.at( col_type ).size(); iCol++ )
		{
			pDataSource->vObjects.Obj.at( col_type ).at( iCol ).get_Coordinates_World( &ptCoord_Col );

			// If the X coordinate is zero (This would be inside the robots' body),
			// the object is invalid, continue to the next one
			if( ptCoord_Col.x == 0.0f ) continue;

			for( size_t iHole = 0; iHole < num_holes; iHole++ )
			{
				if( in_range( ptCoord_Col, vCoordHole_limits.at( iHole ) ) )
				{
					// this column is inserted in the hole, push a pair of ( Column id, Hole id )
					pDataSource->Column_InsertedAt.Obj.emplace_back(
						std::pair<int, int>( OBJ_COLUMN_REAL[iObj], iHole + OBJECT_BASE_HOLE_1 ) );

					// Setup what hole the column is inserted at
					pDataSource->vObjects.Obj.at( col_type ).at( iCol ).vInsertedAt.emplace_back( iHole + OBJECT_BASE_HOLE_1 );

					//-------------------------------------------------------------------------
					// Add a column in the objects vector
					pDataSource->vObjects.Obj[ OBJ_COLUMN_REAL[iObj] ].emplace_back( OBJ_COLUMN_REAL[iObj] );
					
					// The column is inserted
					pDataSource->vObjects.Obj[OBJ_COLUMN_REAL[iObj]].back().bInserted = true;

					// The column is inserted in this hole
					pDataSource->vObjects.Obj[OBJ_COLUMN_REAL[iObj]].back().vInsertedAt.emplace_back( iHole + OBJECT_BASE_HOLE_1 );
					
					// Take the coordinates of the hole it is inserted at and add 15cm in height
					ptCoord_Hole = vCoordHole.at( iHole );
					ptCoord_Hole.z += 150.0f;

					// Set the coordinates
					pDataSource->vObjects.Obj[OBJ_COLUMN_REAL[iObj]].back().set_Coordinates_World( ptCoord_Hole );
					
					// Because the column is inserted the orientation is '0' in all axes
					pDataSource->vObjects.Obj[OBJ_COLUMN_REAL[iObj]].back().set_Orientation( cv::Point3f( 0.0f, 0.0f, 0.0f ) );

					// Set the region
					pDataSource->vObjects.Obj[OBJ_COLUMN_REAL[iObj]].back().set_Region( pDataSource->vObjects.Obj.at( col_type ).at( iCol ).Region );
					
					// Hide the object
					pDataSource->vObjects.Obj.at( col_type ).at( iCol ).bVisible = false;
				}
			}
		}
	}
}
