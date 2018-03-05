#include "AmtecPStepServer.h"

#include <iostream>
#include <string>

// Required for STARTUPINFO and PROCESS_INFORMATION objects
#if defined WIN32 && !defined _WINDOWS_
#include <Windows.h>
#endif

#ifndef AMTEC_PSTEPSERVER_EXE
#define	AMTEC_PSTEPSERVER_PATH	"/Hardware/YARP_Wrappers/Amtec_PStepServer/bin/"
#define	AMTEC_PSTEPSERVER_EXE	"PStepServer.exe -s"
#endif

CAmtecPStepServer::CAmtecPStepServer( void )
: PStepServer_proc( nullptr )
, PStepServer_startup_info( nullptr )
{
}

CAmtecPStepServer::~CAmtecPStepServer( void )
{
	if( PStepServer_proc != nullptr )
		Terminate();
}

bool CAmtecPStepServer::Initialize( void )
{

	PStepServer_startup_info = new STARTUPINFO;

	LPSTARTUPINFO startup_info = (LPSTARTUPINFO) PStepServer_startup_info;

	GetStartupInfo( startup_info );

	std::string strTitle = "Amtec PStepServer";

	const int string_size = strTitle.size();

	startup_info->lpTitle = new char[string_size + 1];

#pragma warning ( disable : 4996 )
	strTitle.copy( startup_info->lpTitle, string_size );
#pragma warning ( default : 4996 )
	startup_info->lpTitle[string_size] = '\0';

	std::string
		strAmtecStepServer = "",
		strAmtecStepServer_Path = getenv( "MAINPROJECT" );

	if( strAmtecStepServer_Path.compare( "" ) != 0 )
	{
		strAmtecStepServer_Path += AMTEC_PSTEPSERVER_PATH;
	}
	else
	{
		//When no File system based on MARL repository layout
		strAmtecStepServer_Path = "PStepServer/";
	}

	strAmtecStepServer = strAmtecStepServer_Path + AMTEC_PSTEPSERVER_EXE;

	PStepServer_proc = new PROCESS_INFORMATION;

	LPPROCESS_INFORMATION ptrProc_Info = (PROCESS_INFORMATION *) PStepServer_proc;

	int nAmtecProc = CreateProcess(
		NULL,					// (LPCTSTR) pointer to name of executable module 
		(char*) strAmtecStepServer.c_str(),	// (LPTSTR) pointer to command line string
		NULL,					// (LPSECURITY_ATTRIBUTES) process security attributes
		NULL,					// (LPSECURITY_ATTRIBUTES) thread security attributes
		TRUE,					// (BOOL) handle inheritance flag
		CREATE_NEW_CONSOLE,		// (DWORD) creation flags
		NULL,					// (LPVOID) pointer to new environment block
		strAmtecStepServer_Path.c_str(),				// (LPCTSTR) pointer to current directory name
		startup_info,			// (LPSTARTUPINFO) pointer to STARTUPINFO
		ptrProc_Info			// (LPPROCESS_INFORMATION) pointer to PROCESS_INFORMATION
		);

	if( nAmtecProc != 0 )
	{
		unsigned long exit_code = 0;

		if( GetExitCodeProcess( ptrProc_Info->hProcess, &exit_code ) != 0 )
		{
			if( exit_code != STILL_ACTIVE )
			{
				std::cerr << std::endl <<
					"Error creating Amtec Power Step Server!" << std::endl;

				delete ( (PROCESS_INFORMATION *) PStepServer_proc );
				PStepServer_proc = nullptr;
				ptrProc_Info = nullptr;

				delete startup_info->lpTitle;
				delete ( (LPSTARTUPINFO) PStepServer_startup_info );
				PStepServer_startup_info = nullptr;
				startup_info = nullptr;

				return false;
			}
			else
			{
				// The server was created successfully!
				return true;
			}
		}
	}

	return false;
}

bool CAmtecPStepServer::Terminate( void )
{
	if( PStepServer_proc == nullptr ) return true;

	LPPROCESS_INFORMATION ptrProc_Info = (PROCESS_INFORMATION *) PStepServer_proc;

	unsigned long exit_code = 0;

	if( GetExitCodeProcess( ptrProc_Info->hProcess, &exit_code ) != 0 )
	{
		if( exit_code == STILL_ACTIVE )
		{
			if( TerminateProcess( ptrProc_Info->hProcess, 0 ) != 0 )
			{
				delete ( (PROCESS_INFORMATION *) PStepServer_proc );
				PStepServer_proc = nullptr;

				delete ((LPSTARTUPINFO)PStepServer_startup_info)->lpTitle;
				delete ( (LPSTARTUPINFO) PStepServer_startup_info );
				PStepServer_startup_info = nullptr;
				
				return true;
			}
		}
	}

	return false;
}

