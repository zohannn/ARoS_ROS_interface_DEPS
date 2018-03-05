#ifndef _ABOUT_DIALOG_H_
#define _ABOUT_DIALOG_H_

#include "StdAfx.h"
#include "resource.h"

class CAboutDlg 
	: public CDialog
{
public:
	CAboutDlg( void );

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _ABOUT_DIALOG_H_
