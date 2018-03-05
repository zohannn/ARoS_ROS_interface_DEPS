#include "VisionBotDlg.h"
#include "DialogVideoParameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogVideoParameters::CDialogVideoParameters(CWnd* pParent /*=nullptr*/)
: CDialog(CDialogVideoParameters::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogVideoParameters)
	//}}AFX_DATA_INIT

	Create(IDD_DIALOG_VIDEO_PARAMETERS, pParent);

	parent = pParent;

	m_spin_exposure.SetRange32( 0, 100 );
	m_spin_gain.SetRange32( 0, 100 );
	m_spin_brightness.SetRange32( 0, 100 );
	m_spin_gamma.SetRange32( 0, 100 );
	m_spin_red.SetRange32( 0, 100 );
	m_spin_green.SetRange32( 0, 100 );
	m_spin_blue.SetRange32( 0, 100 );
	m_spin_saturation.SetRange32( 0, 100 );
}

void CDialogVideoParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogVideoParameters)
	DDX_Control(pDX, IDC_EDIT_SATURATION, m_edit_saturation);
	DDX_Control(pDX, IDC_EDIT_RED, m_edit_red);
	DDX_Control(pDX, IDC_EDIT_GAMMA, m_edit_gamma);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_edit_gain);
	DDX_Control(pDX, IDC_EDIT_EXPOSURE, m_edit_exposure);
	DDX_Control(pDX, IDC_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_EDIT_BLUE, m_edit_blue);
	DDX_Control(pDX, IDC_SPIN_SATURATION, m_spin_saturation);
	DDX_Control(pDX, IDC_CHECK_BALANCE, m_check_balance);
	DDX_Control(pDX, IDC_SPIN_RED, m_spin_red);
	DDX_Control(pDX, IDC_SPIN_GAMMA, m_spin_gamma);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_spin_gain);
	DDX_Control(pDX, IDC_SPIN_EXPOSURE, m_spin_exposure);
	DDX_Control(pDX, IDC_SPIN_BRIGHTNESS, m_spin_brightness);
	DDX_Control(pDX, IDC_SPIN_BLUE, m_spin_blue);
	DDX_Control(pDX, IDC_CHECK_GAIN, m_check_gain);
	DDX_Control(pDX, IDC_CHECK_EXPOSURE, m_check_exposure);
	DDX_Control(pDX, IDC_CHECK_BRIGHTNESS, m_check_brightness);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_GREEN, m_edit_green);
	DDX_Control(pDX, IDC_SPIN_GREEN, m_spin_green);
}


BEGIN_MESSAGE_MAP(CDialogVideoParameters, CDialog)
	//{{AFX_MSG_MAP(CDialogVideoParameters)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE, OnChangeEditExposure)
	ON_EN_CHANGE(IDC_EDIT_BLUE, OnChangeEditBlue)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, OnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_GAIN, OnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_GAMMA, OnChangeEditGamma)
	ON_EN_CHANGE(IDC_EDIT_RED, OnChangeEditRed)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_VIDEO_PAR, OnButtonLoadVideoPar)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_VIDEO_PAR, OnButtonSaveVideoPar)
	ON_EN_CHANGE(IDC_EDIT_SATURATION, OnChangeEditSaturation)
	ON_BN_CLICKED(IDC_CHECK_EXPOSURE, OnCheckExposure)
	ON_BN_CLICKED(IDC_CHECK_GAIN, OnCheckGain)
	ON_BN_CLICKED(IDC_CHECK_BRIGHTNESS, OnCheckBrightness)
	ON_BN_CLICKED(IDC_CHECK_BALANCE, OnCheckBalance)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_GREEN, &CDialogVideoParameters::OnEnChangeEditGreen)
END_MESSAGE_MAP()

void CDialogVideoParameters::Configure_Dialog( void )
{
	LoadParametersFromGrabber();
}

void CDialogVideoParameters::Enable_Control_Exposure( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_exposure.EnableWindow( state );
	m_spin_exposure.EnableWindow( state );
	m_check_exposure.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_Gain( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_gain.EnableWindow( state );
	m_spin_gain.EnableWindow( state );
	m_check_gain.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_Brightness( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_brightness.EnableWindow( state );
	m_spin_brightness.EnableWindow( state );
	m_check_brightness.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_Saturation( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_saturation.EnableWindow( state );
	m_spin_saturation.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_Gamma( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_gamma.EnableWindow( state );
	m_spin_gamma.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_WB_Red( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_red.EnableWindow( state );
	m_spin_red.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_WB_Green( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_green.EnableWindow( state );
	m_spin_green.EnableWindow( state );
}

void CDialogVideoParameters::Enable_Control_WB_Blue( bool enable )
{
	int state = enable ? 1 : 0;

	m_edit_blue.EnableWindow( state );
	m_spin_blue.EnableWindow( state );
}

void CDialogVideoParameters::UpdateExposure()
{
	if( m_spin_exposure.m_hWnd == nullptr ) return;

	bool auto_exp = ( m_check_exposure.GetCheck() == 1 );
	int exp = m_spin_exposure.GetPos();
	
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_Exposure( exp );
	vision_bot->pThreadProc->grabberObject->set_Exposure_Auto( auto_exp );
}

void CDialogVideoParameters::UpdateGain()
{
	if( m_spin_gain.m_hWnd == nullptr ) return;

	bool auto_gain = ( m_check_gain.GetCheck() == 1 );
	int gain = m_spin_gain.GetPos();
	
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_Gain( gain );
	vision_bot->pThreadProc->grabberObject->set_Gain_Auto( auto_gain );
}

void CDialogVideoParameters::UpdateBrightness()
{
	if( m_spin_brightness.m_hWnd == nullptr ) return;

	bool auto_brightness = ( m_check_brightness.GetCheck() == 1 );
	int brightness = m_spin_brightness.GetPos();
	
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_Brightness( brightness );
	vision_bot->pThreadProc->grabberObject->set_Brightness_Auto( auto_brightness );
}

void CDialogVideoParameters::UpdateGamma()
{
	if( m_spin_gamma.m_hWnd == nullptr ) return;

	int gamma = m_spin_gamma.GetPos();

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_Gamma( gamma );
}

void CDialogVideoParameters::UpdateWhiteBalance()
{
	if( ( m_spin_red.m_hWnd == nullptr ) || 
		( m_spin_green.m_hWnd == nullptr ) || 
		( m_spin_blue.m_hWnd == nullptr ) ) 
		return;

	bool auto_balance = ( m_check_balance.GetCheck() == 1 );
	
	int 
		red = m_spin_red.GetPos(),
		green = m_spin_green.GetPos(),
		blue = m_spin_blue.GetPos();

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_WhiteBalance_Red( red );
	vision_bot->pThreadProc->grabberObject->set_WhiteBalance_Green( green );
	vision_bot->pThreadProc->grabberObject->set_WhiteBalance_Blue( blue );
	vision_bot->pThreadProc->grabberObject->set_WhiteBalance_Auto( auto_balance );
}

void CDialogVideoParameters::UpdateSaturation()
{
	if( m_spin_saturation.m_hWnd == nullptr ) return;

	int saturation = m_spin_saturation.GetPos();

	CVisionBotDlg * vision_bot = (CVisionBotDlg*) parent;

	vision_bot->pThreadProc->grabberObject->set_Saturation( saturation );
}

void CDialogVideoParameters::LoadParametersFromGrabber( void )
{
	CGrabber * grabber = ( (CVisionBotDlg*) parent )->pThreadProc->grabberObject;

	if( grabber == nullptr ) return;
	
	int nAuto = grabber->get_Brightness_Auto() ? 1 : 0;
	m_check_brightness.SetCheck( nAuto );

	m_spin_brightness.SetPos( grabber->get_Brightness() );

	nAuto = grabber->get_Exposure_Auto() ? 1 : 0;
	m_check_exposure.SetCheck( nAuto );

	m_spin_exposure.SetPos( grabber->get_Exposure() );

	nAuto = grabber->get_Gain_Auto() ? 1 : 0;
	m_check_gain.SetCheck( nAuto );

	m_spin_gain.SetPos( grabber->get_Gain() );

	nAuto = grabber->get_WhiteBalance_Auto() ? 1 : 0;
	m_check_balance.SetCheck( nAuto );

	m_spin_blue.SetPos( grabber->get_WhiteBalance_Blue() );

	m_spin_green.SetPos( grabber->get_WhiteBalance_Green() );

	m_spin_red.SetPos( grabber->get_WhiteBalance_Red() );

	m_spin_saturation.SetPos( grabber->get_Saturation() );

	m_spin_gamma.SetPos( grabber->get_Gamma() );
}

void CDialogVideoParameters::OnChangeEditExposure() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateExposure();
}

void CDialogVideoParameters::OnChangeEditBrightness() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateBrightness();
}

void CDialogVideoParameters::OnChangeEditGain() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateGain();
}

void CDialogVideoParameters::OnChangeEditGamma() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateGamma();
}

void CDialogVideoParameters::OnChangeEditRed() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateWhiteBalance();
}

void CDialogVideoParameters::OnEnChangeEditGreen()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateWhiteBalance();
}

void CDialogVideoParameters::OnChangeEditBlue() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateWhiteBalance();
}

void CDialogVideoParameters::OnChangeEditSaturation() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateSaturation();
}

void CDialogVideoParameters::OnButtonLoadVideoPar() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg_open(
		TRUE,
		CA2T( strDefaultExtension.c_str() ),
		NULL,
		ulFlags, 
		CA2T( strFileFilters.c_str() ),
		this );

	std::string file_name = "";

	auto nResult = dlg_open.DoModal();

	if( nResult == IDOK )
	{
		std::tstring file_name_t = _T( "" );
		
		file_name_t = dlg_open.GetPathName();
		
	#ifdef UNICODE
		file_name = CT2A( file_name_t.c_str() );
	#else
		file_name = file_name_t;
	#endif
	}

	if( !file_name.empty() )
	{
		CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;
		vision_bot->pThreadProc->grabberObject->Load_parameters_from_file( file_name );
	}
}

void CDialogVideoParameters::OnButtonSaveVideoPar() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg_save(
		FALSE,
		CA2T( strDefaultExtension.c_str() ),
		NULL,
		ulFlags, 
		CA2T( strFileFilters.c_str() ),
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
		CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;
		vision_bot->pThreadProc->grabberObject->Save_parameters_to_file( file_name );
	}
}

void CDialogVideoParameters::OnCheckExposure() 
{
	// TODO: Add your control notification handler code here
	int state = static_cast<int>( m_check_exposure.GetCheck() == BST_UNCHECKED );
	
	m_edit_exposure.EnableWindow( state );
	m_spin_exposure.EnableWindow( state );

	UpdateExposure();
}

void CDialogVideoParameters::OnCheckGain() 
{
	// TODO: Add your control notification handler code here
	int state = static_cast<int>( m_check_gain.GetCheck() == BST_UNCHECKED );

	m_edit_gain.EnableWindow( state );
	m_spin_gain.EnableWindow( state );

	UpdateGain();
}

void CDialogVideoParameters::OnCheckBrightness() 
{
	// TODO: Add your control notification handler code here
	int state = static_cast<int>( m_check_brightness.GetCheck() == BST_UNCHECKED );

	m_edit_brightness.EnableWindow( state );
	m_spin_brightness.EnableWindow( state );

	UpdateBrightness();
}

void CDialogVideoParameters::OnCheckBalance() 
{
	// TODO: Add your control notification handler code here
	int state = static_cast<int>( m_check_balance.GetCheck() == BST_UNCHECKED );

	m_edit_red.EnableWindow( state );
	m_spin_red.EnableWindow( state );
	m_edit_green.EnableWindow( state );
	m_spin_green.EnableWindow( state );
	m_edit_blue.EnableWindow( state );
	m_spin_blue.EnableWindow( state );

	UpdateWhiteBalance();
}

void CDialogVideoParameters::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if( bShow == TRUE )	LoadParametersFromGrabber();
}


