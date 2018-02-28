#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#pragma once

#include <string>

/// <summary>CConfigFile implments the management of configuration files.
/// <para>Provides an abstraction layer to the required functions of managing files.
/// Handles file creation, opening, closing, reading and writing.</para>
/// <para>The user has to only define the formatting rules to write, read or both, the
/// selected configuration file for each situation.</para>
///</summary>
class CConfigFile
{
public:
	///	<summary>Creates an instance of CConfigFile.</summary>
	CConfigFile( void );
	
	///	<summary>Copy constructor. Creates a copy of a CConfigFile object.</summary>
	CConfigFile( const CConfigFile& obj );

	///	<summary>Destroys the instance of a CConfigFile object.</summary>
	~CConfigFile( void );

public:
	///	<summary>Define the path for the config file.</summary>
	///	<param name="file_path">Path to the used file.</param>
	///	<returns>true if config file exists and was successfully defined, false otherwise.</returns>
	bool set_Config_File( std::string file_path );

	///	<summary>Query if there is a valid configuration file available.</summary>
	///	<returns>true if a config file exists, false otherwise.</returns>
	bool is_Valid( void );

	///	<summary>Load the parameters from the file.
	/// If no parameter is given, the function will try to load the preconfigured file.
	/// <para>If there is a valid file, the function will open it, invoke
	/// <see ref="CConfigFile::Parse_Parameters_Load"/> to parse the data 
	/// from the file.</para></summary>
	///	<param name="file_path">File path.</param>
	///	<returns>true if successful, false otherwise.</returns>
	bool Load_parameters_from_file( std::string file_path = "" );

	///	<summary>Save parameters to file.
	/// If no parameter is given, the function will try to save to the preconfigured file.
	/// <para>If there is a valid file, the function will open it, invoke
	/// <see ref="CConfigFile::Parse_Parameters_Save"/> to format data,
	/// and write to the file.</para></summary>
	///	<param name="file_path">File path.</param>
	///	<returns>true if successful, false otherwise.</returns>
	bool Save_parameters_to_file( std::string file_path = "" );

protected:
	///	<summary>Parse the parameters when loading from file.
	/// <para>This function is invoked by <see ref="CConfigFile::Parse_Parameters_Load"/> 
	/// to translate data read from file into usable structures inside the class
	/// where is being used.</para>
	/// <para>This function must be implemented in a derived class if
	/// reading parameters from a file is required.</para></summary>
	virtual bool Parse_Parameters_Load( void ) { return false; };

	///	<summary>Parse the parameters when saving to file.
	/// <para>This function is invoked by <see ref="CConfigFile::Parse_Parameters_Save"/> 
	/// to translate data from structures inside the class where is being used 
	/// into a file format defined by the user.</para>
	/// <para>This function must be implemented in a derived class if
	/// saving parameters to a file is required.</para></summary>
	virtual void Parse_Parameters_Save( void ) {};

protected:
	std::string 
		///	<summary>File path to be used.</summary>
		strConfigFile,
		///	<summary>Buffer that holds the information to be written to a file.</summary>
		strFileBuffer;


};

#endif // _CONFIG_FILE_H_
