#include "VisionBot.h"
#include "VisionBotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVisionBotApp

BEGIN_MESSAGE_MAP(CVisionBotApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVisionBotApp construction

CVisionBotApp::CVisionBotApp()
{
	// support Restart Manager
#if WINVER>=0x0600
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#endif

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVisionBotApp object

CVisionBotApp theApp;


// CVisionBotApp initialization

BOOL CVisionBotApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
#if WINVER>=0x0600
	CShellManager *pShellManager = new CShellManager;
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey( _T( "UM:DEI - Mobile and Anthropomorphic Robotics Laboratory" ) );

	CVisionBotDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if( nResponse == IDOK )
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if( nResponse == IDCANCEL )
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
#if WINVER>=0x0600
	if( pShellManager != nullptr )
	{
		delete pShellManager;
	}
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
