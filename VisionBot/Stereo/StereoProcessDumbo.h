#if !defined( _STEREO_PROCESS_DUMBO_H_ ) && defined( USE_StereoVidere )
#define _STEREO_PROCESS_DUMBO_H_

#pragma once

#include "StereoProcess.h"
#include "StereoVidereDumbo.h"

class CStereoProcessDumbo 
	: public CStereoProcess
{
public:
	CStereoProcessDumbo( void );
	~CStereoProcessDumbo( void );

//---------------------------------------------------------------------------
public:
	void Process_Stereo_Data( void );
};

#endif // !defined( _STEREO_PROCESS_DUMBO_H_ ) && defined( USE_StereoVidere )
