#ifndef _DATA_H_
#define _DATA_H_

#pragma once

#include <string>
#include <vector>

#include "AttachMutex.hpp"
#include "ConfigFile.h"
#include "DataDefinitions.h"
#include "HumanInfo.h"
#include "Object_T.h"
#include "ObjectBasic.h"
#include "PanTiltData.h"

class CData  
	: public CConfigFile
{
public:
	CData( void );
	~CData( void );

//------------------------------------------------------------------------------
// Data variables
protected:
	CAttachMutex_n<unsigned int> uTask;

private:
	std::string sRobotName;

	CAttachMutex_o< std::vector<int> > vHistogram;

public:
	CAttachMutex_o< std::vector< std::vector< CObject_f > > >
		vObjects,
		vObjects_Shared;

	/// <summary>Object that holds the last known complete (with holes position) information about the base.</summary>
	CObject_f objBase;

	/// <summary>Object to handle data from PanTilt.</summary>
	CPanTiltData PanTilt;

	/// <summary>What objects are inserted on the Human side and on Robots side.</summary>
	CAttachMutex_o< std::vector< std::vector<int> > > 
		vInsertedObjects,
		vInsertedObjects_Shared;
	
	/// <summary>Vector of pairs composed of:
	/// first: Column identifier;
	/// second: Hole where is inserted.</summary>
	CAttachMutex_o< std::vector< std::pair < unsigned int, unsigned int > > >
		Column_InsertedAt,
		Column_InsertedAt_Shared;

	void * image_procObject;

	/// <summary>Copies the information stored in the objects processed by vision
	/// to another place to be used by communications.</summary>
	void Refresh_Information( void );

	void Update_LastKnownGoodBase( void );

protected:
	bool Parse_Parameters_Load( void );

//------------------------------------------------------------------------------
// Accessors
public:
	std::string get_RobotName( void );

	unsigned int get_Task( void );
	void set_Task( unsigned int task );

	unsigned int get_ObjectInsertedWhere( unsigned int nObjectType );
	
	unsigned int get_HandNearObject( unsigned int nObjectType );

	void set_HistogramData( std::vector<int> v_data );
	void get_HistogramData( std::vector<int> &v_data );

//------------------------------------------------------------------------------
// Movement related information
public:
	CHumanInfo HumanInfo;

protected:
	CAttachMutex_t< std::vector< int > > vTaskObjects;
	void SetupTaskObjects( void );

public:
	int get_TaskNumObjects( void );
	int get_TaskObject( int index );
	std::vector< int > get_TaskObjects( void );

//------------------------------------------------------------------------------
public:
	// Hold information about status on task Toy Robot Learning
	CAttachMutex_t< std::vector< unsigned int > > vTaskStatus;

//------------------------------------------------------------------------------
protected:
	bool 
		bUseHandTracker,
		bUseBodyMovement;

public:
	bool get_UseHandTracker( void );
	bool get_UseBodyMovement( void );

//------------------------------------------------------------------------------
public:
	CAttachMutex_o< std::vector< unsigned int > > vGestures;
	CAttachMutex_o < std::vector< CObjectBasic > > vObjPointed;

//------------------------------------------------------------------------------
public:
	CAttachMutex_o< std::vector< unsigned int > > vDrinkTaskStatus;

	CAttachMutex_o< std::vector< unsigned int > > vComputerAssemblyTaskStatus;

//------------------------------------------------------------------------------
protected:
	CAttachMutex_o< structMobileRobotTarget [6] > mobileRobotTarget;

public:
	// type = 0, 1, 2, 3, 4 or 5
	//	1 -> OBJECT_RED
	//	2 -> OBJECT_YELLOW
	//	3 -> OBJECT_GREEN
	//	4 -> OBJECT_YELLOW
	//	5 -> OBJECT_BLUE
	//	6 -> OBJECT_MAGENTA
	void set_MobileRobotTarget( unsigned int type, float angle, float distance, bool exist );
	int get_MobileRobotTarget(unsigned int type, float *angle, float *distance, bool *exist);

};
#endif // _DATA_H_
