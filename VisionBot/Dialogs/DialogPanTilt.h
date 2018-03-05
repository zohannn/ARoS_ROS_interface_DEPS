#if !defined(AFX_DIALOGPANTILT_H__B643E6B9_63BD_4638_8AF5_125E5CB36D79__INCLUDED_)
#define AFX_DIALOGPANTILT_H__B643E6B9_63BD_4638_8AF5_125E5CB36D79__INCLUDED_

#pragma once

#include <afxwin.h>

#include "ansi_unicode_definitions.h"

class CDialogPanTilt 
	: public CDialog
{
// Construction
public:
	CDialogPanTilt( CWnd* pParent = nullptr );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPanTilt)
	enum { IDD = IDD_DIALOG_PANTILT };
	
	CStatic	
		m_static_pan,
		m_static_tilt,
		m_static_units_pan,
		m_static_units_tilt;
	
	CEdit 
		m_edit_pan,
		m_edit_tilt,
		m_edit_pos_x,
		m_edit_pos_y,
		m_edit_pos_z;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPanTilt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CWnd * parent;

protected:
	bool SendCommand( unsigned int command, std::vector<float> vData = std::vector<float>() );

	int get_ControlMode( void );

public:
	void RefreshPan( void );
	void RefreshTilt( void );

	void RefreshPanPosition( void );
	void RefreshTiltPosition( void );

	void RefreshPanVelocity( void );
	void RefreshTiltVelocity( void );

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPanTilt)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonSetPan();
	afx_msg void OnButtonGetPan();
	afx_msg void OnButtonSetTilt();
	afx_msg void OnButtonGetTilt();
	afx_msg void OnButtonSetPanTilt();
	afx_msg void OnButtonGetPanTilt();
	afx_msg void OnBnClickedButtonStopPan();
	afx_msg void OnBnClickedButtonStopTilt();
	afx_msg void OnBnClickedButtonStopPantilt();
	afx_msg void OnBnClickedButtonInitPan();
	afx_msg void OnBnClickedButtonInitTilt();
	afx_msg void OnBnClickedButtonInitPantilt();
	afx_msg void OnBnClickedButtonLookToPosition();
	afx_msg void OnBnClickedRadioControlModePosition();
	afx_msg void OnBnClickedRadioControlModeVelocity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_radio_control_position;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPANTILT_H__B643E6B9_63BD_4638_8AF5_125E5CB36D79__INCLUDED_)
