// VisionBotDlg.h : header file
//

#if !defined(AFX_VISIONBOTDLG_H__E9043B9F_D39F_419C_9362_B9EE72CC987D__INCLUDED_)
#define AFX_VISIONBOTDLG_H__E9043B9F_D39F_419C_9362_B9EE72CC987D__INCLUDED_

#pragma once

#include <afxwin.h>

//---------------------------------------------------------------------------
#include "VisionBot.h"

//---------------------------------------------------------------------------
#include "Data.h"
#include "ConfigFile.h"

//---------------------------------------------------------------------------
// Project Dialogs
#include "DialogConnection.h"
#include "DialogFileOpenSave_types.h"
#include "DialogGestureParameters.h"
#include "DialogHumanInfo.h"
#include "DialogPanTilt.h"
#include "DialogSegmentation.h"
#include "DialogVideoParameters.h"
#include "DialogYARPPortConfig.h"

//---------------------------------------------------------------------------
// Information Dialogs
#include "DialogInfo.h"

//---------------------------------------------------------------------------
// Project Threads

#ifdef USE_YarpCommunication
	#include <yarp/os/Network.h>
	#include "YarpCommVisionBot.h"
#endif

#include "ProcessThread.h"

#include <vector>
#include <tuple>

//---------------------------------------------------------------------------

class CVisionBotDlg 
	: public CDialog
	, public CConfigFile
{
public:
	// standard constructor
	CVisionBotDlg( CWnd* pParent = nullptr );
	~CVisionBotDlg( void );

// Dialog Data
	//{{AFX_DATA(CVisionBotDlg)
	enum { IDD = IDD_VISIONBOT_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionBotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVisionBotDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMainMenuStart();
	afx_msg void OnMainMenuStop();
	afx_msg void OnMainMenuExit();
	afx_msg void OnMainMenuYARPConnections();
	afx_msg void OnMainMenuPanTilt();
	afx_msg void OnMainMenuSegmentation();
	afx_msg void OnMainMenuSegmentationFeedback();
	afx_msg void OnMainMenuVideoParameters();
	afx_msg void OnImageProcessing();
	afx_msg void OnImageStereoprocessing();
	afx_msg void OnImageSourceFile();
	afx_msg void OnImageSourceSvsfile();
	afx_msg void OnImageSourceSvsvideo();
	afx_msg void OnImageSourceOpencvgrabber();
	afx_msg void OnImageSourceCmu1394();
	afx_msg void OnImageSaveToFile();
	afx_msg void OnImageFramesaving();
	afx_msg void OnMainTaskTestStuff();
	afx_msg void OnMainTaskConstruction();
	afx_msg void OnMainTaskDrinking();
	afx_msg void OnMainMenuStartHandAcq();
	afx_msg void OnMainMenuStopHandAcq();
	afx_msg void OnMainTaskTumIntegration();
	afx_msg void OnImageSourceYarpport();
	afx_msg void OnImageBroadcast();
	afx_msg void OnImageSourcePS3Eye();
	afx_msg void OnMainTaskConstructionToyRobotFullSVS();
	afx_msg void OnMainTaskConstructionToyRobotFullAVT();
	afx_msg void OnMainTaskColourLearning();
	afx_msg void OnMainTaskFaceTracking();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMainTaskMobileRobotTargetAcquisition();
	afx_msg void OnMainTaskToyRobotLearning();
	afx_msg void OnMenuImageStreamOriginal();
	afx_msg void OnMenuImageStreamProcessed();
	afx_msg void OnImageShowimage();
	afx_msg void OnMainTaskPlayPiano();
	afx_msg void OnMainTaskHRTransport();
	afx_msg void OnMainTaskDifferentialKinematics();
	afx_msg void OnImageSourceVideoFile();
	afx_msg void OnMainMenuHumanInfo();
	afx_msg void OnStereoDisplayDisparity();
	afx_msg void OnStereoSaveDisparityImage();
	afx_msg void OnMainmenuGestureparameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------
private:
	void Clear_Application( void );

	bool bTimerOn;

//---------------------------------------------------------------------------
// Threads
public:
	CProcessThread * pThreadProc;

//---------------------------------------------------------------------------
// Communications
#ifdef USE_YarpCommunication

private:
	yarp::os::Network yarp_net;

public:
	std::vector< CYarpCommVisionBot * > pThreadComm;

#endif

//---------------------------------------------------------------------------
// The object responsible to store all the data for the program
public:
	CData * pData;

//---------------------------------------------------------------------------
// GUI objects
private:
	CStatic	m_Frame_Counter;

	CButton 
		m_check_actions,
		m_check_csg,
		m_check_frame_rate;

	// Pointer to the application' main menu
	CMenu * main_menu;

private:
	// Auxiliary vectors that hold the identifiers for items in main menu
	std::vector<int> 
		vMenuItem_ImageSource,	// Menu items in Image Source selection sub-menu
		vMenuItem_Task;			// Menu items in Task selection sub-menu

//---------------------------------------------------------------------------
// Auxiliary objects
public:
	std::vector< int > vTaskObjects;

private:
	std::string
		strVisionBot_DIR,
		str_AdditionalConfig_file;
	//Stores the grabber selected in config file, if any.
	int nInitial_Grabber;
	//Stores whether grabber image should be presented or not, as selected in config file.
	bool bShow_Image;
	//Stores whether grabber image should be processed or not, as selected in config file.
	bool bProcessing;
	//Stores whether app should start the grabber or not, as selected in config file.
	bool bStart_Grabber;
	//Store communications options. tuple: <string, string, string, bool> <ID, portname, portnameserver, enable>
	std::vector<std::tuple<std::string, std::string, std::string, bool>> comm_opt;

//---------------------------------------------------------------------------
// Functions that control the interaction with the dialog
private:
	// Setup this dialog's controls
	void SetupControls( void );
	
	// Setup additional dialogs
	void CreateExtraDialogs( void );
	void DestroyExtraDialogs( void );
	
	// Setup App menu
	void SetupMenu( void );
	
	// Setup communication objects
	void SetupCommunications( void );
	
	// Configure App timers
	void StartTimers( void );
	void StopTimers( void );

	// Select in main menu the current Image source and uncheck all others
	void SelectMenu_ImageSource( int img_source );
	
	// Select in main menu the current task and uncheck all others
	void SelectMenu_Task( int task );

public:
	// Update frame rate in main window
	void UpdateInfo_FrameRate( void );

	// Update check status in 'Image' menu for the item 'Image Processing'
	void Update_MenuState_ImageProcessing( void );
	
	// Update check status in 'Image' menu for the item 'Stereo processing'
	void Update_MenuState_StereoProcessing( void );

	// Update check status in 'Stereo' menu for the item 'Display disparity'
	void Update_MenuState_DisplayDisparity( void );

	// Update check status in 'Image' menu for the item 'Frame Saving'
	void Update_MenuState_FrameSaving( void );

	// Configure GUI with options for the selected grabber
	void SelectGrabber( int grabber, std::string file_name = "" );

protected:
	// Configure GUI with options for the selected task
	void SelectTask( int num_of_task );

//---------------------------------------------------------------------------
// Dialog Objects
protected:
	CDialogConnection dlgConnection;
	CDialogPanTilt dlgPanTilt;
	CDialogSegmentation dlgSeg;
	CDialogVideoParameters dlgVideoPar;
	CDialogYARPPortConfig dlgYARPPortConf;
	CDialogHumanInfo dlgHumanInfo;
	CDialogGestureParameters dlgGestureParams;

	CDialogInfo *pDlgInfo;

//---------------------------------------------------------------------------
// Load/Save dialogs
protected:
	CDialogFileOpenSave_bmp	dlgLoadSVSFile;

	CDialogFileOpenSave_images dlgLoadSaveImageFile;
	
	CDialogFileOpenSave_avi dlgLoadVideoFile;

//---------------------------------------------------------------------------
public:
	// Generic function that writes text 'item_txt' in 'item'
	void set_MenuItem_Text( unsigned int item, std::string item_txt );

//---------------------------------------------------------------------------
protected:
	// Parsing rules to interpret the configuration file
	bool Parse_Parameters_Load( void );

public:
	afx_msg void OnMainTaskComputerassembly();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONBOTDLG_H__E9043B9F_D39F_419C_9362_B9EE72CC987D__INCLUDED_)
