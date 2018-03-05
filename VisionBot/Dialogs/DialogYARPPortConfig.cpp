#include "VisionBotDlg.h"
#include "DialogYARPPortConfig.h"

#include "DialogUtils.h"
#include "GrabberDefinitions.h"
#include "GrabberYARPPort.h"

IMPLEMENT_DYNAMIC(CDialogYARPPortConfig, CDialog)

CDialogYARPPortConfig::CDialogYARPPortConfig( CWnd* pParent, bool config_remote )
	: CDialog(CDialogYARPPortConfig::IDD, pParent)
	, bConfigRemote( config_remote )
	, strPortName_Local( "/aros/vision/grabber" )
	, strPortName_Remote( "/grabber" )
{
	Create( IDD_DIALOG_YARPPORT_CONFIGURATION, pParent );

	parent = pParent;

	m_Yarp_Port_Local.SetWindowText( CA2T( strPortName_Local.c_str() ) );
	m_Yarp_Port_Remote.SetWindowText( CA2T( strPortName_Remote.c_str() ) );

	if( !config_remote )
	{
		m_Yarp_Port_Remote.ShowWindow( SW_HIDE );
		m_static_remote.ShowWindow( SW_HIDE );
	}
}

void CDialogYARPPortConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_YARPPORT_LOCAL, m_Yarp_Port_Local);
	DDX_Control(pDX, IDC_EDIT_YARPPORT_REMOTE, m_Yarp_Port_Remote);
	DDX_Control(pDX, IDC_STATIC_REMOTE, m_static_remote);
}


BEGIN_MESSAGE_MAP(CDialogYARPPortConfig, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogYARPPortConfig message handlers

void CDialogYARPPortConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	std::tstring strText;

	if( ReadTextFromEditBox( &m_Yarp_Port_Local, strText ) )
		strPortName_Local = CT2A( strText.c_str() );

	if( bConfigRemote )
	{
		if( ReadTextFromEditBox( &m_Yarp_Port_Remote, strText ) )
			strPortName_Remote = CT2A( strText.c_str() );
	}

	if( parent != nullptr )
	{
		CVisionBotDlg * aros_dlg = (CVisionBotDlg*) parent;

		aros_dlg->SelectGrabber( GrabberType::YARP_Port );

		CGrabberYARPPort * grabber = static_cast<CGrabberYARPPort*>( aros_dlg->pThreadProc->grabberObject );

		grabber->set_PortNameLocal( strPortName_Local );
		grabber->set_PortNameRemote( strPortName_Remote );
	}

	OnOK();
}

void CDialogYARPPortConfig::set_PortName_Local( std::string port )
{
	strPortName_Local = port;
	m_Yarp_Port_Local.SetWindowText( CA2T( strPortName_Local.c_str() ) );
}

void CDialogYARPPortConfig::set_PortName_Remote( std::string port )
{ 
	strPortName_Remote = port;
	m_Yarp_Port_Remote.SetWindowText( CA2T( strPortName_Remote.c_str() ) );
}

std::string CDialogYARPPortConfig::get_PortName_Local( void )
{
	return strPortName_Local;
}

std::string CDialogYARPPortConfig::get_PortName_Remote( void )
{
	return strPortName_Remote;
}
