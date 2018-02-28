#include "FrameSequenceWriter.h"

#include <iomanip>
#include <time.h>
#include <opencv2/highgui/highgui.hpp>

#include "Files_Directories.h"
#include "string_operations.h"

CFrameSequenceWriter::CFrameSequenceWriter( bool use_log_file )
: strFolderBase( "./" )
, strFolderPrefix( "Seq" )
, strFilePrefix( "Frame" )
, strFileExtension( "png" )
, strLog( "" )
, bFrameSaving_enabled( false )
, bStartNewSequence( false )
, bUseLogFile( use_log_file )
, uFrameCount( 0 )
{
}

bool CFrameSequenceWriter::Parse_Parameters_Load( void )
{
	yarp::os::Bottle bot_fs = prop_ConfigFile.findGroup( "FrameSaving" );

	std::string str_aux = bot_fs.find( "Folder" ).asString();

	if( DirectoryExists( str_aux ) )
		strFolderBase = str_aux;

	string_replace_all( strFolderBase, "//", "/" );

	if( strFolderBase.back() != '/' )
		strFolderBase += '/';

	str_aux = bot_fs.find( "FolderPrefix" ).asString();
	if( str_aux.size() > 0 )
		strFolderPrefix = str_aux;

	str_aux = bot_fs.find( "FilePrefix" ).asString();
	if( str_aux.size() > 0 )
		strFilePrefix = str_aux;

	str_aux = bot_fs.find( "FileExtension" ).asString();
	if( str_aux.size() > 0 )
		strFileExtension = str_aux;

	return true;
}

void CFrameSequenceWriter::Toggle_Frame_Saving( void )
{
	if( !bFrameSaving_enabled )
		bStartNewSequence = true;

	else
		CloseLogFile();

	bFrameSaving_enabled = !bFrameSaving_enabled;
}

bool CFrameSequenceWriter::Save_Frame( cv::Mat frame, std::string extra_info_filename, std::string info_metadata )
{
	if( bStartNewSequence )
		if( !Config_New_Frame_Sequence() ) return false;

	std::stringstream strmFileName;

	strmFileName << 
		strFolderPath << 
		strFilePrefix << 
		"_" << 
		std::setw( 5 ) << std::setfill( '0' ) << 
		uFrameCount;
	
	if( !extra_info_filename.empty() )
		strmFileName << "_" << extra_info_filename;
	
	if( !info_metadata.empty() && bUseLogFile )
	{
		// Extra information to log
		strLog = info_metadata;
		Log_to_File();
	}

	strmFileName << "." << strFileExtension << '\0';	// Add zero to force string termination

	uFrameCount++;
	
	bool success = false;

	if( strFileExtension.size() > 0 )
		success = cv::imwrite( strmFileName.str(), frame );

	frame.release();

	strmFileName.clear();

	return success;
}

bool CFrameSequenceWriter::get_FrameSaving_enabled( void )
{
	return bFrameSaving_enabled;
}

bool CFrameSequenceWriter::Config_New_Frame_Sequence( void )
{
	bool bNewDirAvailable = false;

	const int fail_count_limit = 10;
	int attempt = 0;

	time_t rawtime;
	time( &rawtime );

	struct tm * timeinfo = localtime( &rawtime );

	std::stringstream strmFolderPath;

	do
	{
		strmFolderPath.str( "" );
		strmFolderPath.clear();

		strmFolderPath <<
			strFolderBase << strFolderPrefix << "_" <<
			timeinfo->tm_year + 1900 << "-" <<			// Year: info has number of years after 1900, hence, adding 1900
			std::setw( 2 ) << std::setfill( '0' ) <<	// set number width as two digits and fill the rest with zeros
			timeinfo->tm_mon + 1 << "-" <<				// Month: info has number of months since january, hence, adding 1
			std::setw( 2 ) << std::setfill( '0' ) <<	// set number width as two digits and fill the rest with zeros
			timeinfo->tm_mday << "_" <<					// Day of the month
			std::setw( 2 ) << std::setfill( '0' ) <<	// set number width as two digits and fill the rest with zeros
			timeinfo->tm_hour << "h" <<					// Hour
			std::setw( 2 ) << std::setfill( '0' ) <<	// set number width as two digits and fill the rest with zeros
			timeinfo->tm_min << "m_" <<					// Minutes
			std::setw( 2 ) << std::setfill( '0' ) <<	// set number width as two digits and fill the rest with zeros
			attempt;									// Attempt counter

		if( !DirectoryExists( strmFolderPath.str() ) )
		{
			if( CreateDirectory( CA2T( strmFolderPath.str().c_str() ), NULL ) )
			{
				strFolderPath = strmFolderPath.str();
				strFolderPath += "/";
				bNewDirAvailable = true;

				// Configure log file
				if( CreateLogFile( strFolderPath + "sequence_info.log", "Frame# | dt" ) )
				{
					bLog.set( OpenLogFile() );
				}
			}
			else
			{
				attempt++;
			}
		}
		else
		{
			attempt++;
		}

	} while( !bNewDirAvailable && ( attempt < fail_count_limit ) );

	bStartNewSequence = false;

	uFrameCount = 0;

	return ( attempt < fail_count_limit );
}

void CFrameSequenceWriter::set_Folder_Base( std::string folder )
{
	strFolderBase = folder;
}

void CFrameSequenceWriter::set_Folder_Prefix( std::string pref )
{
	strFolderPrefix = pref;
}

void CFrameSequenceWriter::set_File_Prefix( std::string pref )
{
	strFilePrefix = pref;
}

void CFrameSequenceWriter::set_File_Extension( std::string ext )
{
	strFileExtension = ext;
}

void CFrameSequenceWriter::Parse_Log( void )
{
	stream_Log << std::setw( 5 ) << std::setfill( '0' ) << uFrameCount << "\t" << strLog << std::endl;
}
