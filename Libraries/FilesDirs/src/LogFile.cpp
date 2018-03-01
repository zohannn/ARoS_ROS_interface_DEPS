#include "LogFile.h"
#include "Files_Directories.h"

CLogFile::CLogFile( std::string str_log_file )
: bLog( false )
, strFile( str_log_file )
{
}

CLogFile::CLogFile( const CLogFile& copy_obj )
: bLog( const_cast<CLogFile&>( copy_obj ).bLog.get() )
, strFile( copy_obj.strFile )
{
	stream_Log << copy_obj.stream_Log.str();
}

CLogFile::~CLogFile( void )
{
	CloseLogFile();
	strFile.clear();
	stream_Log.clear();
}

void CLogFile::set_FilePath( std::string file_path )
{
	strFile = file_path;
}

std::string CLogFile::get_FilePath( void )
{
	return strFile;
}

bool CLogFile::CreateLogFile( std::string file_path, std::string header )
{
	if( !file_path.empty() )
		strFile = file_path;

	// Open the file to write and close, this clears the existing file, if already exists
	stream_file_log.open( strFile, std::ios_base::out );

	if( !stream_file_log.is_open() ) return false;

	if( !header.empty() )
		stream_file_log << header << std::endl;

	stream_file_log.close();

	return true;
}

bool CLogFile::OpenLogFile( void )
{
	// Open the file to append new content
	if( stream_file_log.is_open() )
		stream_file_log.close();

	stream_file_log.open( strFile, std::ios_base::app );

	return stream_file_log.is_open();
}

void CLogFile::CloseLogFile( void )
{
	if( stream_file_log.is_open() )
		stream_file_log.close();
}

void CLogFile::ToggleLog( void )
{
	if( bLog.get() )
	{
		// If logging enabled, close the log file
		CloseLogFile();
	}

	else
	{
		// If logging disabled, check for log file existance, if not present, create it.
		if( !FileExists( strFile ) ) CreateLogFile( strFile );

		// Open the log file
		OpenLogFile();
	}

	// Toggle log flag
	bLog.set( !bLog.get() );
}

bool CLogFile::is_Log_Active( void )
{
	return bLog.get();
}

bool CLogFile::is_Log_File_Open( void )
{
	return stream_file_log.is_open();
}

void CLogFile::Log_to_File( void )
{
	if( !stream_file_log.is_open() ) return;
	
	if( bLog.get() && OpenLogFile() )
	{
		Parse_Log();
		stream_file_log << stream_Log.str();
		
		stream_Log.str( "" );
		stream_Log.clear();
	}
}
