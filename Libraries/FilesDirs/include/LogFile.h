#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#pragma once

#include <fstream>
#include <string>
#include <sstream>

#include "AttachMutex.hpp"

/// <summary>CLogFile handles log files management.
/// <para>Provides a management of the file itself: creation, exsitence, opening and closing.</para>
///</summary>
class CLogFile
{
public:
	/// <summary>Creates an instance of CLogFile.</summary>
	/// <param name="str_log_file">Name for the log file.</param>
	CLogFile( std::string str_log_file = "" );

	/// <summary>Copy constructor. Creates a copy of another CLogFile object.</summary>
	/// <param name="copy_obj">Object to be copied.</param>
	CLogFile( const CLogFile& copy_obj );

	/// <summary>Destroys the instance of a CLogFile object.</summary>
	~CLogFile( void );

private:
	/// <summary>Internal object that stores the file name.</summary>
	std::string	strFile;

protected:
	/// <summary>File stream of the log file itself.</summary>
	std::ofstream stream_file_log;

	/// <summary>Mutex protected bool that flags if the logging is active or not.</summary>
	CAttachMutex_n<bool> bLog;

	/// <summary>Stream that contains information to be logged.</summary>
	std::stringstream stream_Log;

public:
	/// <summary>Is logging active?</summary>
	/// <returns>true if the logging to file is active, false if not.</returns>
	bool is_Log_Active( void );

	/// <summary>Check to see if the log file is opened.</summary>
	/// <returns>true if the file is opened, false if not.</returns>
	bool is_Log_File_Open( void );

public:
	/// <summary>Creates the file.</summary>
	/// <param name="file_path">Path to the file.</param>
	/// <returns>true if the file was created successfully, false otherwise.</returns>
	bool CreateLogFile( std::string file_path = "", std::string header = "" );
	
	/// <summary>Get the current log file.</summary>
	/// <returns>string with file path.</returns>
	std::string get_FilePath( void );
	
	/// <summary>Set the current log file.</summary>
	/// <param name="file_path">Path and file name.</param>
	void set_FilePath( std::string file_path );
	
	/// <summary>Opens the log file.</summary>
	/// <returns>true if the file was opened successfully, false otherwise.</returns>
	bool OpenLogFile( void );
	
	/// <summary>Closes the log file.</summary>
	/// <returns>true if the file was closed successfully, false otherwise.</returns>
	void CloseLogFile( void );
	
	/// <summary>Switches the file logging between active and inactive, 
	/// see also <see cref="CLogFile::bLog"/>.</summary>
	void ToggleLog( void );
	
	/// <summary>Writes data to log file formatted according to 
	/// <see cref="CLogFile::Parse_Log"/>.</summary>
	void Log_to_File( void );

	/// <summary>Virtual function that implements what and how the data is written 
	/// in the file this function must be implemented in the derived class, defining 
	/// the formatting rules for each situation.</summary>
	virtual void Parse_Log( void ) {};
};

#endif // _LOG_FILE_H_
