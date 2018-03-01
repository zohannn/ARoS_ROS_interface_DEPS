#ifndef _DIALOG_FILE_OPEN_SAVE_H_
#define _DIALOG_FILE_OPEN_SAVE_H_

#pragma once

#include <string>

class CFileDialog;

class CDialogFileOpenSave
{
public:
	//-----------------------------------------------------------------------
	// Construction / Destruction
	CDialogFileOpenSave( void );
	virtual ~CDialogFileOpenSave( void );

	// Generic function that uses a file dialog and returns its file name and path
	// Do not use this! The called dialog does not return the file path properly.
	std::string Prompt_User_Get_Filename( CFileDialog * pDlg );

	//-----------------------------------------------------------------------
public:
	// string objects that hold information about file extensions and filters
	std::string
		strDefaultExtension,
		strFileFilters;

	const unsigned long ulFlags;
};

#endif // _DIALOG_FILE_OPEN_SAVE_H_
