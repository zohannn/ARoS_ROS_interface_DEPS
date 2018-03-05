#ifndef _DIALOG_YARP_PORT_CONFIG_H_
#define _DIALOG_YARP_PORT_CONFIG_H_

#pragma once

#include <afxwin.h>
#include <string>

class CDialogYARPPortConfig 
	: public CDialog
{
	DECLARE_DYNAMIC( CDialogYARPPortConfig )

public:
	// Constructor
	CDialogYARPPortConfig( CWnd* pParent = nullptr, bool config_remote = true );

// Dialog Data
	enum { IDD = IDD_DIALOG_YARPPORT_CONFIGURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

protected:
	CStatic m_static_remote;

protected:
	std::string
		strPortName_Local,
		strPortName_Remote;

	CWnd * parent;
	
	CEdit 
		m_Yarp_Port_Local,
		m_Yarp_Port_Remote;

	bool bConfigRemote;

public:
	std::string get_PortName_Local( void );
	std::string get_PortName_Remote( void );

	void set_PortName_Local( std::string port );
	void set_PortName_Remote( std::string port );
};

#endif // _DIALOG_YARP_PORT_CONFIG_H_
