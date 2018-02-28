#include <fstream>
#include <io.h>			// For access().
#include <sys/types.h>	// For stat().
#include <sys/stat.h>	// For stat().
#include <sstream>
#include <direct.h>

#include "Files_Directories.h"
#include "string_operations.h"

bool FileExists( std::string file_name )
{
	// Replace all ocurrences of '//' with '/', this speeds the process
	string_replace_all( file_name, "//", "/" );

	std::ifstream strmFile;

	// Attempt to open the file, don't create it if it doesn't exist
	strmFile.open( file_name.c_str(), std::ios::_Nocreate );

	// is the file opened?
	if( strmFile.is_open() )
	{
		// close it if it's open
		strmFile.close();
		return true;
	}
	else return false;
}

bool DirectoryExists( std::string path_dir )
{
	bool bExists = false;

	std::string test_path = path_dir;

	//-----------------------------------------------------------------------------
	// Remove a slash at the end of the folder path if present
	// When running in release, if the path ends with a '/', the folder is
	// considered not existing, even if it does.

	string_replace_all( test_path, "//", "/" );
	string_replace_all( test_path, "\\", "/" );

	if( test_path.back() == '/' )
		test_path.pop_back();
	
	//-----------------------------------------------------------------------------

	if( _access( test_path.c_str(), 0 ) == 0 )
	{
		struct stat status;
		stat( test_path.c_str(), &status );

		if( status.st_mode & S_IFDIR )
			bExists = true;
		//else
			//The path is a file
	}

	return bExists;
}

bool Find_File( std::string file_name, std::string &file_path, std::vector< std::string > vecExtraDirectories, std::vector< std::string > vecDirVariations )
{
	std::stringstream strmFile;

	std::vector< std::string > vecDirectories;
	vecDirectories.emplace_back( "." );
	vecDirectories.emplace_back( ".." );
	vecDirectories.emplace_back( "../.." );
	vecDirectories.emplace_back( "../../.." );

	std::vector< std::string > vecVariations;
	vecVariations.emplace_back( "." );
	vecVariations.emplace_back( "Config" );
	vecVariations.emplace_back( "Conf" );
	vecVariations.emplace_back( "config" );
	vecVariations.emplace_back( "conf" );
	
	size_t vecDir_size = vecDirVariations.size();

	if( vecDir_size > 0 )
	{
		for( size_t iVar=0 ; iVar<vecDir_size ; iVar++ )
		{
			vecVariations.emplace_back( vecDirVariations.at( iVar ) );
		}
	}

	size_t vecExtraDir_size = vecExtraDirectories.size();
	if( vecExtraDir_size > 0 )
	{
		for( unsigned iDir=0 ; iDir<vecExtraDir_size ; iDir++ )
		{
			vecDirectories.emplace_back( vecExtraDirectories.at( iDir ) );
		}
	}

	size_t
		nDir = vecDirectories.size(),
		nVar = vecVariations.size();

	std::string strFilePath_aux = "";

	for( size_t i=0 ; i<nDir ; i++ )
	{
		for( size_t j=0 ; j<nVar ; j++ )
		{
			strmFile.str( "" );
			strmFile.clear();
			strmFile << vecDirectories.at( i ) << "/" << vecVariations.at( j ) << "/" << file_name;

			strFilePath_aux = strmFile.str();

			string_replace_all( strFilePath_aux, "//", "/" );

			if( FileExists( strFilePath_aux ) )
			{
				file_path = strFilePath_aux;
				return true;
			}
		}
	}

	return false;
}

bool CreateDirectory( std::string dir_path )
{
	return ( _mkdir( dir_path.c_str() ) == 0 );
}
