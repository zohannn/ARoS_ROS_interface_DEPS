#include "VisionBotDlg.h"
#include "ansi_unicode_definitions.h"
#include "DialogConnection.h"
#include "DialogUtils.h"

#ifdef USE_YarpCommunication
#include "YarpCommunication.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogConnection::CDialogConnection(CWnd* pParent /*=nullptr*/)
	: CDialog(CDialogConnection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConnection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Create( IDD_DIALOG_CONNECTION, pParent );

	parent = pParent;

	CenterWindow();
}

void CDialogConnection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConnection)
	DDX_Control(pDX, IDC_EDIT_PEER_NAME, m_edit_Peer_Name);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edit_Name);
	DDX_Control(pDX, IDC_LIST_COMMS, m_List_Comms);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_static_Status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogConnection, CDialog)
	//{{AFX_MSG_MAP(CDialogConnection)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTION_START_ALL, OnBnClickedButtonConnectionStartAll)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTION_STOP_ALL, OnBnClickedButtonConnectionStopAll)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTION_START_STOP, OnBnClickedButtonConnectionStartStop)
	ON_LBN_SELCHANGE(IDC_LIST_COMMS, OnLbnSelchangeListComms)
END_MESSAGE_MAP()

void CDialogConnection::Configure_Dialog( void )
{
	PopulateCommsList();
}

void CDialogConnection::PopulateCommsList( void )
{
#ifdef USE_YarpCommunication
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	m_List_Comms.ResetContent();

	size_t comm_size = vision_bot->pThreadComm.size();

	for( size_t i=0 ; i<comm_size ; i++ )
	{
		m_List_Comms.InsertString( static_cast<int>( i ), CA2T( Comm::ID[i].c_str() ) );
	}

	if( comm_size > 0 )
	{
		m_List_Comms.SetCurSel( 0 );
		RefreshGUI();
	}
#endif
}

void CDialogConnection::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	OnOK();
}

void CDialogConnection::OnBnClickedButtonConnectionStartAll()
{
	// TODO: Add your control notification handler code here

#ifdef USE_YarpCommunication
	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	size_t comm_size = vision_bot->pThreadComm.size();
	
	for( size_t i=0 ; i<comm_size ; i++ )
	{
		if( !vision_bot->pThreadComm.at( i )->isOpened() )
		{
			if( !vision_bot->pThreadComm.at( i )->open() )
			{
				std::tstring strError = _T( "Failed to create connection:\n" );
				strError += CA2T( vision_bot->pThreadComm.at( i )->get_PortName().c_str() );
				if( vision_bot->pThreadComm.at( i )->get_PortNameServer().size() > 0 )
				{
					strError += _T( " --> " );
					strError += CA2T( vision_bot->pThreadComm.at( i )->get_PortNameServer().c_str() );
				}
				else
				{
					strError += _T( " server" );
				}

				AfxMessageBox( strError.c_str(), MB_ICONERROR );
			}
		}
	}
#endif

	RefreshGUI();
}

void CDialogConnection::OnBnClickedButtonConnectionStopAll()
{
	// TODO: Add your control notification handler code here

#ifdef USE_YarpCommunication
	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	size_t comm_size = vision_bot->pThreadComm.size();

	for( size_t i=0 ; i<comm_size ; i++ )
	{
		if( vision_bot->pThreadComm.at( i )->isOpened() )
			vision_bot->pThreadComm.at( i )->close();
	}
#endif

	RefreshGUI();
}

void CDialogConnection::OnBnClickedButtonConnectionStartStop()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	
	int sel = m_List_Comms.GetCurSel();
	
	if( sel<0 ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	int nComs = static_cast<int>( vision_bot->pThreadComm.size() );
	
	if( ( nComs <= 0 ) || ( sel >= nComs ) ) return;

	// If the connection is opened, close it.
	if( vision_bot->pThreadComm.at( sel )->isOpened() )
	{
		vision_bot->pThreadComm.at( sel )->close();
		m_static_Status.SetWindowText( _T( "Status: Stopped" ) );
	}
	else
	{
		//-------------------------------------------------------------------
		// Attempt to set port name

		std::tstring str_name;
		std::string str_name_a = "";

		if( ReadTextFromEditBox( &m_edit_Name, str_name ) )
		{
			str_name_a = std::string( CT2A( str_name.c_str() ) );

			if( vision_bot->pThreadComm.at( sel )->set_PortName( str_name_a ) )
				m_edit_Name.SetWindowText( CA2T( vision_bot->pThreadComm.at( sel )->get_PortName().c_str() ) );
		}

		//-------------------------------------------------------------------
		// Attempt to set peer port name

		if( ReadTextFromEditBox( &m_edit_Peer_Name, str_name ) )
		{
			str_name_a = std::string( CT2A( str_name.c_str() ) );

			if( vision_bot->pThreadComm.at( sel )->set_PortNameServer( str_name_a ) )
				m_edit_Name.SetWindowText( CA2T( vision_bot->pThreadComm.at( sel )->get_PortNameServer().c_str() ) );
		}
		
		//-------------------------------------------------------------------
		// Try to open the connection.
		if( vision_bot->pThreadComm.at( sel )->open() )
		{
			m_static_Status.SetWindowText( _T( "Status: Running..." ) );
		}
		else
		{
			std::string strCommError = "Failed to open connection.\n";
			strCommError += vision_bot->pThreadComm.at( sel )->get_ErrorString();
			AfxMessageBox( CA2T( strCommError.c_str() ), MB_ICONERROR );
		}
	}
#endif
}

void CDialogConnection::RefreshGUI( void )
{
#ifdef USE_YarpCommunication

	int sel = m_List_Comms.GetCurSel();
	
	if( sel<0 ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	int nComs = static_cast<int>( vision_bot->pThreadComm.size() );

	if( ( nComs <= 0 ) || ( sel >= nComs ) ) return;

	m_edit_Name.SetWindowText( CA2T( vision_bot->pThreadComm.at( sel )->get_PortName().c_str() ) );
	m_edit_Peer_Name.SetWindowText( CA2T( vision_bot->pThreadComm.at( sel )->get_PortNameServer().c_str() ) );
	
	m_static_Status.SetWindowText(
			vision_bot->pThreadComm.at( sel )->isOpened() ? 
			_T( "Status: Running..." ) : 
			_T( "Status: Stopped" )
		);
#endif
}

void CDialogConnection::OnLbnSelchangeListComms()
{
	// TODO: Add your control notification handler code here
	RefreshGUI();
}
