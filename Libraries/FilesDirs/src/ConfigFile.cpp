#include "ConfigFile.h"
#include <fstream>
#include "Files_Directories.h"

CConfigFile::CConfigFile( void )
: strConfigFile( "" )
, strFileBuffer( "" )
{
}

CConfigFile::CConfigFile( const CConfigFile& obj )
: strConfigFile( obj.strConfigFile )
, strFileBuffer( obj.strFileBuffer )
, prop_ConfigFile( obj.prop_ConfigFile )
{
}

CConfigFile::~CConfigFile( void )
{
	strConfigFile.clear();
	strFileBuffer.clear();

	prop_ConfigFile.clear();
}

bool CConfigFile::set_Config_File( std::string file_path )
{
	if( FileExists( file_path ) )
	{
		strConfigFile = file_path;
		return true;
	}
	
	else return false;
}

bool CConfigFile::Load_parameters_from_file( std::string file_path )
{
	if( !file_path.empty() )
	{
		set_Config_File( file_path );
	}

	if( !is_Valid() ) return false;

	if( !prop_ConfigFile.fromConfigFile( strConfigFile.c_str() ) )
		return false;

	return Parse_Parameters_Load();
}

bool CConfigFile::Save_parameters_to_file( std::string file_path )
{
	Parse_Parameters_Save();

	std::ofstream file_stream( file_path, std::fstream::out );

	if( !file_stream.is_open() ) return false;

	file_stream << strFileBuffer;

	file_stream.close();

	return true;
}

bool CConfigFile::is_Valid( void )
{
	return !strConfigFile.empty();
}
