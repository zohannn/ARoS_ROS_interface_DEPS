#if !defined(AFX_DIALOGVIDEOPARAMETERS_H__61E9F61A_E824_438D_AB46_00442E555602__INCLUDED_)
#define AFX_DIALOGVIDEOPARAMETERS_H__61E9F61A_E824_438D_AB46_00442E555602__INCLUDED_

#pragma once

#include <afxcmn.h>

#include "DialogFileOpenSave_types.h"

class CDialogVideoParameters 
	: public CDialog
	, public CDialogFileOpenSave_ini
{
public:
	// Construction
	CDialogVideoParameters(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogVideoParameters)
	enum { IDD = IDD_DIALOG_VIDEO_PARAMETERS };
	CEdit	m_edit_saturation;
	CEdit	m_edit_red;
	CEdit	m_edit_green;
	CEdit	m_edit_blue;
	CEdit	m_edit_gamma;
	CEdit	m_edit_gain;
	CEdit	m_edit_exposure;
	CEdit	m_edit_brightness;
	CSpinButtonCtrl	m_spin_saturation;
	CSpinButtonCtrl	m_spin_red;
	CSpinButtonCtrl m_spin_green;
	CSpinButtonCtrl	m_spin_blue;
	CSpinButtonCtrl	m_spin_gamma;
	CSpinButtonCtrl	m_spin_gain;
	CSpinButtonCtrl	m_spin_exposure;
	CSpinButtonCtrl	m_spin_brightness;
	CButton	m_check_balance;
	CButton	m_check_gain;
	CButton	m_check_exposure;
	CButton	m_check_brightness;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogVideoParameters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogVideoParameters)
	afx_msg void OnChangeEditExposure();
	afx_msg void OnChangeEditBlue();
	afx_msg void OnChangeEditBrightness();
	afx_msg void OnChangeEditGain();
	afx_msg void OnChangeEditGamma();
	afx_msg void OnChangeEditRed();
	afx_msg void OnEnChangeEditGreen();
	afx_msg void OnButtonLoadVideoPar();
	afx_msg void OnButtonSaveVideoPar();
	afx_msg void OnChangeEditSaturation();
	afx_msg void OnCheckExposure();
	afx_msg void OnCheckGain();
	afx_msg void OnCheckBrightness();
	afx_msg void OnCheckBalance();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWnd * parent;

	void UpdateExposure();
	void UpdateGain();
	void UpdateBrightness();
	void UpdateGamma();
	void UpdateWhiteBalance();
	void UpdateSaturation();

	void LoadParametersFromGrabber( void );

public:
	void Enable_Control_Exposure( bool enable );
	void Enable_Control_Gain( bool enable );
	void Enable_Control_Brightness( bool enable );
	void Enable_Control_Saturation( bool enable );
	void Enable_Control_Gamma( bool enable );
	void Enable_Control_WB_Red( bool enable );
	void Enable_Control_WB_Green( bool enable );
	void Enable_Control_WB_Blue( bool enable );

	void Configure_Dialog( void );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGVIDEOPARAMETERS_H__61E9F61A_E824_438D_AB46_00442E555602__INCLUDED_)
