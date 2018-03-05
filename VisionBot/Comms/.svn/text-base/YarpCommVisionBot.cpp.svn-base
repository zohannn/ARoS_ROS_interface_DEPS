#ifdef USE_YarpCommunication

#pragma once

#include <math.h>

#include "common.h"
#include "Data.h"
#include "MutexUtils.hpp"
#include "pantilt_definitions.h"
#include "ProcessImage.h"
#include "vector_operations.hpp"
#include "VisionBotDlg.h"
#include "YarpCommVisionBot.h"

CYarpCommVisionBot::CYarpCommVisionBot( const std::string name, const std::string peerName, bool isServer )
: CYarpCommunication( name, peerName, isServer )
, pDataSource( nullptr )
, pYarpComm_PanTilt( nullptr )
, visionbot_dlg_ptr( nullptr )
{
}

CYarpCommVisionBot::~CYarpCommVisionBot( void )
{
	pDataSource = nullptr;
	pYarpComm_PanTilt = nullptr;
	visionbot_dlg_ptr = nullptr;
}

void CYarpCommVisionBot::set_DataSource( void *pData )
{
	pDataSource = pData;
}

void CYarpCommVisionBot::set_YarpComm_PanTilt_Pointer( CYarpCommunication * pYarpComm )
{
	pYarpComm_PanTilt = pYarpComm;
}

void CYarpCommVisionBot::set_VisionBotDlg_Ptr( void * visionbot_ptr )
{
	visionbot_dlg_ptr = visionbot_ptr;
}

void CYarpCommVisionBot::get_Position_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	size_t
		i = 0,
		number_objects = 0;

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );
	
	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	
	number_objects = pData->vObjects_Shared.Obj.at( object ).size();
		
	msg->uParam.assign( 1, 0 );

	for( i=0 ; i<number_objects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Coordinates_World( &coord );

		// Position X, Y, Z
		msg->fData.emplace_back( coord.x / 10.0f );
		msg->fData.emplace_back( coord.y / 10.0f );
		msg->fData.emplace_back( coord.z / 10.0f );
	}

	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::get_Orientation_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	size_t
		i = 0, 
		number_objects = 0;

	cv::Point3f ori( 0.0f, 0.0f, 0.0f );

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	
	number_objects = pData->vObjects_Shared.Obj.at( object ).size();
	
	msg->uParam.assign( 1, 0 );

	for( i=0 ; i<number_objects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Orientation( &ori );

		// Orientation Phi, Psi, Theta
		msg->fData.emplace_back( ori.z );
		msg->fData.emplace_back( ori.y );
		msg->fData.emplace_back( ori.x );
	}

	pData->vObjects_Shared.unlock();
}


void CYarpCommVisionBot::get_PositionOrientation_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;
	
	size_t
		i = 0, 
		number_objects = 0;
	
	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	
	number_objects = pData->vObjects_Shared.Obj.at( object ).size();
	
	msg->uParam.assign( 1, 0 );

	for( i=0 ; i<number_objects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Coordinates_World( &coord );

		// Position X, Y, Z
		msg->fData.emplace_back( coord.x / 10.0f );
		msg->fData.emplace_back( coord.y / 10.0f );
		msg->fData.emplace_back( coord.z / 10.0f );

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Orientation( &coord );

		// Orientation Phi
		msg->fData.emplace_back( coord.z );
	}

	pData->vObjects_Shared.unlock();
}


void CYarpCommVisionBot::get_PositionOrientationInserted_ObjectType( unsigned int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;
	
	size_t
		i = 0, 
		number_objects = 0;
	
	unsigned int Col_Hole_Ins = 0;

	const unsigned int COL_HOLE_ID_DIFF = OBJECT_COLUMN_1 - OBJECT_BASE_HOLE_1;

	cv::Point3f 
		coord( 0.0f, 0.0f, 0.0f ),
		ori( 0.0f, 0.0f, 0.0f );

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	
	number_objects = pData->vObjects_Shared.Obj.at( object ).size();

	msg->uParam.assign( 1, 0 );

	bool bColInserted = false;

	for( i=0 ; i<number_objects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Coordinates_World( &coord );

		// Position X, Y, Z
		msg->fData.emplace_back( coord.x/10.0f );
		msg->fData.emplace_back( coord.y/10.0f );
		msg->fData.emplace_back( coord.z/10.0f );

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_Orientation( &ori );

		// Orientation Phi
		msg->fData.emplace_back( ori.z );

		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bInserted )
			msg->fData.emplace_back( static_cast<float>( Side::NOT_INSERTED ) );
		else
		{
			// Insertion on Base
			switch( object )
			{
			case OBJECT_WHEEL:
			case OBJECT_NUT:
				msg->fData.emplace_back( static_cast<float>( get_Workspace_Position( coord.x ) ) );
				break;

			case OBJECT_COLUMN_1:
			case OBJECT_COLUMN_2:
			case OBJECT_COLUMN_3:
			case OBJECT_COLUMN_4:
				for( size_t iCol = 0; iCol < pData->vObjects_Shared.Obj.at( object ).at( i ).vInsertedAt.size(); iCol++ )
				{
					Col_Hole_Ins = object - pData->vObjects_Shared.Obj.at( object ).at( i ).vInsertedAt.at( iCol ).get_Type();

					if( Col_Hole_Ins == COL_HOLE_ID_DIFF ) bColInserted = true;
				}

				msg->fData.emplace_back( static_cast<float>( bColInserted ? Side::INSERTED : Side::NOT_INSERTED ) );
				break;

			default:
				msg->fData.emplace_back( static_cast<float>( Side::NOT_INSERTED ) );
				break;
			}
		}
	}

	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::get_PanTilt_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	size_t
		i = 0,
		number_objects = pData->vObjects_Shared.Obj.at( object ).size();

	float
		fPan = 0.0f,
		fTilt = 0.0f;

	msg->fData.assign( number_objects * 2, 0.0f );

	msg->uParam.assign( 1, 0 );

	for( i=0 ; i<number_objects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		pData->vObjects_Shared.Obj.at( object ).at( i ).get_PanTiltAngles( &fPan, &fTilt );

		msg->fData.emplace_back( fPan );
		msg->fData.emplace_back( fTilt );
	}

	pData->vObjects_Shared.unlock();
}

float CYarpCommVisionBot::get_DistanceToObject( CObject_f * obj )
{
	cv::Point3f pt( 0.0f, 0.0f, 0.0f );

	obj->get_Coordinates_World( &pt );

	return sqrt( pow( pt.x, 2.0f ) + pow( pt.y, 2.0f ) );
}

void CYarpCommVisionBot::get_Distance_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	size_t nObjects = pData->vObjects_Shared.Obj.at( object ).size();

	msg->uParam.assign( 1, 0 );

	for( size_t i=0 ; i<nObjects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;

		msg->uParam.front()++;

		msg->fData.emplace_back( get_DistanceToObject( &pData->vObjects_Shared.Obj.at( object ).at( i ) ) );
	}

	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::get_PanTiltDistance_ObjectType( int object, CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	size_t
		i = 0,
		nObjects = pData->vObjects_Shared.Obj.at( object ).size();

	float
		fPan = 0.0f,
		fTilt = 0.0f;

	msg->uParam.assign( 1, 0 );

	for( i=0 ; i<nObjects ; i++ )
	{
		if( !pData->vObjects_Shared.Obj.at( object ).at( i ).bVisible ) continue;
		
		msg->uParam.front()++;
		
		pData->vObjects_Shared.Obj.at( object ).at( i ).get_PanTiltAngles( &fPan, &fTilt );

		msg->fData.emplace_back( fPan );
		msg->fData.emplace_back( fTilt );
		msg->fData.emplace_back( get_DistanceToObject( &pData->vObjects_Shared.Obj.at( object ).at( i ) ) );
	}

	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::InterfacePanTilt( CMessage *msgIn, CMessage *msgOut )
{
	bool bUsingProcessing = false;

	CData * pData = static_cast<CData*>( pDataSource );

	CProcessImage * image_proc_object = static_cast<CProcessImage*>( pData->image_procObject );
	
	if( pYarpComm_PanTilt == nullptr ) 
	{
		msgOut->uParam.assign( 1, msgIn->uParam.front() + ACK );
		msgOut->iErrorCode = -1;
		msgOut->strText = "Communication with PanTilt not present!";
		return;
	}

	if( !pYarpComm_PanTilt->isOpened() )
	{
		msgOut->uParam.assign( 1, msgIn->uParam.front() + ACK );
		msgOut->iErrorCode = -1;
		msgOut->strText = "Connection with PanTilt not established!";
		return;
	}

	if( !pYarpComm_PanTilt->isOpened() )
	{
		msgOut->uParam.assign( 1, msgIn->uParam.front() + ACK );
		msgOut->iErrorCode = -1;
		msgOut->strText = "No parameter in received message, invalid command.";
		return;
	}

	// Copy the message contents
	CMessage msgPanTilt( *msgIn );

	// Reformat necessary parameters for Pan Tilt to use
	msgPanTilt.uCommand = msgIn->uParam.front();
	msgPanTilt.uParam.clear();

	bUsingProcessing = image_proc_object->useImageProcessing();

	// Turn image processing off while pan tilt is moving
	if( bUsingProcessing )
		image_proc_object->doImageProcessing( false );

	if( !pYarpComm_PanTilt->Send( msgPanTilt ) )
	{
		msgOut->uParam.assign( 1, msgIn->uParam.front() + ACK );
		msgOut->iErrorCode = -1;
		msgOut->strText = "VisionBot was unable to send the command to PanTilt!";
		return;
	}

	// Wait for the pan tilt command to be executed
	bool bSuccess = pYarpComm_PanTilt->Send( static_cast<unsigned int>( PANTILT_WAIT_MOTION_END ) );
		
	pData->PanTilt.Update_All_Values();

	// Format the reply message
	Format_PanTilt_Reply( msgIn->uParam.front(), msgOut );

	if( !bSuccess )
	{
		msgOut->uParam.assign( 1, msgIn->uParam.front() + ACK );
		msgOut->iErrorCode = -1;
		msgOut->strText = "VisionBot was unable to send command 'PANTILT_WAIT_MOTION_END' to the PanTilt!";
		return;
	}

	// Turn image processing on
	if( bUsingProcessing )
		image_proc_object->doImageProcessing( true );
}

void CYarpCommVisionBot::Format_PanTilt_Reply( unsigned int command, CMessage * msg )
{
	msg->iErrorCode = 0;
	msg->uParam.assign( 1, command + ACK );

	CData * pData = static_cast<CData*>( pDataSource );

	switch( command )
	{
	default:
	case PANTILT_ISACTIVE:
	case PANTILT_RESET_PAN:
	case PANTILT_RESET_TILT:
	case PANTILT_INITIALIZE:
	case PANTILT_RESET_PANTILT:
	case PANTILT_STOP_PAN:
	case PANTILT_STOP_TILT:
	case PANTILT_STOP_PANTILT:
	case PANTILT_MOVE_ANGLE_REL_PAN:
	case PANTILT_MOVE_ANGLE_REL_TILT:
	case PANTILT_MOVE_ANGLE_REL_PANTILT:
	case PANTILT_MOVE_VELOCITY_REL_PAN:
	case PANTILT_MOVE_VELOCITY_REL_TILT:
	case PANTILT_MOVE_VELOCITY_REL_PANTILT:
	case PANTILT_GET_REF_ACCELERATION_PAN:
	case PANTILT_GET_REF_ACCELERATION_TILT:
	case PANTILT_GET_REF_ACCELERATION_PANTILT:
	case PANTILT_SET_REF_ACCELERATION_PAN:
	case PANTILT_SET_REF_ACCELERATION_TILT:
	case PANTILT_SET_REF_ACCELERATION_PANTILT:
		// No data will be sent, only '0' error code and command ack
		break;

	case PANTILT_HOME:
	case PANTILT_USE_POSITION_CONTROL:
	case PANTILT_GET_ANGLE_PANTILT:
	case PANTILT_MOVE_ANGLE_ABS_PANTILT:
	case PANTILT_AROS_LOOK_AT_POSITION:
		msg->fData.assign( 2, 0.0f );
		if( pDataSource != nullptr )
		{
			msg->fData.at( PanTilt::Axis::ID::PAN ) = pData->PanTilt.fPan_Angle.get();
			msg->fData.at( PanTilt::Axis::ID::TILT ) = pData->PanTilt.fTilt_Angle.get();
		}
		break;

	case PANTILT_USE_VELOCITY_CONTROL:
	case PANTILT_GET_REF_SPEED_PANTILT:
	case PANTILT_SET_REF_SPEED_PANTILT:
	case PANTILT_MOVE_VELOCITY_ABS_PANTILT:
		msg->fData.assign( 2, 0.0f );
		if( pDataSource != nullptr )
		{
			msg->fData.at( PanTilt::Axis::ID::PAN ) = pData->PanTilt.fPan_Velocity.get();
			msg->fData.at( PanTilt::Axis::ID::TILT ) = pData->PanTilt.fTilt_Velocity.get();
		}
		break;

	case PANTILT_GET_ANGLE_PAN:
	case PANTILT_MOVE_ANGLE_ABS_PAN:
		if( pDataSource != nullptr )
			msg->fData.assign( 1, pData->PanTilt.fPan_Angle.get() );
		break;
				
	case PANTILT_GET_ANGLE_TILT:
	case PANTILT_MOVE_ANGLE_ABS_TILT:
		if( pDataSource != nullptr )
			msg->fData.assign( 1, pData->PanTilt.fTilt_Angle.get() );
		break;

	case PANTILT_GET_REF_SPEED_PAN:
	case PANTILT_SET_REF_SPEED_PAN:
	case PANTILT_MOVE_VELOCITY_ABS_PAN:
		if( pDataSource != nullptr )
			msg->fData.assign( 1, pData->PanTilt.fPan_Velocity.get() );
		break;
				
	case PANTILT_GET_REF_SPEED_TILT:
	case PANTILT_SET_REF_SPEED_TILT:
	case PANTILT_MOVE_VELOCITY_ABS_TILT:
		if( pDataSource != nullptr )
			msg->fData.assign( 1, pData->PanTilt.fTilt_Velocity.get() );
		break;
	}
}

bool CYarpCommVisionBot::get_PositionOfPointedObject( CMessage * msg )
{
	if( pDataSource == nullptr ) return false;

	cv::Point3f pt( 0.0f, 0.0f, 0.0f );

	msg->fData.assign( 3, 0.0f );

	bool bResult = false;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	pData->vObjects_Shared.Obj.at( 0 ).front();

	if( pData->vObjects_Shared.Obj.at( OBJECT_HAND ).size() > 0 )
	{
		if( pData->vObjects_Shared.Obj.at( OBJECT_HAND ).front().vNearObjects.size() > 0 )
		{
			pData->vObjects_Shared.Obj.at( OBJECT_HAND ).front().vNearObjects.front().get_Coordinates_World( &pt );
			msg->fData.at( 0 ) = pt.x/10.0f;
			msg->fData.at( 1 ) = pt.y/10.0f;
			msg->fData.at( 2 ) = pt.z/10.0f;

			bResult = true;
		}
	}

	pData->vObjects_Shared.unlock();

	return bResult;
}

void CYarpCommVisionBot::get_Columns_Insertion_Status( CMessage * msg )
{
	if( pDataSource == nullptr ) return;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->Column_InsertedAt_Shared.lock();
	
	size_t nPairs = pData->Column_InsertedAt_Shared.Obj.size();

	for( size_t iPair = 0; iPair < nPairs; iPair++ )
	{
		msg->uParam.emplace_back( pData->Column_InsertedAt_Shared.Obj.at( iPair ).first );
		msg->uParam.emplace_back( pData->Column_InsertedAt_Shared.Obj.at( iPair ).second );
	}

	pData->Column_InsertedAt_Shared.unlock();
}

void CYarpCommVisionBot::IsObjectTypeAtPosition( CMessage * msgIn, CMessage * msgOut )
{
	// Input:
	// uParam.at( 0 ) -> Object Type
	// fData[0-2] -> x,y,z position (units in mm)
	
	const int
		NO_OBJECT_IN_POSITION_GIVE_CLOSER = 0,
		OBJECT_IN_POSITION = 1,
		NO_OBJECTS_OF_THIS_TYPE = 2,
		OBJECT_NEAR = 50;

	if( ( msgIn->fData.size() < 3 ) || ( msgIn->uParam.size() < 1 ) ) return;
	
	//--------------------------------------------------------------------------
	// Extract information contained in the message
	cv::Point3f 
		pt( msgIn->fData.at( 0 ), msgIn->fData.at( 1 ), msgIn->fData.at( 2 ) ),
		pt_obj( 0.0f, 0.0f, 0.0f ),
		pt_coord( 0.0f, 0.0f, 0.0f );

	int nObj = msgIn->uParam.front();

	//--------------------------------------------------------------------------
	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	size_t nObjects = pData->vObjects_Shared.Obj.at( nObj ).size();

	msgOut->uParam.assign( 1, 0 );

	if( nObjects < 1 )
	{
		msgOut->uParam.front() = NO_OBJECTS_OF_THIS_TYPE;

		// Nothing else to do here!
		return;
	}

	//--------------------------------------------------------------------------
	// Implementation follows here!

	float 
		fMin = 0.0f,
		*fDistances = new 
			#ifndef _DEBUG 
				(std::nothrow) 
			#endif
				float[ nObjects ];

	if( fDistances == nullptr ) return;

	bool bObjectInPosition = false;
	
	size_t iCloser = 0;

	// Compute distance of given coordinates to each object
	for( size_t i=0 ; i<nObjects ; i++ )
	{
		pData->vObjects_Shared.Obj.at( nObj ).at( i ).get_Coordinates_World( &pt_obj );

		fDistances[i] = sqrt( pow( (pt.x-pt_obj.x), 2 ) + pow( (pt.y-pt_obj.y), 2 ) + pow( (pt.z-pt_obj.z), 2 ) );
				
		if( fDistances[i] < OBJECT_NEAR ) bObjectInPosition = true;

		if( i==0 )
		{
			fMin = fDistances[0];
			iCloser = 0;
		}
		else
		{
			if( fDistances[i]<fMin )
			{
				fMin = fDistances[i];
				iCloser = i;
			}
		}
	}
	//--------------------------------------------------------------------------
	// Format resulting message
	// Output:
	// uParam.at( 0 ):
	//		0 - No. Give info about close one. fData[0-4]: x,y,z,phi,inserted
	//		1 - Yes
	//		2 - No Objects of that type

	if( bObjectInPosition )
	{
		msgOut->uParam.front() = OBJECT_IN_POSITION;
	}
	else
	{
		msgOut->uParam.front() = NO_OBJECT_IN_POSITION_GIVE_CLOSER;

		pData->vObjects_Shared.Obj.at( nObj ).at( iCloser ).get_Coordinates_World( &pt_obj );
			
		msgOut->fData.assign( 5, 0.0f );
		// X, Y, Z
		msgOut->fData.at( 0 ) = pt_obj.x;
		msgOut->fData.at( 1 ) = pt_obj.y;
		msgOut->fData.at( 2 ) = pt_obj.z;
		
		msgOut->fData.at( 4 ) = static_cast<float>( get_Workspace_Position( pt_obj.x ) );

		// Inserted
		pData->vObjects_Shared.Obj.at( nObj ).at( iCloser ).get_Orientation( &pt_obj );

		// Phi
		msgOut->fData.at( 3 ) = pt_obj.z;
	}

	pData->vObjects_Shared.unlock();
			
	//--------------------------------------------------------------------------
	// Clean up
	delete[] fDistances;
	fDistances = nullptr;
}

void CYarpCommVisionBot::get_HandNear_ObjectsPointed_GripTypes( CMessage * msg )
{
	CData * pData = static_cast<CData*>( pDataSource );

	pData->vGestures.lock();

	size_t 
		nGestures = pData->vGestures.Obj.size(),
		num_CSG_Objects = sizeof( CSG_Objects ) / sizeof( unsigned int );

	msg->uParam.assign( 2 * num_CSG_Objects, 0 );

	for( size_t i = 0; i<num_CSG_Objects; i++ )
	{
		msg->uParam.at( i ) = pData->get_HandNearObject( CSG_Objects[i] );
	}

	// Objects pointed
	pData->vObjPointed.lock();

	size_t nPointing = pData->vObjPointed.Obj.size();

	if( pData->vObjPointed.Obj.size() > 0 )
	{ 
		for( size_t index = num_CSG_Objects; index < 2 * num_CSG_Objects; index++ )
		{
			msg->uParam.at( index ) = Side::NONE;

			for( size_t i = 0; i < nPointing; i++ )
			{
				if( pData->vObjPointed.Obj.at( i ).uType == CSG_Objects[index - num_CSG_Objects] )
				{
					msg->uParam.at( index ) = get_Workspace_Position( pData->vObjPointed.Obj.at( i ).Coord_World.x );
				}
			}
		}
	}
	
	pData->vObjPointed.unlock();

	// Grip types
	unsigned int uGest = 0;
	bool bValidGesture = false;

	for( size_t i = 0; i < nGestures; i++ )
	{
		uGest = pData->vGestures.Obj.at( i );
		if( uGest == Grip::ABOVE || uGest == Grip::SIDE )
		{
			msg->uParam.emplace_back( uGest );
			bValidGesture = true;
		}
	}
	
	if( !bValidGesture )
		msg->uParam.emplace_back( Grip::NONE );
	
	pData->vGestures.unlock();
}

void CYarpCommVisionBot::get_Robot_Human_Workspace_CSG_Objects( CMessage * msg )
{
	size_t 
		nObjs = 0,
		num_CSG_Objects = sizeof( CSG_Objects ) / sizeof( unsigned int );

	cv::Point3f pt_coord( 0.0f, 0.0f, 0.0f );

	msg->uParam.assign( num_CSG_Objects, Side::NONE );

	unsigned int
		uHuman = Side::NONE,
		uRobot = Side::NONE;

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	
	for( size_t iObj = 0; iObj < num_CSG_Objects; iObj++ )
	{
		nObjs = pData->vObjects_Shared.Obj.at( CSG_Objects[iObj] ).size();

		uHuman = Side::NONE;
		uRobot = Side::NONE;

		for( size_t i = 0; i < nObjs; i++ )
		{
			if( !pData->vObjects_Shared.Obj.at( CSG_Objects[iObj] ).at( i ).has3D() ||	// Valid 3D coordinates ?
				pData->vObjects_Shared.Obj.at( CSG_Objects[iObj] ).at( i ).bInserted ||	// The object is inserted ?
				!pData->vObjects_Shared.Obj.at( CSG_Objects[iObj] ).at( i ).bVisible )	// Marked as not visible ?
				continue;	// continue to the next object

			pData->vObjects_Shared.Obj.at( CSG_Objects[iObj] ).at( i ).get_Coordinates_World( &pt_coord );

			// Check the workspace
			if( get_Workspace_Position( pt_coord.x ) == Side::ROBOT )
				// It's in the robot workspace
				uRobot = Side::ROBOT;
			else
				// It's in the human workspace
				uHuman = Side::HUMAN;
		}
	
		msg->uParam.at( iObj ) = uRobot + uHuman;
	}
	
	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::set_FrameSaving_StartStop( CMessage * msg )
{
	CData * pData = static_cast<CData*>( pDataSource );

	CProcessImage * image_proc_object = static_cast<CProcessImage*>( pData->image_procObject );
	CVisionBotDlg * visionbot_dlg = static_cast<CVisionBotDlg*>( visionbot_dlg_ptr );

	if( msg->uParam.size() > 0 )
	{
		( msg->uParam.front() == 1 ) ? 
			image_proc_object->FrameSaving_On() : 
			image_proc_object->FrameSaving_Off();

		visionbot_dlg->Update_MenuState_FrameSaving();
	}

	image_proc_object = nullptr;
	visionbot_dlg = nullptr;
}

void CYarpCommVisionBot::set_ImageProcessing_OnOff( CMessage * msg )
{
	CData * pData = static_cast<CData*>( pDataSource );

	CProcessImage * image_proc_object = static_cast<CProcessImage*>( static_cast<CData*>( pDataSource )->image_procObject );
	CVisionBotDlg * visionbot_dlg = static_cast<CVisionBotDlg*>( visionbot_dlg_ptr );

	if( msg->uParam.size() > 0 )
	{
		if( image_proc_object != nullptr )
			image_proc_object->doImageProcessing( msg->uParam.front() == 1 );

		if( visionbot_dlg != nullptr )
			visionbot_dlg->Update_MenuState_ImageProcessing();
	}

	image_proc_object = nullptr;
	visionbot_dlg = nullptr;
}

void CYarpCommVisionBot::ARoS_Look_at_Pointed_Object( void )
{
	CMessage 
		msg_aux,
		msg;

	if( get_PositionOfPointedObject( &msg_aux ) )
	{
		msg_aux.uParam.assign( 1, PANTILT_AROS_LOOK_AT_POSITION );
		InterfacePanTilt( &msg_aux, &msg );
	}
}

void CYarpCommVisionBot::get_Histogram_Hue( CMessage * msg )
{
	std::vector<int> v_hist;

	static_cast<CData*>( pDataSource )->get_HistogramData( v_hist );

	size_t vSize = v_hist.size();

	msg->fData.assign( vSize, 0.0f );

	for( size_t i = 0; i<vSize; i++ )
	{
		msg->fData.at( i ) = static_cast<float>( v_hist.at( i ) );
	}
	
	v_hist.clear();
}

void CYarpCommVisionBot::get_NumberObjects( CMessage * msg )
{
	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();
	msg->fData.assign( NUMBER_OF_OBJECT_TYPES, 0.0f );

	size_t nObj = 0;

	for( int i = 0; i<NUMBER_OF_OBJECT_TYPES; i++ )
	{
		nObj = pData->vObjects_Shared.Obj.at( i ).size();

		for( size_t iObj = 0; iObj < nObj; iObj++ )
		{
			// Count only visible objects
			if( pData->vObjects_Shared.Obj.at( i ).at( iObj ).bVisible )
				msg->fData.at( i )++;
		}
	}
	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::get_Is_Hand_Near( CMessage * msg )
{
	size_t num_CSG_Objects = sizeof( CSG_Objects ) / sizeof( unsigned int );

	msg->uParam.assign( num_CSG_Objects, 0 );

	CData * pData = static_cast<CData*>( pDataSource );

	pData->vObjects_Shared.lock();

	if( pData->vObjects_Shared.Obj.at( OBJECT_HAND ).size() > 0 )
	{
		for( size_t i = 0; i<num_CSG_Objects; i++ )
		{
			msg->uParam.at( i ) = pData->get_HandNearObject( CSG_Objects[i] );
		}
	}

	pData->vObjects_Shared.unlock();
}

void CYarpCommVisionBot::get_InsertedObjects( CMessage * msg )
{
	size_t num_CSG_Objects = sizeof( CSG_Objects ) / sizeof( unsigned int );

	CData * pData = static_cast<CData*>( pDataSource );

	msg->uParam.assign( num_CSG_Objects, 0 );
	for( size_t i = 0; i<num_CSG_Objects; i++ )
	{
		msg->uParam.at( i ) = pData->get_ObjectInsertedWhere( CSG_Objects[i] );
	}
}

void CYarpCommVisionBot::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	// Here I reply to commands I receive
	// acting as a server and answer to requests

	// Test if it is a valid command
	if( !msgIn.isValid() ) return;

	if( msgIn.uCommand > PANTILT_COMMAND  && msgIn.uCommand < VISION_BOT_COMMAND )
	{
		// Process PanTilt commands
		Process_Commands_PanTilt( &msgIn, &msgOut );
	}
	else
	{
		// Process VisionBot commands
		Process_Commands_VisionBot( &msgIn, &msgOut );
	}
}

void CYarpCommVisionBot::Process_Commands_MobileRobot( CMessage *msgIn, CMessage *msgOut )
{
	CData * pData = static_cast<CData*>( pDataSource );

	float tempValue = 0;

	switch( msgIn->uCommand )
	{
	//--------------------------------------------------------------------------
	case TARGET_GET_ANGLE_AND_DISTANCE:
		msgOut->fData.assign( 2, 0 );
		msgOut->uParam.assign(1, 0);
		if( msgIn->uParam.size() > 0 )
		{
			bool tmpExist;
			msgOut->iErrorCode = pData->get_MobileRobotTarget(msgIn->uParam.at(0), &msgOut->fData.at(0), &msgOut->fData.at(1), &tmpExist);
			msgOut->uParam[0] = tmpExist;
			if( msgOut->iErrorCode == -1 )
			{
				msgOut->strText = "The specified target was not detected.";
			}
			else if( msgOut->iErrorCode == -2 )
			{
				msgOut->strText = "The specified target is not valid.";
				msgOut->fData.at( 0 ) = 0.0f;
				msgOut->fData.at( 1 ) = 0.0f;
			}
		}
		else
		{
			msgOut->iErrorCode = -3;
			msgOut->strText = "Target type must be specified in uParam[0].";
		}
		break;

	//--------------------------------------------------------------------------
	case TARGET_GET_ANGLE:
		msgOut->fData.assign( 1, 0 );
		msgOut->uParam.assign(1, 0);
		if( msgIn->uParam.size() > 0 )
		{
			bool tmpExist;
			msgOut->iErrorCode = pData->get_MobileRobotTarget(msgIn->uParam.at(0), &msgOut->fData.at(0), &tempValue, &tmpExist);
			msgOut->uParam[0] = tmpExist;
			if( msgOut->iErrorCode == -1 )
				msgOut->strText = "The specified target was not detected.";
			else if( msgOut->iErrorCode == -2 )
				msgOut->strText = "The specified target is not valid.";
		}
		else
		{
			msgOut->iErrorCode = -3;
			msgOut->strText = "Target type should be specified in uParam[0].";
		}
		break;

	//--------------------------------------------------------------------------
	case TARGET_GET_DISTANCE:
		msgOut->fData.assign( 1, 0 );
		msgOut->uParam.assign(1, 0);
		if( msgIn->uParam.size() > 0 )
		{
			bool tmpExist;
			msgOut->iErrorCode = pData->get_MobileRobotTarget(msgIn->uParam.at(0), &tempValue, &msgOut->fData.at(0), &tmpExist);
			msgOut->uParam[0] = tmpExist;
			if( msgOut->iErrorCode == -1 )
				msgOut->strText = "The specified target was not detected.";
			else if( msgOut->iErrorCode == -2 )
				msgOut->strText = "The specified target is not valid.";
		}
		else
		{
			msgOut->iErrorCode = -3;
			msgOut->strText = "Target type should be specified in uParam[0].";
		}
		break;

	//--------------------------------------------------------------------------
	default:
		break;
	}
}

void CYarpCommVisionBot::Process_Commands_PanTilt( CMessage *msgIn, CMessage *msgOut )
{
	float
		fPan = 0.0f,
		fTilt = 0.0f;

	CData * pData = static_cast<CData*>( pDataSource );

	switch( msgIn->uCommand )
	{
	//--------------------------------------------------------------------------
	case PANTILT_USE_POSITION_CONTROL + ACK:
		if( msgIn->iErrorCode == 0 )
		{
			pData->PanTilt.nControlMode.set( PanTilt::ControlMode::POSITION );

			if( ( msgIn->uParam.size() > 1 ) && ( msgIn->fData.size() > 1 ) )
			{
				if( msgIn->uParam.at( PanTilt::Axis::ID::PAN ) == 1 )
					pData->PanTilt.fPan_Angle.set( msgIn->fData.at( PanTilt::Axis::ID::PAN ) );

				if( msgIn->uParam.at( PanTilt::Axis::ID::TILT ) == 1 )
					pData->PanTilt.fTilt_Angle.set( msgIn->fData.at( PanTilt::Axis::ID::TILT ) );
			}
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_USE_VELOCITY_CONTROL + ACK:
		if( msgIn->iErrorCode == 0 )
		{
			pData->PanTilt.nControlMode.set( PanTilt::ControlMode::VELOCITY );

			if( ( msgIn->uParam.size() > 1 ) && ( msgIn->fData.size() > 1 ) )
			{
				if( msgIn->uParam.at( PanTilt::Axis::ID::PAN ) == 1 )
					pData->PanTilt.fPan_Velocity.set( msgIn->fData.at( PanTilt::Axis::ID::PAN ) );

				if( msgIn->uParam.at( PanTilt::Axis::ID::TILT ) == 1 )
					pData->PanTilt.fTilt_Velocity.set( msgIn->fData.at( PanTilt::Axis::ID::TILT ) );
			}
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_ANGLE_PAN + ACK:
	case PANTILT_MOVE_ANGLE_ABS_PAN + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
			pData->PanTilt.fPan_Angle_temp.set( msgIn->fData.front() );
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_ANGLE_TILT + ACK:
	case PANTILT_MOVE_ANGLE_ABS_TILT + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
			pData->PanTilt.fTilt_Angle_temp.set( msgIn->fData.front() );
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_ANGLE_PANTILT + ACK:
	case PANTILT_MOVE_ANGLE_ABS_PANTILT + ACK:
	case PANTILT_AROS_LOOK_AT_POSITION + ACK:
		if( msgIn->fData.size() < 2 ) break;

		if( msgIn->iErrorCode == 0 )
		{
			pData->PanTilt.set_PanTiltAngles_temp( msgIn->fData.at( 0 ), msgIn->fData.at( 1 ) );
		}
		else if( msgIn->uParam.size() > 1 )
		{
			if( msgIn->uParam.at( 0 ) == 1 )
				pData->PanTilt.fPan_Angle.set( msgIn->fData.at( 0 ) );

			if( msgIn->uParam.at( 1 ) == 1 )
				pData->PanTilt.fTilt_Angle.set( msgIn->fData.at( 1 ) );
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_MOVE_ANGLE_REL_PAN + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
		{
			fPan = pData->PanTilt.fPan_Angle.get() + msgIn->fData.front();
			pData->PanTilt.fPan_Angle_temp.set( fPan );
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_MOVE_ANGLE_REL_TILT + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
		{
			fTilt = pData->PanTilt.fTilt_Angle.get() + msgIn->fData.front();
			pData->PanTilt.fTilt_Angle_temp.set( fTilt );
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_MOVE_ANGLE_REL_PANTILT + ACK:
		if( msgIn->fData.size() < 2 ) break;

		if( msgIn->iErrorCode == 0 )
		{
			pData->PanTilt.get_PanTiltAngles( &fPan, &fTilt );
			fPan += msgIn->fData.at( 0 );
			fTilt += msgIn->fData.at( 1 );
			pData->PanTilt.set_PanTiltAngles_temp( fPan, fTilt );
		}

		else if( msgIn->uParam.size() > 1 )
		{
			if( msgIn->uParam.at( 0 ) == 1 )
			{
				fPan = pData->PanTilt.fPan_Angle.get() + msgIn->fData.at( 0 );
				pData->PanTilt.fPan_Angle_temp.set( fPan );
			}

			if( msgIn->uParam.at( 1 ) == 1 )
			{
				fTilt = pData->PanTilt.fTilt_Angle.get() + msgIn->fData.at( 1 );
				pData->PanTilt.fTilt_Angle_temp.set( fTilt );
			}
		}
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_REF_SPEED_PAN + ACK:
	case PANTILT_MOVE_VELOCITY_ABS_PAN + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
			pData->PanTilt.fPan_Velocity_temp.set( msgIn->fData.front() );
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_REF_SPEED_TILT + ACK:
	case PANTILT_MOVE_VELOCITY_ABS_TILT + ACK:
		if( ( msgIn->fData.size() > 0 ) && ( msgIn->iErrorCode == 0 ) )
			pData->PanTilt.fTilt_Velocity_temp.set( msgIn->fData.front() );
		break;

	//--------------------------------------------------------------------------
	case PANTILT_GET_REF_SPEED_PANTILT + ACK:
	case PANTILT_MOVE_VELOCITY_ABS_PANTILT + ACK:
		if( msgIn->fData.size() < 2 ) break;

		if( msgIn->iErrorCode == 0 )
		{
			pData->PanTilt.set_PanTiltVelocities( msgIn->fData.at( 0 ), msgIn->fData.at( 1 ) );
		}
		else if( msgIn->uParam.size() > 1 )
		{
			if( msgIn->uParam.at( 0 ) == 1 )
				pData->PanTilt.fPan_Velocity_temp.set( msgIn->fData.at( 0 ) );

			if( msgIn->uParam.at( 1 ) == 1 )
				pData->PanTilt.fTilt_Velocity_temp.set( msgIn->fData.at( 1 ) );
		}
		break;

	//--------------------------------------------------------------------------
	default:
		// Do nothing...
		break;
	}
}

void CYarpCommVisionBot::Process_Commands_VisionBot( CMessage *msgIn, CMessage *msgOut )
{
	CData * pData = static_cast<CData*>( pDataSource );

	//--------------------------------------------------------------------------
	// Process the command
	switch( msgIn->uCommand )
	{
	//--------------------------------------------------------------------------
	case TARGET_GET_ANGLE_AND_DISTANCE:
	case TARGET_GET_ANGLE:
	case TARGET_GET_DISTANCE:
		Process_Commands_MobileRobot( msgIn, msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_NUMBER_OBJECTS:
		get_NumberObjects( msgOut );
		break;

	//--------------------------------------------------------------------------
	// Command to get position information about object types
	case VISION_BOT_GET_POSITION_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_Position_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	// Command to get orientation information about object types
	case VISION_BOT_GET_ORIENTATION_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_Orientation_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	// Command to get position and orientation information about object types
	case VISION_BOT_GET_POSITION_ORIENTATION_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_PositionOrientation_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	// Command to get position and orientation information about inserted object types
	case VISION_BOT_GET_POSITION_ORIENTATION_INSERTED_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_PositionOrientationInserted_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_IS_HAND_NEAR:
		get_Is_Hand_Near( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_INSERTED_OBJECTS:
		get_InsertedObjects( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_HISTOGRAM_HUE:
		get_Histogram_Hue( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_IS_OBJECT_TYPE_AT_POSITION:
		IsObjectTypeAtPosition( msgIn, msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_POSITION_OF_POINTED_OBJECT:
		get_PositionOfPointedObject( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_AROS_LOOK_AT_POINTED_OBJECT:
		ARoS_Look_at_Pointed_Object();
		break;

	//--------------------------------------------------------------------------
	//	Interface with Pan Tilt
	case VISION_BOT_AROS_PAN_TILT_INTERFACE:
		InterfacePanTilt( msgIn, msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_LEARNING_TASK_STATUS:
		mutex_vector_copy( pData->vTaskStatus, msgOut->uParam );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_PANTILT_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_PanTilt_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_DISTANCE_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_Distance_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_PANTILT_DISTANCE_OBJECT_TYPE:
		if( msgIn->uParam.size() > 0 )
			get_PanTiltDistance_ObjectType( msgIn->uParam.front(), msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_PANTILT_DISTANCE_OBJECT_TYPE_HEAD_ANGLES:
		if( msgIn->uParam.size() > 0 )
		{
			get_PanTiltDistance_ObjectType( msgIn->uParam.front(), msgOut );
			msgOut->fData.insert( msgOut->fData.begin(), 2, 0.0f );
			msgOut->fData.at( 0 ) = pData->PanTilt.fPan_Angle.get();
			msgOut->fData.at( 1 ) = pData->PanTilt.fTilt_Angle.get();
		}
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_FRAME_SAVING_START_STOP:
		set_FrameSaving_StartStop( msgIn );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_IMAGE_PROCESSING_ENABLE_DISABLE:
		set_ImageProcessing_OnOff( msgIn );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_USER_BODY_MOVEMENT:
		msgOut->fData.assign( 1, pData->HumanInfo.BodyMovement.get() );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_HAND_MOVEMENT:
		msgOut->fData.assign( 1, pData->HumanInfo.HandMovement.get() );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_COLUMNS_INSERTION:
		get_Columns_Insertion_Status( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_GESTURE:
		mutex_vector_copy( pData->vGestures, msgOut->uParam );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_HAND_NEAR_GRIP_TYPE_OBJ_POINTED:
		get_HandNear_ObjectsPointed_GripTypes( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_ROBOT_HUMAN_WORKSPACE_CSG_OBJECTS:
		get_Robot_Human_Workspace_CSG_Objects( msgOut );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_LEARNING_DRINKING_TASK_STATUS:
		mutex_vector_copy( pData->vDrinkTaskStatus, msgOut->uParam );
		break;

	//--------------------------------------------------------------------------
	case VISION_BOT_GET_LEARNING_COMPUTERASSEMBLY_TASK_STATUS:
		mutex_vector_copy(pData->vComputerAssemblyTaskStatus, msgOut->uParam);
		break;
	
	//--------------------------------------------------------------------------
	default:
		// Do nothing...
		break;
	}

}

#endif // USE_YarpCommunication
