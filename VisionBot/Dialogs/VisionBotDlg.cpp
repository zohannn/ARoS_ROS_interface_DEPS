#include "VisionBotDlg.h"

#if WINVER>=0x0600
#include <afxdialogex.h>
#endif

#include <opencv2/highgui/highgui.hpp>

#include "AboutDlg.h"
#include "ansi_unicode_definitions.h"
#include "DialogInfoARoS.h"
#include "EnvironmentVars.h"
#include "Files_Directories.h"
#include "Grabber.h"
#include "GrabberDefinitions.h"
#include "Timers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisionBotDlg::CVisionBotDlg( CWnd* pParent )
: CDialog( CVisionBotDlg::IDD, pParent )
, pThreadProc( nullptr )
, pData( nullptr )
, pDlgInfo( nullptr )
, dlgConnection( this )
, dlgPanTilt( this )
, dlgSeg( this )
, dlgVideoPar( this )
, dlgYARPPortConf( this )
, dlgHumanInfo( this )
, dlgGestureParams( this )
, str_AdditionalConfig_file( "" )
, strVisionBot_DIR( "" )
, bTimerOn( false )
, vMenuItem_ImageSource( GrabberType::NUM_OF_GRABBERS, 0 )
, vMenuItem_Task( Task::NUMBER_OF_TASKS, 0 )
{
	//{{AFX_DATA_INIT(CVisionBotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//--------------------------------------------------------------------------
	// Load default yarp communications
	//load default names
	comm_opt.emplace_back(
	std::make_tuple(
	Comm::ID[Comm::ID_Vision_Server],
	Comm::PortName[Comm::ID_Vision_Server],
	Comm::PeerName[Comm::ID_Vision_Server],
	false));


	comm_opt.emplace_back(
	std::make_tuple(
	Comm::ID[Comm::ID_Pan_Tilt],
	Comm::PortName[Comm::ID_Pan_Tilt],
	Comm::PeerName[Comm::ID_Pan_Tilt],
	false));

	//--------------------------------------------------------------------------
	// Get the source directory where VisionBot is, if the environment variable VISIONBOT_DIR exists.
	strVisionBot_DIR = get_Env_Var_ANSI( "VISIONBOT_DIR" );

	std::vector< std::string > 
		vecDir, 
		vecVar;

	// Extra directory to look for
	vecDir.emplace_back( strVisionBot_DIR );

	// Variations/Subdirectories to look for
	vecVar.emplace_back( "/Config/" );
	vecVar.emplace_back( "/Config/Parameters/" );

	std::string 
		file_name = "AppConfig.ini",
		file_path = "";

	bool is_ConfigFile_present = Find_File( file_name, file_path, vecDir, vecVar );
	
	if( is_ConfigFile_present )
		Load_parameters_from_file( file_path );

	//--------------------------------------------------------------------------
	try
	{
		pData = new CData();

	// Load main parameters in CData and CVisionBotDlg
		if( is_ConfigFile_present )
		{
			pData->Load_parameters_from_file( file_path );
		}

		// Load Extra parameters for CData
		if( !str_AdditionalConfig_file.empty() )
		{
			if( Find_File( str_AdditionalConfig_file, file_path, vecDir, vecVar ) )
			{
				pData->HumanInfo.Load_parameters_from_file( file_path );
			}
		}	
	}
	catch( std::bad_alloc& ex )
	{
		std::tstring strError = _T( "Data object failed to be instantiated! Exception caught: " );
		strError += CA2T( ex.what() );

		AfxMessageBox( strError.c_str(), MB_ICONERROR );

		pData = nullptr;
	}

	//--------------------------------------------------------------------------

	vMenuItem_ImageSource.at( GrabberType::SVS_Video ) = ID_IMAGE_SOURCE_SVSVIDEO;
	vMenuItem_ImageSource.at( GrabberType::SVS_Image_File ) = ID_IMAGE_SOURCE_SVSFILE;
	vMenuItem_ImageSource.at( GrabberType::Common_Image_File ) = ID_IMAGE_SOURCE_FILE;
	vMenuItem_ImageSource.at( GrabberType::OpenCV_Camera ) = ID_IMAGE_SOURCE_OPENCVCAMERA;
	vMenuItem_ImageSource.at( GrabberType::YARP_Port ) = ID_IMAGE_SOURCE_YARPPORT;
	vMenuItem_ImageSource.at( GrabberType::CMU1394 ) = ID_IMAGE_SOURCE_CMU1394;
	vMenuItem_ImageSource.at( GrabberType::PS3Eye ) = ID_IMAGE_SOURCE_PS3EYE;
	vMenuItem_ImageSource.at( GrabberType::OpenCV_Video_File ) = ID_IMAGE_SOURCE_OPENCV_VIDEO_FILE;

	//--------------------------------------------------------------------------
	
	vMenuItem_Task.at( Task::TEST ) = ID_MAIN_TASK_TEST_STUFF;
	vMenuItem_Task.at( Task::CONSTRUCTION_TOY_STEREO ) = ID_MAIN_TASK_CONSTRUCTION;
	vMenuItem_Task.at( Task::DRINKING ) = ID_MAIN_TASK_DRINKING;
	vMenuItem_Task.at( Task::TUM_INTEGRATION ) = ID_MAIN_TASK_TUM_INTEGRATION;
	vMenuItem_Task.at( Task::CONSTRUCTION_TOY_FULL_SVS ) = ID_MAIN_TASK_CONSTRUCTIONTOYROBOTFULL_SVS;
	vMenuItem_Task.at( Task::CONSTRUCTION_TOY_FULL_AVT ) = ID_MAIN_TASK_CONSTRUCTIONTOYROBOTFULL_AVT;
	vMenuItem_Task.at( Task::COLOUR_LEARNING ) = ID_MAIN_TASK_COLOURLEARNING;
	vMenuItem_Task.at( Task::FACE_TRACKING ) = ID_MAIN_TASK_FACETRACKING;
	vMenuItem_Task.at( Task::MOBILE_ROBOT_TARGET_ACQUISITION ) = ID_MAIN_TASK_MOBILE_ROBOT_TARGET_ACQUISITION;
	vMenuItem_Task.at( Task::TOY_ROBOT_LEARNING ) = ID_MAIN_TASK_TOYROBOTLEARNING;
	vMenuItem_Task.at( Task::PLAY_PIANO ) = ID_MAIN_TASK_PLAYPIANO;
	vMenuItem_Task.at( Task::HR_TRANSPORT ) = ID_MAIN_TASK_HRTRANSPORT;
	vMenuItem_Task.at( Task::DIFF_KINEMATICS ) = ID_MAIN_TASK_DIFFERENTIAL_KINEMATICS;
	vMenuItem_Task.at( Task::COMPUTER_ASSEMBLY) = ID_MAIN_TASK_COMPUTERASSEMBLY;
}

CVisionBotDlg::~CVisionBotDlg( void )
{
	Clear_Application();
}

void CVisionBotDlg::Clear_Application( void )
{
	StopTimers();

	if( pThreadProc != nullptr )
	{
		if( pThreadProc->image_procObject != nullptr )
			if( pThreadProc->image_procObject->isFeedbackActive() )
			{
			pThreadProc->image_procObject->HideSegmentationFeedback();
			main_menu->CheckMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_UNCHECKED );
			}

	#ifdef USE_SVS
		if( pThreadProc->pStereoProcess != nullptr )
			if( pThreadProc->pStereoProcess->objStereo != nullptr )
				if( pThreadProc->pStereoProcess->objStereo->get_DisplayDisparityWindow() )
				{
			OnStereoDisplayDisparity();
				}
	#endif

		if( pThreadProc->isRunning() )
			pThreadProc->stop();

		delete pThreadProc;
		pThreadProc = nullptr;
	}

#ifdef USE_YarpCommunication
	size_t num_comms = pThreadComm.size();
	for( size_t i = 0; i<num_comms; i++ )
	{
		if( pThreadComm.at( i ) == nullptr ) continue;

		if( pThreadComm.at( i )->isOpened() ) pThreadComm.at( i )->close();

		delete pThreadComm.at( i );

		pThreadComm.at( i ) = nullptr;
	}

	pThreadComm.clear();

	yarp_net.fini();

#endif // USE_YarpCommunication

	DestroyExtraDialogs();

	if( pData != nullptr )
	{
		delete pData;
		pData = nullptr;
	}

	//cv::destroyAllWindows();
}

bool CVisionBotDlg::Parse_Parameters_Load(void)
{
	bool bSuccess = true;

	yarp::os::Bottle bot_main = prop_ConfigFile.findGroup("App_Config");

	int task = bot_main.find("Task").asInt();
	int image_source = bot_main.find("Image_Source").isInt() ? bot_main.find("Image_Source").asInt() : -1;
	std::string show_image = bot_main.check("Show_Image") ? bot_main.find("Show_Image").asString() : "true";
	std::string processing = bot_main.check("Processing") ? bot_main.find("Processing").asString() : "false";
	std::string start_grabber = bot_main.check("Start_Grabber") ? bot_main.find("Start_Grabber").asString() : "false";

	std::string str_ParName = "Additional_Config";
	if (bot_main.check(str_ParName.c_str()))
	{
		str_AdditionalConfig_file = bot_main.find(str_ParName.c_str()).asString();
	}
	else bSuccess = false;

	// Default value for the task
	if (pData != nullptr)
	{
		int nTheTask = Task::CONSTRUCTION_TOY_FULL_SVS;
		if (task >= 0 && task < Task::NUMBER_OF_TASKS) nTheTask = task;
		pData->set_Task(nTheTask);
	}

	//Default value for Image Source (Grabber)
	nInitial_Grabber = GrabberType::SVS_Video;
	if (image_source >= 0 && image_source < GrabberType::NUM_OF_GRABBERS) nInitial_Grabber = image_source;

	bShow_Image = show_image.compare("true") == 0;
	bProcessing = processing.compare("true") == 0;
	bStart_Grabber = start_grabber.compare("true") == 0;

	//get communications options
	yarp::os::Bottle bot_visionserver = prop_ConfigFile.findGroup("VisionServer_Config");
	if (!bot_visionserver.isNull())	//load default names
	{
		std::string portname = bot_visionserver.check("portname") ? bot_visionserver.find("portname").asString() : "";
		bool enable = bot_visionserver.check("enable") ? bot_visionserver.find("enable").asString() == "true" : false;
		comm_opt[Comm::ID_Vision_Server] = std::make_tuple(
			Comm::ID[Comm::ID_Vision_Server],
			portname,
			Comm::PeerName[Comm::ID_Vision_Server],
			enable);
	}

	yarp::os::Bottle bot_pantiltclient = prop_ConfigFile.findGroup("PanTiltClient_Config");
	if (!bot_pantiltclient.isNull())	//load default names
	{
		std::string portname = bot_pantiltclient.check("portname") ? bot_pantiltclient.find("portname").asString() : "";
		std::string portnameserver = bot_pantiltclient.check("portnameserver") ? bot_pantiltclient.find("portnameserver").asString() : "";
		bool enable = bot_pantiltclient.check("enable") ? bot_pantiltclient.find("enable").asString() == "true" : false;
		comm_opt[Comm::ID_Pan_Tilt] = std::make_tuple(
			Comm::ID[Comm::ID_Pan_Tilt],
			portname,
			portnameserver,
			enable);
	}
	return bSuccess;
}

void CVisionBotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionBotDlg)
	DDX_Control(pDX, IDC_STATIC_FRAME_COUNTER, m_Frame_Counter);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_FRAME_RATE, m_check_frame_rate);
}

BEGIN_MESSAGE_MAP(CVisionBotDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionBotDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_COMMAND(ID_MAIN_MENU_START, OnMainMenuStart)
	ON_COMMAND(ID_MAIN_MENU_STOP, OnMainMenuStop)
	ON_COMMAND(ID_MAIN_MENU_EXIT, OnMainMenuExit)
	ON_COMMAND(ID_MAIN_MENU_CONNECT_ROUTER, OnMainMenuYARPConnections)
	ON_COMMAND(ID_MAIN_MENU_PAN_TILT, OnMainMenuPanTilt)
	ON_COMMAND(ID_MAIN_MENU_SEGMENTATION, OnMainMenuSegmentation)
	ON_COMMAND(ID_MAIN_MENU_SEGMENTATION_FEEDBACK, OnMainMenuSegmentationFeedback)
	ON_COMMAND(ID_MAIN_MENU_VIDEO_PARAMETERS, OnMainMenuVideoParameters)
	ON_COMMAND(ID_MAIN_MENU_START_HAND_ACQ, OnMainMenuStartHandAcq)
	ON_COMMAND(ID_MAIN_MENU_STOP_HAND_ACQ, OnMainMenuStopHandAcq)
	ON_COMMAND(ID_MAIN_MENU_HUMAN_INFO, OnMainMenuHumanInfo)
	ON_COMMAND(ID_IMAGE_PROCESSING, OnImageProcessing)
	ON_COMMAND(ID_IMAGE_SOURCE_FILE, OnImageSourceFile)
	ON_COMMAND(ID_IMAGE_SOURCE_SVSFILE, OnImageSourceSvsfile)
	ON_COMMAND(ID_IMAGE_SOURCE_SVSVIDEO, OnImageSourceSvsvideo)
	ON_COMMAND(ID_IMAGE_SOURCE_OPENCVCAMERA, OnImageSourceOpencvgrabber)
	ON_COMMAND(ID_IMAGE_SOURCE_YARPPORT, OnImageSourceYarpport)
	ON_COMMAND(ID_IMAGE_SOURCE_PS3EYE, OnImageSourcePS3Eye)
	ON_COMMAND(ID_IMAGE_SOURCE_OPENCV_VIDEO_FILE, OnImageSourceVideoFile)
	ON_COMMAND(ID_IMAGE_SAVE_TO_FILE, OnImageSaveToFile)
	ON_COMMAND(ID_IMAGE_SHOWIMAGE, OnImageShowimage)
	ON_COMMAND(ID_IMAGE_STEREOPROCESSING, OnImageStereoprocessing)
	ON_COMMAND(ID_IMAGE_FRAMESAVING, OnImageFramesaving)
	ON_COMMAND(ID_IMAGE_SOURCE_CMU1394, OnImageSourceCmu1394)
	ON_COMMAND(ID_MAIN_TASK_TEST_STUFF, OnMainTaskTestStuff)
	ON_COMMAND(ID_MAIN_TASK_CONSTRUCTION, OnMainTaskConstruction)
	ON_COMMAND(ID_MAIN_TASK_DRINKING, OnMainTaskDrinking)
	ON_COMMAND(ID_MAIN_TASK_TUM_INTEGRATION, OnMainTaskTumIntegration)
	ON_COMMAND(ID_MAIN_TASK_CONSTRUCTIONTOYROBOTFULL_SVS, OnMainTaskConstructionToyRobotFullSVS)
	ON_COMMAND(ID_MAIN_TASK_CONSTRUCTIONTOYROBOTFULL_AVT, OnMainTaskConstructionToyRobotFullAVT)
	ON_COMMAND(ID_MAIN_TASK_COLOURLEARNING, OnMainTaskColourLearning)
	ON_COMMAND(ID_MAIN_TASK_FACETRACKING, OnMainTaskFaceTracking)
	ON_COMMAND(ID_MAIN_TASK_MOBILE_ROBOT_TARGET_ACQUISITION, OnMainTaskMobileRobotTargetAcquisition)
	ON_COMMAND(ID_MAIN_TASK_TOYROBOTLEARNING, OnMainTaskToyRobotLearning)
	ON_COMMAND(ID_MAIN_TASK_PLAYPIANO, OnMainTaskPlayPiano)
	ON_COMMAND(ID_MAIN_TASK_HRTRANSPORT, OnMainTaskHRTransport)
	ON_COMMAND(ID_MAIN_TASK_DIFFERENTIAL_KINEMATICS, OnMainTaskDifferentialKinematics)
	ON_COMMAND(ID_MENU_IMAGE_STREAM_ORIGINAL, OnMenuImageStreamOriginal)
	ON_COMMAND(ID_MENU_IMAGE_STREAM_PROCESSED, OnMenuImageStreamProcessed)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_STEREO_DISPLAY_DISPARITY, &CVisionBotDlg::OnStereoDisplayDisparity)
	ON_COMMAND(ID_STEREO_SAVEDISPARITYIMAGE, &CVisionBotDlg::OnStereoSaveDisparityImage)
	ON_COMMAND( ID_MAINMENU_GESTUREPARAMETERS, &CVisionBotDlg::OnMainmenuGestureparameters )
	ON_COMMAND(ID_MAIN_TASK_COMPUTERASSEMBLY, &CVisionBotDlg::OnMainTaskComputerassembly)
END_MESSAGE_MAP()

BOOL CVisionBotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if( pData == nullptr )
	{
		AfxMessageBox( _T( "Data object failed to be instantiated" ), MB_ICONERROR );
		return FALSE;
	}

	//--------------------------------------------------------------------------
	// Configure Processing Thread
	try
	{
		pThreadProc = new CProcessThread( pData->get_Task() );
	}
	catch( std::bad_alloc& ex )
	{
		std::tstring strError = _T( "Process thread failed to be instantiated! Exception caught: " );
		strError += CA2T( ex.what() );

		AfxMessageBox( strError.c_str(), MB_ICONERROR );
		return FALSE;
	}

	// What camera will be used
	pThreadProc->ConfigureCurrentGrabber();
	
	if( pThreadProc->image_procObject != nullptr )
	{
		// Where to keep the data
		pThreadProc->image_procObject->set_DataSource( pData );
	
		// Save in DataSource a pointer to the image processing object
		pData->image_procObject = (void*) pThreadProc->image_procObject;

		// Do image processing to start?
		pThreadProc->image_procObject->doImageProcessing( false );
	}
	else
	{
		AfxMessageBox( _T( "Image processing object had an error while being initialized" ), MB_ICONEXCLAMATION );
	}

	//--------------------------------------------------------------------------
	// Configure main App
	
	SetupMenu();

	SetupCommunications();
	
	CreateExtraDialogs();

	SetupControls();

	//--------------------------------------------------------------------------
	// redefine window position

	RECT rect;
	GetWindowRect( &rect );

	int 
		width = static_cast<int>( rect.right - rect.left ),
		height = static_cast<int>( rect.bottom - rect.top );
	
	SetWindowPos( nullptr, 1, 1, width, height, SWP_NOZORDER );

	//--------------------------------------------------------------------------

	SelectTask( pData->get_Task() );

	//Set the file options
	//grabber type
	SelectGrabber(nInitial_Grabber);

	//show image
	main_menu->CheckMenuItem(ID_IMAGE_SHOWIMAGE, bShow_Image ? MF_CHECKED : MF_UNCHECKED);
	pThreadProc->image_procObject->set_ImageDisplayStatus(bShow_Image);

	//processing
	main_menu->CheckMenuItem(ID_IMAGE_PROCESSING, bProcessing ? MF_CHECKED : MF_UNCHECKED);
	pThreadProc->image_procObject->doImageProcessing(bProcessing);

	//start grabber
	if (bStart_Grabber)
		OnMainMenuStart();

	// All set, ready to go!

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisionBotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisionBotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisionBotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CVisionBotDlg::SetupControls()
{
	// Configure checkboxes
	m_check_frame_rate.SetCheck( BST_CHECKED );
}

void CVisionBotDlg::SetupMenu( void )
{
	//--------------------------------------------------------------------------
	//Setup the application menu
	main_menu = GetMenu();
	main_menu->EnableMenuItem( ID_MAIN_MENU_START, MF_ENABLED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_STOP, MF_GRAYED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_GRAYED );

	main_menu->EnableMenuItem( ID_MAIN_MENU_START_HAND_ACQ, MF_ENABLED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_STOP_HAND_ACQ, MF_GRAYED );

	if( pData == nullptr ) return;

	SelectMenu_Task( pData->get_Task() );

	//--------------------------------------------------------------------------
	// Configure image source at menu
	SelectMenu_ImageSource( pThreadProc->get_GrabberType() );	

	//--------------------------------------------------------------------------
	auto state = MF_UNCHECKED;

	//--------------------------------------------------------------------------
	// Image processing check menu
	Update_MenuState_ImageProcessing();

	//--------------------------------------------------------------------------
	// Stereo processing check menu
	Update_MenuState_StereoProcessing();

	//--------------------------------------------------------------------------
	// Display stereo disparity image
	Update_MenuState_DisplayDisparity();

	#ifndef USE_SVS
	main_menu->EnableMenuItem( ID_STEREO_DISPLAY_DISPARITY, MF_GRAYED );
	#endif

	//--------------------------------------------------------------------------
	// Frame saving enabled?
	Update_MenuState_FrameSaving();

	//--------------------------------------------------------------------------
	// Image streaming enabled?
	state = pThreadProc->bStream_Image_Original.get() ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_ORIGINAL, state );

	std::string item_text = "Original -> ";
	item_text += pThreadProc->imgStream_orig.get_PortName();

	set_MenuItem_Text( ID_MENU_IMAGE_STREAM_ORIGINAL, item_text );

	state = pThreadProc->bStream_Image_Processed.get() ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_PROCESSED, state );

	item_text = "Processed -> ";
	item_text += pThreadProc->imgStream_proc.get_PortName();

	set_MenuItem_Text( ID_MENU_IMAGE_STREAM_PROCESSED, item_text );

	state = pThreadProc->image_procObject->get_ImageDisplayStatus() ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_SHOWIMAGE, state );
}

void CVisionBotDlg::Update_MenuState_ImageProcessing( void )
{
	auto state = pThreadProc->image_procObject->useImageProcessing() ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_PROCESSING, state );
}

void CVisionBotDlg::Update_MenuState_StereoProcessing( void )
{
	auto state = pThreadProc->bUse_Stereo.get() ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_STEREOPROCESSING, state );
}

void CVisionBotDlg::Update_MenuState_DisplayDisparity( void )
{
	auto state = MF_UNCHECKED;

#ifdef USE_SVS
	if( pThreadProc != nullptr )
		if( pThreadProc->pStereoProcess != nullptr )
			if( pThreadProc->pStereoProcess->objStereo != nullptr ) 
				state = pThreadProc->pStereoProcess->objStereo->get_DisplayDisparityWindow() ? MF_CHECKED : MF_UNCHECKED;
#endif

	main_menu->CheckMenuItem( ID_STEREO_DISPLAY_DISPARITY, state );
}

void CVisionBotDlg::Update_MenuState_FrameSaving( void )
{
	auto state = pThreadProc->image_procObject->get_FrameSavingStatus() ? MF_CHECKED : MF_UNCHECKED;
	
	main_menu->CheckMenuItem( ID_IMAGE_FRAMESAVING, state );
}

void CVisionBotDlg::CreateExtraDialogs( void )
{
	// Dialog initialization
	dlgConnection.Configure_Dialog();
	dlgConnection.ShowWindow( SW_HIDE );

	dlgPanTilt.ShowWindow( SW_HIDE );

	dlgSeg.Configure_Dialog();
	dlgSeg.ShowWindow( SW_HIDE );

	dlgVideoPar.Configure_Dialog();
	dlgVideoPar.ShowWindow( SW_HIDE );

	dlgYARPPortConf.ShowWindow( SW_HIDE );

	dlgHumanInfo.Configure_Dialog();
	dlgHumanInfo.ShowWindow( SW_HIDE );

	dlgGestureParams.Configure_Dialog();
	dlgGestureParams.ShowWindow( SW_HIDE );
}

void CVisionBotDlg::DestroyExtraDialogs( void )
{
	// Dialog destruction
	if( dlgConnection.m_hWnd != nullptr )
		dlgConnection.DestroyWindow();

	if( dlgGestureParams.m_hWnd != nullptr )
		dlgGestureParams.DestroyWindow();

	if( dlgHumanInfo.m_hWnd != nullptr )
		dlgHumanInfo.DestroyWindow();

	if( dlgPanTilt.m_hWnd != nullptr )
		dlgPanTilt.DestroyWindow();

	if( dlgSeg.m_hWnd != nullptr )
		dlgSeg.DestroyWindow();

	if( dlgVideoPar.m_hWnd != nullptr )
		dlgVideoPar.DestroyWindow();

	if( dlgYARPPortConf.m_hWnd != nullptr )
		dlgYARPPortConf.DestroyWindow();

	if( pDlgInfo != nullptr )
	{
		if( pDlgInfo->DestroyWindow() == TRUE )
		{
			delete pDlgInfo;
			pDlgInfo = nullptr;
		}
	}
}

void CVisionBotDlg::StartTimers( void )
{
	if( !bTimerOn )
		bTimerOn = ( SetTimer( Timer::VisionBot::ID::FRAME_RATE, Timer::VisionBot::Timeout::FRAME_RATE, NULL ) != 0 );
}

void CVisionBotDlg::StopTimers( void )
{
	if( bTimerOn )
		bTimerOn = ( KillTimer( Timer::VisionBot::ID::FRAME_RATE ) == FALSE );
}

void CVisionBotDlg::SetupCommunications( void )
{
#ifdef USE_YarpCommunication

	pThreadComm.assign( Comm::TOTAL_COMM_DST, nullptr );

	std::string 
		sPort = "/",
		sPeer = "/";

	sPort += pData->get_RobotName();
	sPort += std::get<1>(comm_opt[Comm::ID_Vision_Server]);

	pThreadComm.at( Comm::ID_Vision_Server ) = new 
		#ifndef _DEBUG 
			(std::nothrow) 
		#endif
		CYarpCommVisionBot( sPort );
	
	if( pThreadComm.at( Comm::ID_Vision_Server ) == nullptr ) return;

	sPort = "/";
	sPort += pData->get_RobotName();
	sPort += std::get<1>(comm_opt[Comm::ID_Pan_Tilt]);

	sPeer += pData->get_RobotName();
	sPeer += std::get<2>(comm_opt[Comm::ID_Pan_Tilt]);

	pThreadComm.at( Comm::ID_Pan_Tilt ) = new
		#ifndef _DEBUG 
			(std::nothrow) 
		#endif
		CYarpCommVisionBot( sPort, sPeer, false );

	if( pThreadComm.at( Comm::ID_Pan_Tilt ) == nullptr ) return;

	size_t num_comms = pThreadComm.size();
	// Pass a pointer to the data source in each comm thread
	for( size_t i=0 ; i<num_comms ; i++ )
	{
		pThreadComm.at( i )->set_DataSource( pData );
		pThreadComm.at( i )->set_VisionBotDlg_Ptr( this );
	}

	pThreadComm.at( Comm::ID_Vision_Server )->set_YarpComm_PanTilt_Pointer( pThreadComm.at( Comm::ID_Pan_Tilt ) );

	if (std::get<3>(comm_opt[Comm::ID_Vision_Server]))
		pThreadComm.at(Comm::ID_Vision_Server)->open();

	if (std::get<3>(comm_opt[Comm::ID_Pan_Tilt]))
		pThreadComm.at(Comm::ID_Pan_Tilt)->open();

#endif
}

void CVisionBotDlg::OnOK() 
{
	// TODO: Add extra validation here

	//CDialog::OnOK();
}

void CVisionBotDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// Assign a function for the respective timer

	switch( nIDEvent )
	{
	case Timer::VisionBot::ID::FRAME_RATE:
		UpdateInfo_FrameRate();
		break;

#ifdef USE_YarpCommunication
	case Timer::VisionBot::ID::PANTILT:
		if( pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened() )
		{
			pThreadComm.at( Comm::ID_Pan_Tilt )->Send( PANTILT_GET_ANGLE_PANTILT );
		}
		break;
#endif

	}

	CDialog::OnTimer(nIDEvent);
}

void CVisionBotDlg::UpdateInfo_FrameRate( void )
{
	if( m_check_frame_rate.GetCheck() == BST_UNCHECKED ) return;

	float fFPS = pThreadProc->fFrameRate.get();

	std::tstringstream str_text;
	str_text.precision( 4 );
	str_text << "Capturing";

	if( pThreadProc->image_procObject->useImageProcessing() )
		str_text << " and processing";

	str_text << " @ " << fFPS << " fps";

	m_Frame_Counter.SetWindowText( str_text.str().c_str() );
}

void CVisionBotDlg::OnMainMenuStart() 
{
	// TODO: Add your command handler code here
	
	if( pThreadProc->start() )
	{
		main_menu->EnableMenuItem( ID_MAIN_MENU_START, MF_GRAYED );
		main_menu->EnableMenuItem( ID_MAIN_MENU_STOP, MF_ENABLED );
		main_menu->EnableMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_ENABLED );

		main_menu->EnableMenuItem( ID_MAIN_TASK_DRINKING, MF_GRAYED );
		main_menu->EnableMenuItem( ID_MAIN_TASK_CONSTRUCTION, MF_GRAYED );

		StartTimers();
	}
	else
	{
		AfxMessageBox( _T( "Grabber failed to start!" ) );
	}
}

void CVisionBotDlg::OnMainMenuStop() 
{
	// TODO: Add your command handler code here

	main_menu->EnableMenuItem( ID_MAIN_MENU_START, MF_ENABLED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_STOP, MF_GRAYED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_GRAYED );

	main_menu->EnableMenuItem( ID_MAIN_TASK_DRINKING, MF_ENABLED );
	main_menu->EnableMenuItem( ID_MAIN_TASK_CONSTRUCTION, MF_ENABLED );

	if( pThreadProc->image_procObject->isFeedbackActive() )
	{
		pThreadProc->image_procObject->HideSegmentationFeedback();
		main_menu->CheckMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_UNCHECKED );
	}

	StopTimers();

	pThreadProc->stop();
}

void CVisionBotDlg::OnMainMenuExit() 
{
	// TODO: Add your command handler code here

	OnCancel();
}

void CVisionBotDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Clear_Application();

	CDialog::OnCancel();
}

void CVisionBotDlg::OnMainMenuYARPConnections() 
{
	// TODO: Add your command handler code here
	dlgConnection.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainMenuPanTilt() 
{
	// TODO: Add your command handler code here
	dlgPanTilt.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainMenuSegmentation() 
{
	// TODO: Add your command handler code here
	dlgSeg.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainmenuGestureparameters()
{
	// TODO: Add your command handler code here
	dlgGestureParams.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainMenuHumanInfo()
{
	// TODO: Add your command handler code here
	if( dlgHumanInfo.m_hWnd == nullptr ) return;

	dlgHumanInfo.LoadParameters();
	dlgHumanInfo.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainMenuVideoParameters() 
{
	// TODO: Add your command handler code here
	
	if( pThreadProc->grabberObject == nullptr )
	{
		AfxMessageBox( _T( "No grabber running!" ), MB_ICONINFORMATION );
		return;
	}
	
	if( !pThreadProc->grabberObject->has_ParamaterControl() )
	{
		AfxMessageBox( _T( "Video parameter control unavailable for the selected grabber type" ), MB_ICONEXCLAMATION );
		return;
	}

	dlgVideoPar.Enable_Control_Brightness( pThreadProc->grabberObject->has_Control_Brightness() );
	dlgVideoPar.Enable_Control_Exposure( pThreadProc->grabberObject->has_Control_Exposure() );
	dlgVideoPar.Enable_Control_Gain( pThreadProc->grabberObject->has_Control_Gain() );
	dlgVideoPar.Enable_Control_Gamma( pThreadProc->grabberObject->has_Control_Gamma() );
	dlgVideoPar.Enable_Control_Saturation( pThreadProc->grabberObject->has_Control_Saturation() );
	dlgVideoPar.Enable_Control_WB_Blue( pThreadProc->grabberObject->has_Control_WB_Blue() );
	dlgVideoPar.Enable_Control_WB_Green( pThreadProc->grabberObject->has_Control_WB_Green() );
	dlgVideoPar.Enable_Control_WB_Red( pThreadProc->grabberObject->has_Control_WB_Red() );

	dlgVideoPar.ShowWindow( SW_SHOW );
}

void CVisionBotDlg::OnMainMenuSegmentationFeedback() 
{
	// TODO: Add your command handler code here
	if( pThreadProc->image_procObject->isFeedbackActive() )
	{
		pThreadProc->image_procObject->HideSegmentationFeedback();
		main_menu->CheckMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_UNCHECKED );
	}

	else
	{
		pThreadProc->image_procObject->ShowSegmentationFeedback();
		main_menu->CheckMenuItem( ID_MAIN_MENU_SEGMENTATION_FEEDBACK, MF_CHECKED );
	}
}

void CVisionBotDlg::OnImageProcessing() 
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_PROCESSING, MF_BYCOMMAND );

	bool bProc = ( uState == MF_UNCHECKED );

	auto state = bProc ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_PROCESSING, state );
	pThreadProc->image_procObject->doImageProcessing( bProc );
}

void CVisionBotDlg::OnImageShowimage()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_SHOWIMAGE, MF_BYCOMMAND );
	
	bool bDisplay = ( uState == MF_UNCHECKED );

	auto state = bDisplay ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_SHOWIMAGE, state );
	pThreadProc->image_procObject->set_ImageDisplayStatus( bDisplay );
}

void CVisionBotDlg::OnImageStereoprocessing()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_STEREOPROCESSING, MF_BYCOMMAND );
	
	bool bState = ( uState == MF_UNCHECKED );

	auto state = bState ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_STEREOPROCESSING, state );
	pThreadProc->bUse_Stereo.set( bState );
}

void CVisionBotDlg::OnStereoDisplayDisparity()
{
	// TODO: Add your command handler code here
#ifdef USE_SVS
	auto uState = main_menu->GetMenuState( ID_STEREO_DISPLAY_DISPARITY, MF_BYCOMMAND );
	
	bool bState = ( uState == MF_UNCHECKED );

	auto state = bState ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_STEREO_DISPLAY_DISPARITY, state );
	pThreadProc->pStereoProcess->objStereo->set_DisplayDisparityWindow( bState );
#endif
}

void CVisionBotDlg::OnImageSourceFile() 
{
	// TODO: Add your command handler code here

	if( pThreadProc->isRunning() ) return;

	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_FILE, MF_BYCOMMAND );

	if( uState == MF_UNCHECKED )
	{
		dlgVideoPar.ShowWindow( SW_HIDE );

		CFileDialog dlg_open(
			TRUE,
			CA2T( dlgLoadSaveImageFile.strDefaultExtension.c_str() ),
			NULL,
			dlgLoadSaveImageFile.ulFlags, 
			CA2T( dlgLoadSaveImageFile.strFileFilters.c_str() ),
			this );

		std::string image_file_name = "";

		auto nResult = dlg_open.DoModal();

		if( nResult == IDOK )
		{
			std::tstring file_name = _T( "" );
		
			file_name = dlg_open.GetPathName();
		
		#ifdef UNICODE
			image_file_name = CT2A( file_name.c_str() );
		#else
			image_file_name = file_name;
		#endif
		}

		if( !image_file_name.empty() )
		{
			SelectGrabber( GrabberType::Common_Image_File, image_file_name );
		}
	}
}

void CVisionBotDlg::OnImageSourceVideoFile()
{
	// TODO: Add your command handler code here

	if( pThreadProc->isRunning() ) return;

	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_OPENCV_VIDEO_FILE, MF_BYCOMMAND );

	if( uState == MF_UNCHECKED )
	{
		//dlgVideoPar->ShowWindow( SW_HIDE );
		CFileDialog dlg_save(
			FALSE,
			CA2T( dlgLoadVideoFile.strDefaultExtension.c_str() ),
			NULL,
			dlgLoadVideoFile.ulFlags, 
			CA2T( dlgLoadVideoFile.strFileFilters.c_str() ),
			this );

		std::string video_file_name = "";

		auto nResult = dlg_save.DoModal();

		if( nResult == IDOK )
		{
			std::tstring file_name_t = _T( "" );
		
			file_name_t = dlg_save.GetPathName();
		
		#ifdef UNICODE
			video_file_name = CT2A( file_name_t.c_str() );
		#else
			video_file_name = file_name_t;
		#endif
		}

		if( !video_file_name.empty() )
		{
			SelectGrabber( GrabberType::OpenCV_Video_File, video_file_name );
		}
	}
}

void CVisionBotDlg::OnImageSourceSvsfile() 
{
	// TODO: Add your command handler code here

	if( pThreadProc->isRunning() ) return;

	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_SVSFILE, MF_BYCOMMAND );

	if( uState == MF_UNCHECKED )
	{
		dlgVideoPar.ShowWindow( SW_HIDE );

		CFileDialog dlg_open(
			TRUE,
			CA2T( dlgLoadSVSFile.strDefaultExtension.c_str() ),
			NULL,
			dlgLoadSVSFile.ulFlags, 
			CA2T( dlgLoadSVSFile.strFileFilters.c_str() ),
			this );

		std::string svs_file_name = "";

		auto nResult = dlg_open.DoModal();

		if( nResult == IDOK )
		{
			std::tstring file_name_t = _T( "" );
		
			file_name_t = dlg_open.GetPathName();
		
		#ifdef UNICODE
			svs_file_name = CT2A( file_name_t.c_str() );
		#else
			svs_file_name = file_name_t;
		#endif
		}

		if( !svs_file_name.empty() )
		{
			SelectGrabber( GrabberType::SVS_Image_File, svs_file_name );
		}
	}
}

void CVisionBotDlg::OnImageSourceSvsvideo() 
{
	// TODO: Add your command handler code here	
	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_SVSVIDEO, MF_BYCOMMAND );

	if( pThreadProc->isRunning() ) return;

	if( uState == MF_UNCHECKED )
	{
		SelectGrabber( GrabberType::SVS_Video );
	}
}

void CVisionBotDlg::OnImageSourceOpencvgrabber()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_OPENCVCAMERA, MF_BYCOMMAND );

	if( pThreadProc->isRunning() ) return;

	if( uState == MF_UNCHECKED )
	{
		SelectGrabber( GrabberType::OpenCV_Camera );
	}
}

void CVisionBotDlg::OnImageSourceYarpport()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_YARPPORT, MF_BYCOMMAND );

	if( pThreadProc->isRunning() ) return;

	if( uState == MF_UNCHECKED )
	{
		// The dialog takes care of selecting the grabber
		dlgYARPPortConf.ShowWindow( SW_SHOW );
	}
}

void CVisionBotDlg::OnImageSourceCmu1394()
{
	// TODO: Add your command handler code here

	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_CMU1394, MF_BYCOMMAND );

	if( pThreadProc->isRunning() ) return;

	if( uState == MF_UNCHECKED )
	{
		SelectGrabber( GrabberType::CMU1394 );
	}
}

void CVisionBotDlg::OnImageSourcePS3Eye()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_SOURCE_PS3EYE, MF_BYCOMMAND );

	if( pThreadProc->isRunning() ) return;

	if( uState == MF_UNCHECKED )
	{
		SelectGrabber( GrabberType::PS3Eye );
	}
}

void CVisionBotDlg::OnImageSaveToFile() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg_save(
		FALSE,
		CA2T( dlgLoadSaveImageFile.strDefaultExtension.c_str() ),
		NULL,
		dlgLoadSaveImageFile.ulFlags, 
		CA2T( dlgLoadSaveImageFile.strFileFilters.c_str() ),
		this );

	std::string file_name = "";
	
	auto nResult = dlg_save.DoModal();

	if( nResult == IDOK )
	{
		std::tstring file_name_t = _T( "" );
		
		file_name_t = dlg_save.GetPathName();
		
	#ifdef UNICODE
		file_name = CT2A( file_name_t.c_str() );
	#else
		file_name = file_name_t;
	#endif
	}

	if( !file_name.empty() )
	{
		if( pThreadProc->image_procObject->useImageProcessing() )
		{
			pThreadProc->image_procObject->SaveProcessedImageToFile( file_name );
		}
		else
		{
			pThreadProc->image_procObject->SaveSourceImageToFile( file_name );
		}
	}
}

void CVisionBotDlg::OnStereoSaveDisparityImage()
{
	// TODO: Add your command handler code here
#ifdef USE_SVS
	CFileDialog dlg_save(
		FALSE,
		CA2T( dlgLoadSaveImageFile.strDefaultExtension.c_str() ),
		NULL,
		dlgLoadSaveImageFile.ulFlags, 
		CA2T( dlgLoadSaveImageFile.strFileFilters.c_str() ),
		this );

	std::string file_name = "";
	
	auto nResult = dlg_save.DoModal();

	if( nResult == IDOK )
	{
		std::tstring file_name_t = _T( "" );
		
		file_name_t = dlg_save.GetPathName();
		
	#ifdef UNICODE
		file_name = CT2A( file_name_t.c_str() );
	#else
		file_name = file_name_t;
	#endif
	}

	if( !file_name.empty() )
	{
		if( pThreadProc != nullptr )
		if( pThreadProc->pStereoProcess != nullptr )
		if( pThreadProc->pStereoProcess->objStereo != nullptr )
			pThreadProc->pStereoProcess->objStereo->Save_Disparity_Image_to_File( file_name );
	}
#endif
}

void CVisionBotDlg::OnMainTaskConstruction() 
{
	// TODO: Add your command handler code here
	SelectTask( Task::CONSTRUCTION_TOY_STEREO );
}

void CVisionBotDlg::OnMainTaskDrinking() 
{
	// TODO: Add your command handler code here
	SelectTask( Task::DRINKING );
}

void CVisionBotDlg::OnMainTaskTumIntegration() 
{
	// TODO: Add your command handler code here
	SelectTask( Task::TUM_INTEGRATION );
}

void CVisionBotDlg::OnMainTaskConstructionToyRobotFullSVS()
{
	// TODO: Add your command handler code here
	SelectTask( Task::CONSTRUCTION_TOY_FULL_SVS );
}

void CVisionBotDlg::OnMainTaskConstructionToyRobotFullAVT()
{
	// TODO: Add your command handler code here
	SelectTask( Task::CONSTRUCTION_TOY_FULL_AVT );
}

void CVisionBotDlg::OnMainTaskColourLearning()
{
	// TODO: Add your command handler code here
	SelectTask( Task::COLOUR_LEARNING );
}

void CVisionBotDlg::OnMainTaskFaceTracking()
{
	// TODO: Add your command handler code here
	SelectTask( Task::FACE_TRACKING );
}

void CVisionBotDlg::OnMainTaskPlayPiano()
{
	// TODO: Add your command handler code here
	SelectTask( Task::PLAY_PIANO );
}

void CVisionBotDlg::OnMainTaskMobileRobotTargetAcquisition()
{
	// TODO: Add your command handler code here
	SelectTask( Task::MOBILE_ROBOT_TARGET_ACQUISITION );
}

void CVisionBotDlg::OnMainTaskToyRobotLearning()
{
	// TODO: Add your command handler code here
	SelectTask( Task::TOY_ROBOT_LEARNING );
}

void CVisionBotDlg::OnMainTaskHRTransport()
{
	// TODO: Add your command handler code here
	SelectTask( Task::HR_TRANSPORT );
}

void CVisionBotDlg::OnMainTaskDifferentialKinematics()
{
	// TODO: Add your command handler code here
	SelectTask( Task::DIFF_KINEMATICS );
}

void CVisionBotDlg::OnMainTaskComputerassembly()
{
	// TODO: Add your command handler code here
	SelectTask(Task::COMPUTER_ASSEMBLY);
}

void CVisionBotDlg::OnMainTaskTestStuff()
{
	// TODO: Add your command handler code here
	SelectTask( Task::TEST );
}

void CVisionBotDlg::SelectTask( int num_of_task )
{
	std::vector< std::string > 
		vecDir, 
		vecVar;

	// Extra directory to look for
	vecDir.emplace_back( strVisionBot_DIR );

	// Variations/Subdirectories to look for
	vecVar.emplace_back( "/Config/Segmentation/" );
	vecVar.emplace_back( "/Segmentation/" );

	std::string 
		file_name,
		file_path;

	//--------------------------------------------------------------------------

	pData->set_Task( num_of_task );

	SelectMenu_Task( num_of_task );

	//--------------------------------------------------------------------------

	if( pDlgInfo )
	{
		delete pDlgInfo;
		pDlgInfo = nullptr;
	}

	//--------------------------------------------------------------------------

	switch( num_of_task )
	{
	case Task::TEST:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Test).ini";
		break;

		case Task::CONSTRUCTION_TOY_STEREO:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Construction Task).ini";
		break;
		
		case Task::DRINKING:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters_(Drinking_Task).ini";
		break;
		
		case Task::TUM_INTEGRATION:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "AVT/Segmentation_Parameters (TUM).ini";
		break;
		
		case Task::CONSTRUCTION_TOY_FULL_SVS:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Construction Task Full).ini";
		break;
		
		case Task::CONSTRUCTION_TOY_FULL_AVT:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "AVT/Segmentation_Parameters (Construction Task Full).ini";
		break;
		
		case Task::COLOUR_LEARNING:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Colour Learning).ini";
		break;
		
		case Task::FACE_TRACKING:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Colour Learning).ini";
		break;

		case Task::MOBILE_ROBOT_TARGET_ACQUISITION:
			file_name = "Omnidirectional/Segmentation_Parameters.ini";
		break;
		
		case Task::TOY_ROBOT_LEARNING:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Construction Task Full).ini";
		break;

		case Task::PLAY_PIANO:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Play Piano).ini";
		break;

		case Task::HR_TRANSPORT:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (HR Transport Dumbo).ini";
		break;

		case Task::DIFF_KINEMATICS:
			pDlgInfo = new CDialogInfoARoS( this, pData );
			pDlgInfo->ShowWindow( SW_SHOW );
			file_name = "Videre/Segmentation_Parameters (Differential Kinematics).ini";
			break;

		case Task::COMPUTER_ASSEMBLY:
			pDlgInfo = new CDialogInfoARoS(this, pData);
			pDlgInfo->ShowWindow(SW_SHOW);
			file_name = "Videre/Segmentation_Parameters_(ComputerAssembly_Task).ini";
			break;

		default:
			// do noting;
		break;
	}

	if( Find_File( file_name, file_path, vecDir, vecVar ) )
	{
		pThreadProc->image_procObject->LoadSegmentationParameters( file_path );
	}
}

void CVisionBotDlg::OnMainMenuStartHandAcq() 
{
	// TODO: Add your command handler code here
	if( pThreadProc->isRunning() )
		pThreadProc->image_procObject->set_HandAcquisition( true );
	
	main_menu->EnableMenuItem( ID_MAIN_MENU_START_HAND_ACQ, MF_GRAYED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_STOP_HAND_ACQ, MF_ENABLED );
}

void CVisionBotDlg::OnMainMenuStopHandAcq() 
{
	// TODO: Add your command handler code here
	if( pThreadProc->isRunning() )
		pThreadProc->image_procObject->set_HandAcquisition( false );

	main_menu->EnableMenuItem( ID_MAIN_MENU_START_HAND_ACQ, MF_ENABLED );
	main_menu->EnableMenuItem( ID_MAIN_MENU_STOP_HAND_ACQ, MF_GRAYED );
}

void CVisionBotDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	OnOK();
}

void CVisionBotDlg::OnImageFramesaving()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_IMAGE_FRAMESAVING, MF_BYCOMMAND );

	bool bFrameSave = ( uState == MF_UNCHECKED );

	auto state = bFrameSave ? MF_CHECKED : MF_UNCHECKED;

	main_menu->CheckMenuItem( ID_IMAGE_FRAMESAVING, state );

	if( bFrameSave )
		pThreadProc->image_procObject->FrameSaving_On();
	else
		pThreadProc->image_procObject->FrameSaving_Off();
}

void CVisionBotDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: Add your message handler code here

	if( pDlgInfo )
	{
		pDlgInfo->AlignWindow();
	}
}

void CVisionBotDlg::SelectGrabber( int grabber, std::string file_name )
{
	pThreadProc->set_GrabberType( grabber, file_name );
	SelectMenu_ImageSource( grabber );
}

void CVisionBotDlg::SelectMenu_ImageSource( int img_source )
{
	auto state = MF_UNCHECKED;

	size_t num_items = vMenuItem_ImageSource.size();
	for( size_t i=0 ; i<num_items ; i++ )
	{
		state = ( i == img_source ) ? MF_CHECKED : MF_UNCHECKED;

		main_menu->CheckMenuItem( vMenuItem_ImageSource.at( i ), state );
	}
}

void CVisionBotDlg::SelectMenu_Task( int task )
{
	auto state = MF_UNCHECKED;
	
	size_t num_items = vMenuItem_Task.size();
	for( size_t i=0 ; i<num_items ; i++ )
	{
		state = ( i == task ) ? MF_CHECKED : MF_UNCHECKED;

		main_menu->CheckMenuItem( vMenuItem_Task.at( i ), state );
	}
}

void CVisionBotDlg::set_MenuItem_Text( unsigned int item, std::string item_txt )
{
	main_menu->ModifyMenu( item, MF_BYCOMMAND | MF_STRING, item, CA2T( item_txt.c_str() ) );
}

void CVisionBotDlg::OnMenuImageStreamOriginal()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_MENU_IMAGE_STREAM_ORIGINAL, MF_BYCOMMAND );
	bool bStatus = false;

	if( uState == MF_CHECKED )
	{
		pThreadProc->bStream_Image_Original.set( false );
		main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_ORIGINAL, MF_UNCHECKED );
	}
	else
	{
		if( !pThreadProc->imgStream_orig.isRunning() ) 
			bStatus = pThreadProc->imgStream_orig.start();
		else
			bStatus = true;

		pThreadProc->bStream_Image_Original.set( bStatus );
		if( bStatus ) main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_ORIGINAL, MF_CHECKED );
	}
}

void CVisionBotDlg::OnMenuImageStreamProcessed()
{
	// TODO: Add your command handler code here
	auto uState = main_menu->GetMenuState( ID_MENU_IMAGE_STREAM_PROCESSED, MF_BYCOMMAND );

	bool bStatus = false;

	if( uState == MF_CHECKED )
	{
		pThreadProc->bStream_Image_Processed.set( false );
		main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_PROCESSED, MF_UNCHECKED );
	}
	else
	{
		if( !pThreadProc->imgStream_proc.isRunning() ) 
			bStatus = pThreadProc->imgStream_proc.start();
		else
			bStatus = true;

		pThreadProc->bStream_Image_Processed.set( bStatus );
		if( bStatus ) main_menu->CheckMenuItem( ID_MENU_IMAGE_STREAM_PROCESSED, MF_CHECKED );
	}
}



