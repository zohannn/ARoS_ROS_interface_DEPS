#ifndef _DIALOG_INFO_H_
#define _DIALOG_INFO_H_

#pragma once

#include <afxwin.h>

#include "Data.h"

class CDialogInfo
	: public CDialog
{
public:
	CDialogInfo( int id_resource = NULL, CWnd* parent = nullptr, CData* data = nullptr );
	~CDialogInfo( void );

protected:
	CWnd* pParent;
	CData* pData;

public:
	void set_DataSource( CData *pDataSource ){ pData = pDataSource; };

public:
	virtual void AlignWindow( void ) { /* do nothing */};
};

#endif // _DIALOG_INFO_H_
