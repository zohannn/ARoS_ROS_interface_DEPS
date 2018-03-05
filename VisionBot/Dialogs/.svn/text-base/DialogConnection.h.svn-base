#if !defined(AFX_DIALOGCONNECTION_H__1CA68228_23E4_4C15_A42E_B03DB429A39D__INCLUDED_)
#define AFX_DIALOGCONNECTION_H__1CA68228_23E4_4C15_A42E_B03DB429A39D__INCLUDED_

#pragma once

#include "ConfigFile.h"

#include <afxwin.h>

class CDialogConnection 
	: public CDialog
	, public CConfigFile
{
// Construction
public:
	CDialogConnection(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogConnection)
	enum { IDD = IDD_DIALOG_CONNECTION };
	CEdit	m_edit_Peer_Name;
	CEdit	m_edit_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConnection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConnection)
	//virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd * parent;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonConnectionStartAll();
	afx_msg void OnBnClickedButtonConnectionStopAll();
	afx_msg void OnBnClickedButtonConnectionStartStop();
	afx_msg void OnLbnSelchangeListComms();

public:
	CListBox m_List_Comms;
	CStatic m_static_Status;

public:
	void Configure_Dialog( void );
	
protected:
	void PopulateCommsList( void );

	void RefreshGUI( void );
	
	//---------------------------------------------------------------------------
protected:
	// Parsing rules to interpret the configuration file
	//bool Parse_Parameters_Load(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONNECTION_H__1CA68228_23E4_4C15_A42E_B03DB429A39D__INCLUDED_)
