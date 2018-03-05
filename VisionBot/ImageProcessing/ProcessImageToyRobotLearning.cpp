#include "ProcessImageToyRobotLearning.h"
#include "commands.h"
#include "MutexUtils.hpp"
#include "range.hpp"
#include "vector_operations.hpp"

CProcessImageToyRobotLearning::CProcessImageToyRobotLearning( int width, int height )
: CProcessImageToyRobotFull_SVS( width, height )
{
	sSegFile = "Videre/Segmentation_Parameters (Toy Robot Learning).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ToyRobotLearning;
}

bool CProcessImageToyRobotLearning::SearchObjects_Hand( void )
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

	Define_New_Region_of_Interest( OBJECT_HAND, cv::Rect( -10, -10, 50, 80 ) );

	return bResult;
}

bool CProcessImageToyRobotLearning::SearchObjects_Top_Floor( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_TOP_FLOOR, 5001 );
}

bool CProcessImageToyRobotLearning::SearchAllObjects( void )
{
	bool bFound = false;

	// The order for object detection matters!
	// Base -> Columns -> Wheels -> Nuts

	//--------------------------------------------------------------------------
	// Search for the Base
	if( SearchObjects_Base() ) bFound = true;
	
	//--------------------------------------------------------------------------
	// Search for the Columns
	// This is disabled to allow the detection of inserted columns for this specific task
	//if( SearchObjects_Column() ) bFound = true;

	//--------------------------------------------------------------------------
	// Search for the Wheels
	if( SearchObjects_Wheel() ) bFound = true;

	//--------------------------------------------------------------------------
	// Search for the Nuts
	if( SearchObjects_Nut() ) bFound = true;

	//--------------------------------------------------------------------------
	// Search for the Top Floor
	if( SearchObjects_Top_Floor() ) bFound = true;
	
	//--------------------------------------------------------------------------
	// Search for the Hands
	//if( SearchObjects_Hand() ) bFound = true;

	//--------------------------------------------------------------------------
	// Other objects used to identify columns
	if( Detect_Object_From_Primary_Colour( OBJECT_BLUE, 0, 5000 ) ) bFound = true;

	if( Detect_Object_From_Primary_Colour( OBJECT_RED, 0, 4999 ) ) bFound = true;

	return bFound;
}

void CProcessImageToyRobotLearning::ObjectPreProcessing( void )
{
	// Detection of inserted objects.
	LookForNearObjects( OBJECT_BASE, OBJECT_WHEEL );

	LookForNearObjects( OBJECT_WHEEL, OBJECT_NUT );
}

void CProcessImageToyRobotLearning::Action_Interpretation( void )
{
	size_t
		i = 0, 
		j = 0,
		k = 0,
		n = 0, 
		m = 0;

	int
		nSide = 0, 
		type = 0;

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	std::pair< cv::Point3f, cv::Point3f > coord_base_limit( 
		cv::Point3f( 500.0f, -300.0f, 800.0f ),		// Min
		cv::Point3f( 900.0f, 100.0f, 1200.0f ) );	// Max

	//--------------------------------------------------------------------------
	// Action Inserted Objects
	// Create a vector with same dimension to temporarily store task status
	
	pDataSource->vTaskStatus.lock();
	std::vector< unsigned int > vStatus( pDataSource->vTaskStatus.size(), Side::NOT_INSERTED );
	pDataSource->vTaskStatus.unlock();

	if( vStatus.size() < 1 ) return;

	// Wheels inserted in the base
	n = pDataSource->vObjects.Obj.at( OBJECT_BASE ).size();
	for( i=0 ; i<n ; i++ )
	{
		pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).get_Coordinates_World( &coord );

		// Test if the base is in place
		if( in_range( coord, coord_base_limit ) )
		{
			vStatus.at( LearnTaskObj::BA ) = Side::INSERTED;
		}
		
		// Process inserted objects in this base
		m = pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.size();
		for( j=0 ; j<m ; j++ )
		{
			if( !pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).has3D() ) continue;

			type = pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).get_Type();

			pDataSource->vObjects.Obj.at( OBJECT_BASE ).at( i ).vNearObjects.at( j ).get_Coordinates_World( &coord );
					
			switch( type )
			{
			case OBJECT_WHEEL:
				if( coord.z > HEIGHT_SECOND_FLOOR ) break;
				if( coord.y < 0.0f ) 
					vStatus.at( LearnTaskObj::LW ) = Side::INSERTED;
				else
					vStatus.at( LearnTaskObj::RW ) = Side::INSERTED;
				break;

			default:
				break;
			}
		}
	}

	//--------------------------------------------------------------------------
	// Nuts inserted in wheels
	n = pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).size();
	for( i=0 ; i<n ; i++ )
	{
		m = pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).at( i ).vNearObjects.size();
		for( j=0 ; j<m ; j++ )
		{
			if( !pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).at( i ).vNearObjects.at( j ).has3D() ) continue;

			type = pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).at( i ).vNearObjects.at( j ).get_Type();

			pDataSource->vObjects.Obj.at( OBJECT_WHEEL ).at( i ).vNearObjects.at( j ).get_Coordinates_World( &coord );
					
			switch( type )
			{
			case OBJECT_NUT:
				if( coord.z > HEIGHT_SECOND_FLOOR ) break;
				if( coord.y < 0.0f ) 
					vStatus[ LearnTaskObj::LN ] = Side::INSERTED;
				else
					vStatus[ LearnTaskObj::RN ] = Side::INSERTED;
				break;

			default:
				break;
			}
		}
	}

	//--------------------------------------------------------------------------
	// Look at objects of colours 'R', 'G', 'B' and 'M'. Analyse their positions to infer inserted columns
	const int 
		OBJ[4] = { OBJECT_WHEEL, OBJECT_RED, OBJECT_NUT, OBJECT_BLUE },
		COL_INDEX[4] = { LearnTaskObj::C1, LearnTaskObj::C2, LearnTaskObj::C3, LearnTaskObj::C4 };
	
	int 
		iObject = 0,
		iCol = 0;

	for( j=0 ; j<4 ; j++ )
	{
		iObject = OBJ[j];
		iCol = COL_INDEX[j];
		n = pDataSource->vObjects.Obj.at( iObject ).size();
		for( i=0 ; i<n ; i++ )
		{
			pDataSource->vObjects.Obj.at( iObject ).at( i ).get_Coordinates_World( &coord );
			if( coord.z > HEIGHT_SECOND_FLOOR )
				vStatus[iCol] = Side::INSERTED;
		}
	}

	//--------------------------------------------------------------------------
	// Look at top floor position, if 'Z' component is above HEIGHT_SECOND_FLOOR, it's inserted
	n = pDataSource->vObjects.Obj.at( OBJECT_TOP_FLOOR ).size();
	for( i=0 ; i<n ; i++ )
	{
		pDataSource->vObjects.Obj.at( OBJECT_TOP_FLOOR ).at( i ).get_Coordinates_World( &coord );
		if( coord.z > HEIGHT_SECOND_FLOOR )
			vStatus[LearnTaskObj::TF] = Side::INSERTED;
	}

	//--------------------------------------------------------------------------
	// At once copy all status to DataSource
	mutex_vector_copy( vStatus, pDataSource->vTaskStatus );
}
