#pragma once

#include "DialogFileOpenSave.h"
#include "GestureClassifier.h"

//#include "afxcmn.h"
#include "afxwin.h"

// CDialogGestureParameters dialog

class CDialogGestureParameters 
	: public CDialog
	, public CDialogFileOpenSave_ini
{
	DECLARE_DYNAMIC(CDialogGestureParameters)

public:
	CDialogGestureParameters(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogGestureParameters();

// Dialog Data
	enum { IDD = IDD_DIALOG_GESTURE_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_slider_hand_hue_min;
	CSliderCtrl m_slider_hand_hue_max;
	CSliderCtrl m_slider_hand_sat_min;
	CSliderCtrl m_slider_hand_sat_max;
	CSliderCtrl m_slider_hand_val_max;
	CSliderCtrl m_slider_hand_val_min;
	CButton m_radio_hand_open;
	CButton m_radio_hand_close;
	CSpinButtonCtrl m_spin_hand_erode;
	CSpinButtonCtrl m_spin_hand_dilate;
	CStatic m_static_hand_hue_min;
	CStatic m_static_hand_hue_max;
	CStatic m_static_hand_sat_min;
	CStatic m_static_hand_sat_max;
	CStatic m_static_hand_val_min;
	CStatic m_static_hand_val_max;
	CEdit m_edit_hu1_ag_min;
	CEdit m_edit_hu2_ag_min;
	CEdit m_edit_hu3_ag_min;
	CEdit m_edit_hu1_ag_max;
	CEdit m_edit_hu2_ag_max;
	CEdit m_edit_hu3_ag_max;
	CEdit m_edit_hu1_sg_min;
	CEdit m_edit_hu2_sg_min;
	CEdit m_edit_hu3_sg_min;
	CEdit m_edit_hu1_sg_max;
	CEdit m_edit_hu2_sg_max;
	CEdit m_edit_hu3_sg_max;
	CEdit m_edit_hu1_pt_min;
	CEdit m_edit_hu2_pt_min;
	CEdit m_edit_hu3_pt_min;
	CEdit m_edit_hu1_pt_max;
	CEdit m_edit_hu2_pt_max;
	CEdit m_edit_hu3_pt_max;
	CEdit m_edit_hu_power_m1;
	CEdit m_edit_hu_power_m2;
	CEdit m_edit_hu_power_m3;
	CSpinButtonCtrl m_spin_hand_point_side_diff;
	CButton m_check_gesture_active;
	afx_msg void OnBnClickedRadioOpen();
	afx_msg void OnBnClickedRadioClose();
	afx_msg void OnNMCustomdrawSliderMinHue( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnNMCustomdrawSliderMaxHue( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnNMCustomdrawSliderMinSat( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnNMCustomdrawSliderMaxSat( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnNMCustomdrawSliderMinVal( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnNMCustomdrawSliderMaxVal( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnEnChangeEditErode();
	afx_msg void OnEnChangeEditDilate();
	afx_msg void OnBnClickedButtonLoadFile();
	afx_msg void OnBnClickedButtonSaveFile();
	afx_msg void OnEnChangeEditPointSideDiff();
	afx_msg void OnBnClickedCheckGestureActive();
	afx_msg void OnBnClickedButtonUpdateHuAbove();
	afx_msg void OnBnClickedButtonUpdateHuPointing();
	afx_msg void OnBnClickedButtonUpdateHuSide();
	afx_msg void OnBnClickedButtonUpdateHuPower();

protected:
	CGestureClassifier * pGestureClassifier;

	void RefreshGUI( void );

private:
	CWnd * parent;

	CEdit 
		*pEdit_Power[3],
		*pEdit_AG[6],
		*pEdit_SG[6],
		*pEdit_PT[6];

	void Setup_pointers_to_controls( void );

public:
	void set_GestureClassifier_pointer( CGestureClassifier * ptr );

	void Configure_Dialog( void );
};
