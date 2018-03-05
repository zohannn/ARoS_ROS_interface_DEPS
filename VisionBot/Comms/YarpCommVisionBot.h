#if !defined( _YARP_COMM_VISIONBOT_H_ ) && defined( USE_YarpCommunication )
#define _YARP_COMM_VISIONBOT_H_

#pragma once

#include "YarpCommunication.h"
#include "Object_T.h"

class CYarpCommVisionBot 
	: public CYarpCommunication
{
public:
	CYarpCommVisionBot( const std::string name, const std::string peerName = "", bool isServer = true );
	~CYarpCommVisionBot( void );

protected:
	void Process( CMessage &msgIn, CMessage &msgOut, void *private_data = nullptr );

//------------------------------------------------------------------------------
protected:
	void Process_Commands_PanTilt( CMessage *msgIn, CMessage *msgOut );
	void Process_Commands_MobileRobot( CMessage *msgIn, CMessage *msgOut );
	void Process_Commands_VisionBot( CMessage *msgIn, CMessage *msgOut );

//------------------------------------------------------------------------------
// Accessors
//------------------------------------------------------------------------------
public:
	// Where's the juice?	
	void set_DataSource( void *pData );

	// Set up dependency of other communication object
	void set_YarpComm_PanTilt_Pointer( CYarpCommunication * pYarpComm );

	// Set up a pointer to the main dialog
	void set_VisionBotDlg_Ptr( void * visionbot_ptr );

//------------------------------------------------------------------------------
// Functions to process requests
public:
	void get_Position_ObjectType( int object, CMessage * msg );
	void get_Orientation_ObjectType( int object, CMessage * msg );
	void get_PositionOrientation_ObjectType( int object, CMessage * msg );
	void get_PositionOrientationInserted_ObjectType( unsigned int object, CMessage * msg );
	bool get_PositionOfPointedObject( CMessage * msg );
	void get_PanTilt_ObjectType( int object, CMessage * msg );
	void get_Distance_ObjectType( int object, CMessage * msg );
	void get_PanTiltDistance_ObjectType( int object, CMessage * msg );
	void get_Columns_Insertion_Status( CMessage * msg );
	float get_DistanceToObject( CObject_f * obj );
	void IsObjectTypeAtPosition( CMessage * msgIn, CMessage * msgOut );
	void get_HandNear_ObjectsPointed_GripTypes( CMessage * msg );
	void get_Robot_Human_Workspace_CSG_Objects( CMessage * msg );
	void get_Histogram_Hue( CMessage * msg );
	void get_NumberObjects( CMessage * msg );
	void get_Is_Hand_Near( CMessage * msg );
	void get_InsertedObjects( CMessage * msg );

	void set_FrameSaving_StartStop( CMessage * msg );
	void set_ImageProcessing_OnOff( CMessage * msg );

	void InterfacePanTilt( CMessage *msgIn, CMessage *msgOut );
	void Format_PanTilt_Reply( unsigned int command, CMessage * msg );

	void ARoS_Look_at_Pointed_Object( void );

//------------------------------------------------------------------------------
private:
	void * pDataSource;

	CYarpCommunication * pYarpComm_PanTilt;

	void * visionbot_dlg_ptr;
};

#endif // !defined( _YARP_COMM_VISIONBOT_H_ ) && defined( USE_YarpCommunication )
