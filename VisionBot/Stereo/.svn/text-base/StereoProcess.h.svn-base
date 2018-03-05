#if !defined( _STEREO_PROCESS_H_ ) && defined( USE_StereoVidere )
#define _STEREO_PROCESS_H_

#pragma once

#include "Data.h"
#include "Object_T.h"
#include "StereoVidere.h"

class CStereoProcess
{
public:
	CStereoProcess( void );
	virtual ~CStereoProcess( void );

//---------------------------------------------------------------------------
protected:
	// Data source
	CData * pDataSource;

public:
	CStereoVidere * objStereo;

public:
	void set_DataSource( CData * ptrData );

protected:
//---------------------------------------------------------------------------
// Get coordinates from picture
public:
	bool CalculateCoordinates( CObject_f * pObj );

	bool CalculateCoordinates_Mask( CObject_f * pObj );

	void Perform_Stereo_Process( void );

	virtual void Process_Stereo_Data( void ) {};
};

#endif // !defined( _STEREO_PROCESS_H_ ) && defined( USE_StereoVidere )
