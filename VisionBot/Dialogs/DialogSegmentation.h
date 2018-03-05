#if !defined(AFX_DIALOGSEGMENTATION_H__5BBAC910_D402_4C44_BD7E_74C519DCD728__INCLUDED_)
#define AFX_DIALOGSEGMENTATION_H__5BBAC910_D402_4C44_BD7E_74C519DCD728__INCLUDED_

#pragma once

#include <afxcmn.h>
#include <vector>

#include "SegmentedImage.h"

class CDialogSegmentation 
	: public CDialog
	, public CDialogFileOpenSave_ini
{
public:
	// Construction
	CDialogSegmentation( CWnd* pParent = nullptr );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSegmentation)
	enum { IDD = IDD_DIALOG_SEGMENTATION };
	CButton	m_radio_open;
	CButton	m_radio_close;
	CSpinButtonCtrl	m_spin_erode;
	CSpinButtonCtrl	m_spin_dilate;
	CStatic	m_static_val_min;
	CStatic	m_static_sat_min;
	CStatic	m_static_hue_min;
	CStatic	m_static_val_max;
	CStatic	m_static_sat_max;
	CStatic	m_static_hue_max;
	CSliderCtrl	m_slider_val_min;
	CSliderCtrl	m_slider_sat_min;
	CSliderCtrl	m_slider_hue_min;
	CSliderCtrl	m_slider_val_max;
	CSliderCtrl	m_slider_sat_max;
	CSliderCtrl	m_slider_hue_max;
	CComboBox	m_combo_colour;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSegmentation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSegmentation)
	afx_msg void OnNMCustomdrawSliderMaxHue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMaxSat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMaxVal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMinHue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMinSat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMinVal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboColour();
	afx_msg void OnButtonLoadFile();
	afx_msg void OnButtonSaveFile();
	afx_msg void OnRadioOpen();
	afx_msg void OnRadioClose();
	afx_msg void OnChangeEditDilate();
	afx_msg void OnChangeEditErode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd * parent;
	int nSelectedColour;

	std::vector< CSegmentedImage * > vSegImage;

	void SelectColour( int nColour );

	void LoadColourParameters( int nColour );

public:
	void Configure_Dialog( void );

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSEGMENTATION_H__5BBAC910_D402_4C44_BD7E_74C519DCD728__INCLUDED_)
