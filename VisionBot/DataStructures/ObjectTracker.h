#ifndef _OBJECT_TRACKER_H_
#define _OBJECT_TRACKER_H_

#pragma once

#include <fstream>

#include "BlobTracker.h"
#include "ObjectMH.h"

#ifndef ENABLE_FILE_LOGGING
#define ENABLE_FILE_LOGGING TRUE
#endif

class CObjectTracker
	: protected CBlobTracker
{
public:
//---------------------------------------------------------------------------
// Construction, setup
	CObjectTracker( unsigned int object_type );
	CObjectTracker( const CObjectTracker &obj );
	~CObjectTracker( void );

	void Setup( unsigned int width, unsigned int height );
	void Clear( void );

//---------------------------------------------------------------------------
public:
	void TrackObjects( cv::Mat * mat_ptr );

	void Push_New_Coordinates( std::vector< CObject_f > &vecObject );

	std::vector< CObjectMH > vObject;

//---------------------------------------------------------------------------
// Accessors
public:
	bool isReady( void );
	void get_Tracked_Objects_Vector( std::vector< CObject_f > &vec );
	float get_MaxMovement( void );
	unsigned int get_ObjectType( void );

//---------------------------------------------------------------------------
protected:
	bool bReady;
	unsigned int uObjectType;
	float fMaxMovement;

	void set_Type( unsigned int type );

	void RetrieveTrackedObjects( void );

//---------------------------------------------------------------------------
#if ENABLE_FILE_LOGGING
protected:
	std::string 
		strLog,
		strLogHeader;

	std::ofstream stream_file_log;
#endif;
};

#endif // _OBJECT_TRACKER_H_
