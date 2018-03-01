#ifndef _FRAME_SEQUENCE_WRITER_H_
#define _FRAME_SEQUENCE_WRITER_H_

#pragma once

#include <opencv2/core/core.hpp>
#include <string>

#include "ConfigFile.h"
#include "LogFile.h"

/// <summary>Implements the saving of sequences of images in the hard drive,
/// creating multiple folders for every sequence.
/// <para>Useful when applied to a grabber to save all captured images.</para>
/// <para>It's possible to configure the folder where to save, the file name,
/// and extension.</para></summary>
class CFrameSequenceWriter
	: public CConfigFile
	, public CLogFile
{
//---------------------------------------------------------------------------
// Construction/Destruction
public:
	/// <summary>Default constructor.</summary>
	CFrameSequenceWriter( bool use_log_file=false );

//---------------------------------------------------------------------------
// CConfigFile parsing function
public:
	/// <summary><see cref="CConfigFile"/> parsing function. Parse loaded parameters.</summary>
	bool Parse_Parameters_Load( void );

//---------------------------------------------------------------------------
// Member variables
protected:
	// Strings that provide information about file names and folders where to save the frames
	std::string
		/// <summary>Base folder where sequences will be saved.</summary>
		strFolderBase,
		/// <summary>Name prefix to the folder for each sequence.</summary>
		strFolderPrefix,
		/// <summary>Main folder path.</summary>
		strFolderPath,
		/// <summary>The file prefix.</summary>
		strFilePrefix,
		/// <summary>The file extension.</summary>
		strFileExtension,
		/// <summary>Information to log.</summary>
		strLog;

	bool
		/// <summary>Enable saving?</summary>
		bFrameSaving_enabled,
		/// <summary>Is this a new sequence?</summary>
		bStartNewSequence,
		/// <summary>Use a log file to store metadata about the sequence?</summary>
		bUseLogFile;

	/// <summary>Counter to track the number of each frame.</summary>
	unsigned int uFrameCount;

//---------------------------------------------------------------------------
// Frame saving control
public:
	/// <summary>Turn frame saving on or off.</summary>
	void Toggle_Frame_Saving( void );

	/// <summary>Save this frame to the configured location.</summary>
	/// <param name="frame">Image to save.</param>
	/// <param name="extra_info_filename">Extra information to save in the file name.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool Save_Frame( cv::Mat frame, std::string extra_info_filename = "", std::string info_metadata = "" );

	/// <summary>Configure a new frame sequence, sets up a new folder where to put the new files.</summary>
	/// <returns>true if successful, otherwise false.</returns>
	bool Config_New_Frame_Sequence( void );

	/// <summary>Parse information to be stored in the metadata file.</summary>
	void Parse_Log( void );

//---------------------------------------------------------------------------
// Accessors
public:
	/// <summary>Check the frame saving status.</summary>
	/// <returns>true when frame saving is enabled, false when it's disabled.</returns>
	bool get_FrameSaving_enabled( void );

	/// <summary>Define the base folder.</summary>
	/// <param name="folder">Path to base folder.</param>
	void set_Folder_Base( std::string folder );

	/// <summary>Define the prefix for folder name.</summary>
	/// <param name="pref">Prefix.</param>
	void set_Folder_Prefix( std::string pref );

	/// <summary>Define the prefix for file name.</summary>
	/// <param name="pref">Prefix.</param>
	void set_File_Prefix( std::string pref );

	/// <summary>Define the extension of the file name.</summary>
	/// <param name="pref">Extension for file name.</param>
	void set_File_Extension( std::string ext );
};

#endif // _FRAME_SEQUENCE_WRITER_H_
