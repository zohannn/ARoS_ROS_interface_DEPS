#ifndef _DIALOG_INFO_AROS_H_
#define _DIALOG_INFO_AROS_H_

#pragma once

#include <afxwin.h>

#include "DialogInfo.h"
#include "ansi_unicode_definitions.h"

class CDialogInfoARoS
	: public CDialogInfo
{
	DECLARE_DYNAMIC(CDialogInfoARoS)

public:
	CDialogInfoARoS( CWnd* pParent = nullptr, CData* pDataStore = nullptr );   // standard constructor
	virtual ~CDialogInfoARoS();

// Dialog Data
	enum { IDD = IDD_DIALOG_INFO_AROS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static_object_info;
	CButton m_check_object_info;
	CStatic m_static_actions;
	CButton m_check_actions;
	CStatic m_static_csg;
	CButton m_check_csg;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

public:
	void Start_Info_Timers( void );
	void Stop_Info_Timers( void );

public:
	void AlignWindow( void );
	void UpdateInfo_Objects( void );
	void UpdateInfo_Objects_Task_Construction( void );
	void UpdateInfo_Objects_Task_FaceTracking( void );
	void UpdateInfo_Objects_Task_PlayPiano( void );
	void UpdateInfo_Objects_Task_HRTransport( void );
	void UpdateInfo_Objects_Task_DiffKinematics( void );
	void UpdateInfo_Objects_Task_Drinking( void );
	void UpdateInfo_Objects_Task_ComputerAssembly(void);
	void UpdateInfo_Actions( void );
	void UpdateInfo_Actions_Construction( void );
	void UpdateInfo_Actions_Drinking( void );
	void UpdateInfo_Actions_ComputerAssembling(void);
	void UpdateInfo_CSG( void );
	void UpdateInfo_CSG_ToyRobotConstruction( void );
	void UpdateInfo_CSG_ToyRobotLearning( void );

private:
	std::vector< std::string > vsObjects_ToyRobotLearning;

	std::vector<bool> vTimerOn;

protected:
	std::tstring get_Quantity_as_tstring( int quant );
};

#endif // _DIALOG_INFO_AROS_H_
