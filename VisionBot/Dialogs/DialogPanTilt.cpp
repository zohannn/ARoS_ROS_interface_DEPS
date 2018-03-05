#include "VisionBotDlg.h"
#include "DialogPanTilt.h"

#include <sstream>

#include "DialogUtils.h"
#include "pantilt_definitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PANTILT_REFRESH_TIMER 0

CDialogPanTilt::CDialogPanTilt(CWnd* pParent /*=nullptr*/)
: CDialog(CDialogPanTilt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPanTilt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Create( IDD_DIALOG_PANTILT, pParent );

	parent = pParent;

	CenterWindow();

	m_radio_control_position.SetCheck( MF_CHECKED );
}

void CDialogPanTilt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPanTilt)
	DDX_Control(pDX, IDC_STATIC_CURRENT_TILT, m_static_tilt);
	DDX_Control(pDX, IDC_STATIC_CURRENT_PAN, m_static_pan);
	DDX_Control(pDX, IDC_EDIT_TILT, m_edit_tilt);
	DDX_Control(pDX, IDC_EDIT_PAN, m_edit_pan);
	DDX_Control(pDX, IDC_EDIT_X_POS, m_edit_pos_x);
	DDX_Control(pDX, IDC_EDIT_Y_POS, m_edit_pos_y);
	DDX_Control(pDX, IDC_EDIT_Z_POS, m_edit_pos_z);
	DDX_Control(pDX, IDC_STATIC_UNITS_PAN, m_static_units_pan);
	DDX_Control(pDX, IDC_STATIC_UNITS_TILT, m_static_units_tilt);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_CONTROL_MODE_POSITION, m_radio_control_position);
}

BEGIN_MESSAGE_MAP(CDialogPanTilt, CDialog)
	//{{AFX_MSG_MAP(CDialogPanTilt)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SET_PAN, OnButtonSetPan)
	ON_BN_CLICKED(IDC_BUTTON_GET_PAN, OnButtonGetPan)
	ON_BN_CLICKED(IDC_BUTTON_SET_TILT, OnButtonSetTilt)
	ON_BN_CLICKED(IDC_BUTTON_GET_TILT, OnButtonGetTilt)
	ON_BN_CLICKED(IDC_BUTTON_SET_PAN_TILT, OnButtonSetPanTilt)
	ON_BN_CLICKED(IDC_BUTTON_GET_PAN_TILT, OnButtonGetPanTilt)
	ON_BN_CLICKED(IDC_BUTTON_STOP_PAN, OnBnClickedButtonStopPan)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TILT, OnBnClickedButtonStopTilt)
	ON_BN_CLICKED(IDC_BUTTON_STOP_PANTILT, OnBnClickedButtonStopPantilt)
	ON_BN_CLICKED(IDC_BUTTON_INIT_PAN, OnBnClickedButtonInitPan)
	ON_BN_CLICKED(IDC_BUTTON_INIT_TILT, OnBnClickedButtonInitTilt)
	ON_BN_CLICKED(IDC_BUTTON_INIT_PANTILT, OnBnClickedButtonInitPantilt)
	ON_BN_CLICKED(IDC_BUTTON_LOOK_TO_POSITION, OnBnClickedButtonLookToPosition)
	ON_BN_CLICKED(IDC_RADIO_CONTROL_MODE_POSITION, OnBnClickedRadioControlModePosition)
	ON_BN_CLICKED(IDC_RADIO_CONTROL_MODE_VELOCITY, OnBnClickedRadioControlModeVelocity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

inline int CDialogPanTilt::get_ControlMode( void )
{
	return ( (CVisionBotDlg*) parent )->pData->PanTilt.nControlMode.get();
}

void CDialogPanTilt::RefreshPan( void )
{
	switch( get_ControlMode() )
	{
	case PanTilt::ControlMode::POSITION:
		RefreshPanPosition();
		break;

	case PanTilt::ControlMode::VELOCITY:
		RefreshPanVelocity();
		break;
	}
}

void CDialogPanTilt::RefreshTilt( void )
{
	switch( get_ControlMode() )
	{
	case PanTilt::ControlMode::POSITION:
		RefreshTiltPosition();
		break;

	case PanTilt::ControlMode::VELOCITY:
		RefreshTiltVelocity();
		break;
	}
}

void CDialogPanTilt::RefreshPanPosition( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;
	vision_bot->pData->PanTilt.Update_Pan_Angle();
	ShowFormatedTextInStatic( &m_static_pan, _T( "Angle: " ), vision_bot->pData->PanTilt.fPan_Angle.get(), _T( " º" ) );
}

void CDialogPanTilt::RefreshTiltPosition( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;
	vision_bot->pData->PanTilt.Update_Tilt_Angle();
	ShowFormatedTextInStatic( &m_static_tilt, _T( "Angle: " ), vision_bot->pData->PanTilt.fTilt_Angle.get(), _T( " º" ) );
}

void CDialogPanTilt::RefreshPanVelocity( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;
	vision_bot->pData->PanTilt.Update_Pan_Velocity();
	ShowFormatedTextInStatic( &m_static_pan, _T( "Velocity: " ), vision_bot->pData->PanTilt.fPan_Velocity.get(), _T( " º/s" ) );
}

void CDialogPanTilt::RefreshTiltVelocity( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;
	vision_bot->pData->PanTilt.Update_Tilt_Velocity();
	ShowFormatedTextInStatic( &m_static_tilt, _T( "Velocity: " ), vision_bot->pData->PanTilt.fTilt_Velocity.get(), _T( " º/s" ) );
}

bool CDialogPanTilt::SendCommand( unsigned int command, std::vector<float> vData )
{
#ifdef USE_YarpCommunication
	if( parent == nullptr ) return false;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	if( !vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened() ) return false;

	CMessage msgCommand( command );

	if( vData.size() > 0 )
		msgCommand.fData.assign( vData.begin(), vData.end() );

	return vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->Send( msgCommand );
#else
	return false;
#endif
}

void CDialogPanTilt::OnButtonSetPan() 
{
	// TODO: Add your control notification handler code here
	if( parent == nullptr ) return;

	std::vector<float> vPan( 1, 0.0f );

	if( !ReadValueFromEditBox( &m_edit_pan, &vPan.front() ) ) return;
	
	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	bool isOpened = false;

#ifdef USE_YarpCommunication
	isOpened = vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened();
#endif
	
	if( isOpened )
	{
#ifdef USE_YarpCommunication
		switch( get_ControlMode() )
		{
		case PanTilt::ControlMode::POSITION:
			SendCommand( PANTILT_MOVE_ANGLE_ABS_PAN, vPan );
			break;

		case PanTilt::ControlMode::VELOCITY:
			SendCommand( PANTILT_MOVE_VELOCITY_ABS_PAN, vPan );
			break;
		}
#endif
	}
	else
	{
		// Simulate write pan
		vision_bot->pData->PanTilt.fPan_Angle.set( vPan.front() );
		vision_bot->pData->PanTilt.fPan_Angle_temp.set( vPan.front() );
	}

	RefreshPan();
}

void CDialogPanTilt::OnButtonSetTilt() 
{
	// TODO: Add your control notification handler code here
	std::vector<float> vTilt( 1, 0.0f );
	
	if( !ReadValueFromEditBox( &m_edit_tilt, &vTilt.front() ) ) return;

	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	bool isOpened = false;

#ifdef USE_YarpCommunication
	isOpened = vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened();
#endif

	if( isOpened )
	{
#ifdef USE_YarpCommunication
		switch( get_ControlMode() )
		{
		case PanTilt::ControlMode::POSITION:
			SendCommand( PANTILT_MOVE_ANGLE_ABS_TILT, vTilt );
			break;

		case PanTilt::ControlMode::VELOCITY:
			SendCommand( PANTILT_MOVE_VELOCITY_ABS_TILT, vTilt );
			break;
		}
#endif
	}
	else
	{
		// Simulate write tilt
		vision_bot->pData->PanTilt.fTilt_Angle.set( vTilt.front() );
		vision_bot->pData->PanTilt.fTilt_Angle_temp.set( vTilt.front() );
	}

	RefreshTilt();
}

void CDialogPanTilt::OnButtonGetPan() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	switch( get_ControlMode() )
	{
	case PanTilt::ControlMode::POSITION:
		SendCommand( PANTILT_GET_ANGLE_PAN );
		break;

	case PanTilt::ControlMode::VELOCITY:
		SendCommand( PANTILT_GET_REF_SPEED_PAN );
		break;
	}
#endif

	RefreshPan();
}

void CDialogPanTilt::OnButtonGetTilt() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication

	switch( get_ControlMode() )
	{
	case PanTilt::ControlMode::POSITION:
		SendCommand( PANTILT_GET_ANGLE_TILT );
		break;

	case PanTilt::ControlMode::VELOCITY:
		SendCommand( PANTILT_GET_REF_SPEED_TILT );
		break;
	}
#endif

	RefreshTilt();
}

void CDialogPanTilt::OnButtonSetPanTilt() 
{
	// TODO: Add your control notification handler code here
	std::vector<float> vData( 2, 0.0f );

	if( !ReadValueFromEditBox( &m_edit_pan, &vData.at( PanTilt::Axis::ID::PAN ) ) ||
		!ReadValueFromEditBox( &m_edit_tilt, &vData.at( PanTilt::Axis::ID::TILT ) ) ) return;

	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	bool isOpened = false;

#ifdef USE_YarpCommunication
	isOpened = vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened();
#endif

	if( isOpened )
	{
#ifdef USE_YarpCommunication
		switch( get_ControlMode() )
		{
		case PanTilt::ControlMode::POSITION:
			SendCommand( PANTILT_MOVE_ANGLE_ABS_PANTILT, vData );
			break;

		case PanTilt::ControlMode::VELOCITY:
			SendCommand( PANTILT_MOVE_VELOCITY_ABS_PANTILT, vData );
			break;
		}
#endif
	}
	else
	{
		// Simulate write pan and tilt
		vision_bot->pData->PanTilt.set_PanTiltAngles( vData.at( PanTilt::Axis::ID::PAN ), vData.at( PanTilt::Axis::ID::TILT ) );
		vision_bot->pData->PanTilt.set_PanTiltAngles_temp( vData.at( PanTilt::Axis::ID::PAN ), vData.at( PanTilt::Axis::ID::TILT ) );
	}

	RefreshPan();
	RefreshTilt();
}

void CDialogPanTilt::OnButtonGetPanTilt()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	switch( get_ControlMode() )
	{
	case PanTilt::ControlMode::POSITION:
		SendCommand( PANTILT_GET_ANGLE_PANTILT );
		break;

	case PanTilt::ControlMode::VELOCITY:
		SendCommand( PANTILT_GET_REF_SPEED_PANTILT );
		break;
	}
#endif

	RefreshPan();
	RefreshTilt();
}

void CDialogPanTilt::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CDialogPanTilt::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CDialogPanTilt::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RefreshPan();
	RefreshTilt();

	CDialog::OnTimer(nIDEvent);
}

void CDialogPanTilt::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
#ifdef USE_YarpCommunication
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	if( vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt ) == nullptr ) return;

	if( !vision_bot->pThreadComm.at( Comm::ID_Pan_Tilt )->isOpened() ) return;
#endif

	if( bShow == TRUE )
		SetTimer( PANTILT_REFRESH_TIMER, 500, NULL );

	else
		KillTimer( PANTILT_REFRESH_TIMER );
}

void CDialogPanTilt::OnBnClickedButtonStopPan()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_STOP_PAN );
#endif
}

void CDialogPanTilt::OnBnClickedButtonStopTilt()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_STOP_TILT );
#endif
}

void CDialogPanTilt::OnBnClickedButtonStopPantilt()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_STOP_PANTILT );
#endif
}

void CDialogPanTilt::OnBnClickedButtonInitPan()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_RESET_PAN );
#endif
}

void CDialogPanTilt::OnBnClickedButtonInitTilt()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_RESET_TILT );
#endif
}

void CDialogPanTilt::OnBnClickedButtonInitPantilt()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_RESET_PANTILT );
#endif
}

void CDialogPanTilt::OnBnClickedButtonLookToPosition()
{
	// TODO: Add your control notification handler code here
	std::vector<float> vData( 3, 0.0f );

	if( !ReadValueFromEditBox( &m_edit_pos_x, &vData.at( 0 ) ) || 
		!ReadValueFromEditBox( &m_edit_pos_y, &vData.at( 1 ) ) || 
		!ReadValueFromEditBox( &m_edit_pos_z, &vData.at( 2 ) ) ) return;

#ifdef USE_YarpCommunication
	SendCommand( PANTILT_AROS_LOOK_AT_POSITION, vData );
#endif

	RefreshPan();
	RefreshTilt();
}

void CDialogPanTilt::OnBnClickedRadioControlModePosition()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_USE_POSITION_CONTROL );
#endif

	if( get_ControlMode() == PanTilt::ControlMode::POSITION )
	{
		m_static_units_pan.SetWindowText( _T( "º" ) );
		m_static_units_tilt.SetWindowText( _T( "º" ) );
	}
}

void CDialogPanTilt::OnBnClickedRadioControlModeVelocity()
{
	// TODO: Add your control notification handler code here
#ifdef USE_YarpCommunication
	SendCommand( PANTILT_USE_VELOCITY_CONTROL );
#endif

	if( get_ControlMode() == PanTilt::ControlMode::VELOCITY )
	{
		m_static_units_pan.SetWindowText( _T( "º/s" ) );
		m_static_units_tilt.SetWindowText( _T( "º/s" ) );
	}
}
