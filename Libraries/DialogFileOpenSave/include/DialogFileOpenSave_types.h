#ifndef _DIALOG_FILE_OPEN_SAVE_TYPES_H_
#define _DIALOG_FILE_OPEN_SAVE_TYPES_H_

#pragma once

#include "DialogFileOpenSave.h"

class CDialogFileOpenSave_ini : public CDialogFileOpenSave
{
public:
	CDialogFileOpenSave_ini( void );
};

class CDialogFileOpenSave_bmp : public CDialogFileOpenSave
{
public:
	CDialogFileOpenSave_bmp( void );
};

class CDialogFileOpenSave_avi : public CDialogFileOpenSave
{
public:
	CDialogFileOpenSave_avi( void );
};

class CDialogFileOpenSave_images : public CDialogFileOpenSave
{
public:
	CDialogFileOpenSave_images( void);
};

#endif // _DIALOG_FILE_OPEN_SAVE_TYPES_H_
