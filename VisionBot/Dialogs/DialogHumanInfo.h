#ifndef _DIALOG_HUMAN_INFO_H_
#define _DIALOG_HUMAN_INFO_H_

#pragma once

#include <afxwin.h>
#include <opencv2/core/core.hpp>

#include "AttachMutex.hpp"
#include "ConfigFile.h"
#include "DialogFileOpenSave_types.h"

class CDialogHumanInfo 
	: public CDialog
	, public CConfigFile
	, public CDialogFileOpenSave_ini
{
	DECLARE_DYNAMIC( CDialogHumanInfo )

public:
	// standard constructor
	CDialogHumanInfo( CWnd* pParent = NULL );
	virtual ~CDialogHumanInfo() {};

// Dialog Data
	enum { IDD = IDD_DIALOG_HUMAN_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CWnd * ptr_parent;

public:
	afx_msg void OnBnClickedCheckShowImageMovement();
	afx_msg void OnBnClickedCheckShowImageStereo();
	afx_msg void OnEnChangeEditXmin();
	afx_msg void OnEnChangeXmax();
	afx_msg void OnEnChangeEditYmin();
	afx_msg void OnEnChangeEditYmax();
	afx_msg void OnEnChangeEditZmin();
	afx_msg void OnEnChangeEditZmax();
	afx_msg void OnEnChangeEditMovHandIdle();
	afx_msg void OnEnChangeEditMovHandLow();
	afx_msg void OnEnChangeEditMovHandMedium();
	afx_msg void OnEnChangeEditMovBodyIdle();
	afx_msg void OnEnChangeEditMovBodyLow();
	afx_msg void OnEnChangeEditMovBodyMedium();
	afx_msg void OnBnClickedButtonHumanInfoSave();
	afx_msg void OnBnClickedButtonHumanInfoLoad();
	afx_msg void OnBnClickedCheckHandMovement();
	afx_msg void OnBnClickedCheckBodyMovement();
	CButton m_check_image_movement;
	CButton m_check_image_stereo;
	CButton m_check_movement_hand;
	CButton m_check_movement_body;
	CSpinButtonCtrl m_spin_xmin;
	CSpinButtonCtrl m_spin_xmax;
	CSpinButtonCtrl m_spin_ymin;
	CSpinButtonCtrl m_spin_ymax;
	CSpinButtonCtrl m_spin_zmin;
	CSpinButtonCtrl m_spin_zmax;
	CEdit m_edit_mov_hand_idle;
	CEdit m_edit_mov_hand_low;
	CEdit m_edit_mov_hand_medium;
	CEdit m_edit_mov_body_idle;
	CEdit m_edit_mov_body_low;
	CEdit m_edit_mov_body_medium;

protected:
	void SetupControls( void );

	void ReadValueFromSpinCtrl( CSpinButtonCtrl * pSpin, CAttachMutex_t< cv::Point3f > * pSemaphore, float * pValue );

	bool isCtrlValid( CWnd * pCtrl );

	bool Parse_Parameters_Load( void );
	void Parse_Parameters_Save( void );

public:
	void LoadParameters( void );

	void Configure_Dialog( void );
};

#endif // _DIALOG_HUMAN_INFO_H_
