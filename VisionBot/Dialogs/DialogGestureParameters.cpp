// C:\Work\MARL\VisionBot\Dialogs\DialogGestureParameters.cpp : implementation file
//

#include <afxdialogex.h>

#include "VisionBotDlg.h"
#include "DialogGestureParameters.h"
#include "ansi_unicode_definitions.h"
#include "DialogFileOpenSave_types.h"
#include "DialogUtils.h"

#include "MutexYarp.h"

// CDialogGestureParameters dialog

IMPLEMENT_DYNAMIC(CDialogGestureParameters, CDialog)

CDialogGestureParameters::CDialogGestureParameters(CWnd* pParent /*=NULL*/)
: CDialog(CDialogGestureParameters::IDD, pParent)
, pGestureClassifier( nullptr )
{
	int aSize = sizeof( pEdit_Power ) / sizeof( CEdit* );
	for( int i = 0; i < aSize; i++ )
		pEdit_Power[i] = nullptr;

	aSize = sizeof( pEdit_AG ) / sizeof( CEdit* );
	for( int i = 0; i < aSize; i++ )
		pEdit_AG[i] = nullptr;

	aSize = sizeof( pEdit_SG ) / sizeof( CEdit* );
	for( int i = 0; i < aSize; i++ )
		pEdit_SG[i] = nullptr;

	aSize = sizeof( pEdit_PT ) / sizeof( CEdit* );
	for( int i = 0; i < aSize; i++ )
		pEdit_PT[i] = nullptr;

	Create( IDD_DIALOG_GESTURE_PARAMETERS, pParent );

	parent = pParent;

	CenterWindow();

	m_slider_hand_hue_max.SetRange( 0, 360 );
	m_slider_hand_hue_min.SetRange( 0, 360 );

	m_slider_hand_sat_max.SetRange( 0, 100 );
	m_slider_hand_sat_min.SetRange( 0, 100 );

	m_slider_hand_val_max.SetRange( 0, 100 );
	m_slider_hand_val_min.SetRange( 0, 100 );

	m_spin_hand_erode.SetRange( 0, 32 );
	m_spin_hand_dilate.SetRange( 0, 32 );

	m_spin_hand_point_side_diff.SetRange( 0, 640 );

	Setup_pointers_to_controls();
}

CDialogGestureParameters::~CDialogGestureParameters()
{
}

void CDialogGestureParameters::Configure_Dialog( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	if( vision_bot->pThreadProc == nullptr ) return;
	if( vision_bot->pThreadProc->image_procObject == nullptr ) return;
	
	set_GestureClassifier_pointer( &vision_bot->pThreadProc->image_procObject->Gesture );

	RefreshGUI();
}

void CDialogGestureParameters::Setup_pointers_to_controls( void )
{
	//----- Hu moment power -----
	pEdit_Power[0] = &m_edit_hu_power_m1;
	pEdit_Power[1] = &m_edit_hu_power_m2;
	pEdit_Power[2] = &m_edit_hu_power_m3;

	//----- Above Grip -----
	pEdit_AG[0] = &m_edit_hu1_ag_min;
	pEdit_AG[1] = &m_edit_hu1_ag_max;
	pEdit_AG[2] = &m_edit_hu2_ag_min;
	pEdit_AG[3] = &m_edit_hu2_ag_max;
	pEdit_AG[4] = &m_edit_hu3_ag_min;
	pEdit_AG[5] = &m_edit_hu3_ag_max;

	//----- Side Grip -----
	pEdit_SG[0] = &m_edit_hu1_sg_min;
	pEdit_SG[1] = &m_edit_hu1_sg_max;
	pEdit_SG[2] = &m_edit_hu2_sg_min;
	pEdit_SG[3] = &m_edit_hu2_sg_max;
	pEdit_SG[4] = &m_edit_hu3_sg_min;
	pEdit_SG[5] = &m_edit_hu3_sg_max;

	//----- Pointing -----
	pEdit_PT[0] = &m_edit_hu1_pt_min;
	pEdit_PT[1] = &m_edit_hu1_pt_max;
	pEdit_PT[2] = &m_edit_hu2_pt_min;
	pEdit_PT[3] = &m_edit_hu2_pt_max;
	pEdit_PT[4] = &m_edit_hu3_pt_min;
	pEdit_PT[5] = &m_edit_hu3_pt_max;
}

void CDialogGestureParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SLIDER_HAND_MIN_HUE, m_slider_hand_hue_min );
	DDX_Control( pDX, IDC_SLIDER_HAND_MAX_HUE, m_slider_hand_hue_max );
	DDX_Control( pDX, IDC_SLIDER_HAND_MIN_SAT, m_slider_hand_sat_min );
	DDX_Control( pDX, IDC_SLIDER_HAND_MAX_SAT, m_slider_hand_sat_max );
	DDX_Control( pDX, IDC_SLIDER_HAND_MAX_VAL, m_slider_hand_val_max );
	DDX_Control( pDX, IDC_SLIDER_HAND_MIN_VAL, m_slider_hand_val_min );
	DDX_Control( pDX, IDC_RADIO_HAND_OPEN, m_radio_hand_open );
	DDX_Control( pDX, IDC_RADIO_HAND_CLOSE, m_radio_hand_close );
	DDX_Control( pDX, IDC_SPIN_HAND_ERODE, m_spin_hand_erode );
	DDX_Control( pDX, IDC_SPIN_HAND_DILATE, m_spin_hand_dilate );
	DDX_Control( pDX, IDC_STATIC_HAND_MIN_HUE, m_static_hand_hue_min );
	DDX_Control( pDX, IDC_STATIC_HAND_MAX_HUE, m_static_hand_hue_max );
	DDX_Control( pDX, IDC_STATIC_HAND_MIN_SAT, m_static_hand_sat_min );
	DDX_Control( pDX, IDC_STATIC_HAND_MAX_SAT, m_static_hand_sat_max );
	DDX_Control( pDX, IDC_STATIC_HAND_MIN_VAL, m_static_hand_val_min );
	DDX_Control( pDX, IDC_STATIC_HAND_MAX_VAL, m_static_hand_val_max );
	DDX_Control( pDX, IDC_SPIN_POINT_SIDE_DIFF, m_spin_hand_point_side_diff );
	DDX_Control( pDX, IDC_CHECK_GESTURE_ACTIVE, m_check_gesture_active );
	DDX_Control( pDX, IDC_EDIT_HU1_AG_MIN, m_edit_hu1_ag_min );
	DDX_Control( pDX, IDC_EDIT_HU2_AG_MIN, m_edit_hu2_ag_min );
	DDX_Control( pDX, IDC_EDIT_HU3_AG_MIN, m_edit_hu3_ag_min );
	DDX_Control( pDX, IDC_EDIT_HU1_AG_MAX, m_edit_hu1_ag_max );
	DDX_Control( pDX, IDC_EDIT_HU2_AG_MAX, m_edit_hu2_ag_max );
	DDX_Control( pDX, IDC_EDIT_HU3_AG_MAX, m_edit_hu3_ag_max );
	DDX_Control( pDX, IDC_EDIT_HU1_SG_MIN, m_edit_hu1_sg_min );
	DDX_Control( pDX, IDC_EDIT_HU2_SG_MIN, m_edit_hu2_sg_min );
	DDX_Control( pDX, IDC_EDIT_HU3_SG_MIN, m_edit_hu3_sg_min );
	DDX_Control( pDX, IDC_EDIT_HU1_SG_MAX, m_edit_hu1_sg_max );
	DDX_Control( pDX, IDC_EDIT_HU2_SG_MAX, m_edit_hu2_sg_max );
	DDX_Control( pDX, IDC_EDIT_HU3_SG_MAX, m_edit_hu3_sg_max );
	DDX_Control( pDX, IDC_EDIT_HU1_PT_MIN, m_edit_hu1_pt_min );
	DDX_Control( pDX, IDC_EDIT_HU2_PT_MIN, m_edit_hu2_pt_min );
	DDX_Control( pDX, IDC_EDIT_HU3_PT_MIN, m_edit_hu3_pt_min );
	DDX_Control( pDX, IDC_EDIT_HU1_PT_MAX, m_edit_hu1_pt_max );
	DDX_Control( pDX, IDC_EDIT_HU2_PT_MAX, m_edit_hu2_pt_max );
	DDX_Control( pDX, IDC_EDIT_HU3_PT_MAX, m_edit_hu3_pt_max );
	DDX_Control( pDX, IDC_EDIT_HU_POWER_M1, m_edit_hu_power_m1 );
	DDX_Control( pDX, IDC_EDIT_HU_POWER_M2, m_edit_hu_power_m2 );
	DDX_Control( pDX, IDC_EDIT_HU_POWER_M3, m_edit_hu_power_m3 );
}


BEGIN_MESSAGE_MAP(CDialogGestureParameters, CDialog)
	ON_BN_CLICKED( IDC_RADIO_HAND_OPEN, &CDialogGestureParameters::OnBnClickedRadioOpen )
	ON_BN_CLICKED( IDC_RADIO_HAND_CLOSE, &CDialogGestureParameters::OnBnClickedRadioClose )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MIN_HUE, &CDialogGestureParameters::OnNMCustomdrawSliderMinHue )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MAX_HUE, &CDialogGestureParameters::OnNMCustomdrawSliderMaxHue )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MIN_SAT, &CDialogGestureParameters::OnNMCustomdrawSliderMinSat )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MAX_SAT, &CDialogGestureParameters::OnNMCustomdrawSliderMaxSat )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MIN_VAL, &CDialogGestureParameters::OnNMCustomdrawSliderMinVal )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_SLIDER_HAND_MAX_VAL, &CDialogGestureParameters::OnNMCustomdrawSliderMaxVal )
	ON_EN_CHANGE( IDC_EDIT_HAND_ERODE, &CDialogGestureParameters::OnEnChangeEditErode )
	ON_EN_CHANGE( IDC_EDIT_HAND_DILATE, &CDialogGestureParameters::OnEnChangeEditDilate )
	ON_BN_CLICKED( IDC_BUTTON_LOAD_FILE, &CDialogGestureParameters::OnBnClickedButtonLoadFile )
	ON_BN_CLICKED( IDC_BUTTON_SAVE_FILE, &CDialogGestureParameters::OnBnClickedButtonSaveFile )
	ON_EN_CHANGE( IDC_EDIT_POINT_SIDE_DIFF, &CDialogGestureParameters::OnEnChangeEditPointSideDiff )
	ON_BN_CLICKED( IDC_CHECK_GESTURE_ACTIVE, &CDialogGestureParameters::OnBnClickedCheckGestureActive )
	ON_BN_CLICKED( IDC_BUTTON_UPDATE_HU_ABOVE, &CDialogGestureParameters::OnBnClickedButtonUpdateHuAbove )
	ON_BN_CLICKED( IDC_BUTTON_UPDATE_HU_POINTING, &CDialogGestureParameters::OnBnClickedButtonUpdateHuPointing )
	ON_BN_CLICKED( IDC_BUTTON_UPDATE_HU_SIDE, &CDialogGestureParameters::OnBnClickedButtonUpdateHuSide )
	ON_BN_CLICKED( IDC_BUTTON_UPDATE_HU_POWER, &CDialogGestureParameters::OnBnClickedButtonUpdateHuPower )
END_MESSAGE_MAP()


// CDialogGestureParameters message handlers


void CDialogGestureParameters::OnBnClickedRadioOpen()
{
	// TODO: Add your control notification handler code here
	m_radio_hand_open.SetCheck( BST_CHECKED );
	m_radio_hand_close.SetCheck( BST_UNCHECKED );

	pGestureClassifier->Hand_morph[Seg::OP].set( MORPH_OPERATION_OPEN );
}


void CDialogGestureParameters::OnBnClickedRadioClose()
{
	// TODO: Add your control notification handler code here
	m_radio_hand_open.SetCheck( BST_UNCHECKED );
	m_radio_hand_close.SetCheck( BST_CHECKED );

	pGestureClassifier->Hand_morph[Seg::OP].set( MORPH_OPERATION_CLOSE );
}


void CDialogGestureParameters::OnNMCustomdrawSliderMinHue( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_hue_min.GetPos();
	pGestureClassifier->Hand_hue[Seg::MIN].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;

	m_static_hand_hue_min.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnNMCustomdrawSliderMaxHue( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_hue_max.GetPos();
	pGestureClassifier->Hand_hue[Seg::MAX].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_hand_hue_max.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnNMCustomdrawSliderMinSat( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_sat_min.GetPos();
	pGestureClassifier->Hand_sat[Seg::MIN].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;

	m_static_hand_sat_min.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnNMCustomdrawSliderMaxSat( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_sat_max.GetPos();
	pGestureClassifier->Hand_sat[Seg::MAX].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_hand_sat_max.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnNMCustomdrawSliderMinVal( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_val_min.GetPos();
	pGestureClassifier->Hand_val[Seg::MIN].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;

	m_static_hand_val_min.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnNMCustomdrawSliderMaxVal( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>( pNMHDR );
	// TODO: Add your control notification handler code here

	int pos = m_slider_hand_val_max.GetPos();
	pGestureClassifier->Hand_val[Seg::MAX].set( static_cast<unsigned int>( pos ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_hand_val_max.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}


void CDialogGestureParameters::OnEnChangeEditErode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( m_spin_hand_erode.m_hWnd == nullptr ) return;

	unsigned int param = static_cast<unsigned int>( m_spin_hand_erode.GetPos() );
	pGestureClassifier->Hand_morph[Seg::ERODE].set( param );
}


void CDialogGestureParameters::OnEnChangeEditDilate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if( m_spin_hand_dilate.m_hWnd == nullptr ) return;

	unsigned int param = static_cast<unsigned int>( m_spin_hand_dilate.GetPos() );
	pGestureClassifier->Hand_morph[Seg::DILATE].set( param );
}


void CDialogGestureParameters::OnEnChangeEditPointSideDiff()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ( m_spin_hand_point_side_diff.m_hWnd == nullptr ) || ( pGestureClassifier == nullptr ) ) return;

	pGestureClassifier->nWidthGesture.set( m_spin_hand_point_side_diff.GetPos() );
}

void CDialogGestureParameters::OnBnClickedCheckGestureActive()
{
	// TODO: Add your control notification handler code here
	if( ( m_check_gesture_active.m_hWnd == nullptr ) || ( pGestureClassifier == nullptr ) ) return;

	pGestureClassifier->bActive.set( m_check_gesture_active.GetCheck() == BST_CHECKED );
}

void CDialogGestureParameters::set_GestureClassifier_pointer( CGestureClassifier * ptr )
{
	pGestureClassifier = ptr;
}


void CDialogGestureParameters::OnBnClickedButtonLoadFile()
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
		pGestureClassifier->Load_parameters_from_file( file_name );
		RefreshGUI();
	}
}


void CDialogGestureParameters::OnBnClickedButtonSaveFile()
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
		pGestureClassifier->Save_parameters_to_file( file_name );
	}
}

void CDialogGestureParameters::RefreshGUI( void )
{
	if( pGestureClassifier == nullptr ) return;

	unsigned int parameter = 0;
	float conv = 100.0f / 255.0f;

	std::tstringstream str_aux;

	m_check_gesture_active.SetCheck( pGestureClassifier->bActive.get() ? BST_CHECKED : BST_UNCHECKED );

	// Hue
	parameter = pGestureClassifier->Hand_hue[Seg::MIN].get();
	m_slider_hand_hue_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_hand_hue_min.SetWindowText( str_aux.str().c_str() );

	parameter = pGestureClassifier->Hand_hue[Seg::MAX].get();
	m_slider_hand_hue_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_hand_hue_max.SetWindowText( str_aux.str().c_str() );

	// Saturation
	parameter = pGestureClassifier->Hand_sat[Seg::MIN].get();
	m_slider_hand_sat_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_hand_sat_min.SetWindowText( str_aux.str().c_str() );

	parameter = pGestureClassifier->Hand_sat[Seg::MAX].get();
	m_slider_hand_sat_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_hand_sat_max.SetWindowText( str_aux.str().c_str() );

	// Value
	parameter = pGestureClassifier->Hand_val[Seg::MIN].get();
	m_slider_hand_val_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_hand_val_min.SetWindowText( str_aux.str().c_str() );

	parameter = pGestureClassifier->Hand_val[Seg::MAX].get();
	m_slider_hand_val_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_hand_val_max.SetWindowText( str_aux.str().c_str() );

	//Morphologic operation
	parameter = pGestureClassifier->Hand_morph[Seg::OP].get();
	switch( parameter )
	{
	case MORPH_OPERATION_CLOSE:
		m_radio_hand_close.SetCheck( BST_CHECKED );
		m_radio_hand_open.SetCheck( BST_UNCHECKED );
		break;

	default:
	case MORPH_OPERATION_OPEN:
		m_radio_hand_close.SetCheck( BST_UNCHECKED );
		m_radio_hand_open.SetCheck( BST_CHECKED );
		break;
	}

	m_spin_hand_erode.SetPos( pGestureClassifier->Hand_morph[Seg::ERODE].get() );
	m_spin_hand_dilate.SetPos( pGestureClassifier->Hand_morph[Seg::DILATE].get() );
	m_spin_hand_point_side_diff.SetPos( pGestureClassifier->nWidthGesture.get() );

	//--------------------------------------------------------------------------
	// Moments edit boxes

	str_aux.precision( 4 );

	size_t
		i = 0,
		vSize = 0,
		num_edit = 0;

	//----- Hu moment power -----

#if 0
	pGestureClassifier->dPower.CMutexYarp::lock();
	
	vSize = pGestureClassifier->dPower.Obj.size();
	
	num_edit = sizeof( pEdit_Power ) / sizeof( CEdit* );

	for( i = 0; i < vSize; i++ )
	{
		if( i < num_edit )
		{
			if( pEdit_Power[i] != nullptr )
			{
				str_aux.str( _T( "" ) );
				str_aux << pGestureClassifier->dPower.Obj.at( i );
				pEdit_Power[i]->SetWindowText( str_aux.str().c_str() );
			}
		}
	}

	pGestureClassifier->dPower.CMutexYarp::unlock();

	//----- Above Grip -----

	pGestureClassifier->vMoments_Above.lock();

	vSize = pGestureClassifier->vMoments_Above.Obj.size();

	num_edit = sizeof( pEdit_AG ) / sizeof( CEdit* );

	for( i = 0; i < vSize; i++ )
	{
		if( i < num_edit )
		{
			if( pEdit_AG[i] != nullptr )
			{
				str_aux.str( _T( "" ) );
				str_aux << pGestureClassifier->vMoments_Above.Obj.at( i );
				pEdit_AG[i]->SetWindowText( str_aux.str().c_str() );
			}
		}
	}

	pGestureClassifier->vMoments_Above.unlock();

	//----- Side Grip -----

	pGestureClassifier->vMoments_Side.lock();

	vSize = pGestureClassifier->vMoments_Side.Obj.size();

	num_edit = sizeof( pEdit_SG ) / sizeof( CEdit* );
	
	for( i = 0; i < vSize; i++ )
	{
		if( i < num_edit )
		{
			if( pEdit_SG[i] != nullptr )
			{
				str_aux.str( _T( "" ) );
				str_aux << pGestureClassifier->vMoments_Side.Obj.at( i );
				pEdit_SG[i]->SetWindowText( str_aux.str().c_str() );
			}
		}
	}

	pGestureClassifier->vMoments_Side.unlock();

	//----- Pointing -----

	pGestureClassifier->vMoments_Point.lock();

	vSize = pGestureClassifier->vMoments_Point.Obj.size();

	num_edit = sizeof( pEdit_PT ) / sizeof( CEdit* );

	for( i = 0; i < vSize; i++ )
	{
		if( i < num_edit )
		{
			if( pEdit_PT[i] != nullptr )
			{
				str_aux.str( _T( "" ) );
				str_aux << pGestureClassifier->vMoments_Point.Obj.at( i );
				pEdit_PT[i]->SetWindowText( str_aux.str().c_str() );
			}
		}
	}

	pGestureClassifier->vMoments_Point.unlock();
#endif
}


void CDialogGestureParameters::OnBnClickedButtonUpdateHuAbove()
{
	// TODO: Add your control notification handler code here
	if( pGestureClassifier == nullptr ) return;

	double dVal = 0.0;
	
	size_t num_edit = sizeof( pEdit_AG ) / sizeof( CEdit* );

	pGestureClassifier->vMoments_Above.lock();

	size_t vSize = pGestureClassifier->vMoments_Above.Obj.size();

	for( size_t i = 0; i < vSize; i++ )
	{
		if( i < num_edit )
			if( ReadValueFromEditBox( pEdit_AG[i], &dVal ) )
				pGestureClassifier->vMoments_Above.Obj.at( i ) = dVal;
	}

	pGestureClassifier->vMoments_Above.unlock();
}


void CDialogGestureParameters::OnBnClickedButtonUpdateHuSide()
{
	// TODO: Add your control notification handler code here
	if( pGestureClassifier == nullptr ) return;

	double dVal = 0.0;

	pGestureClassifier->vMoments_Side.lock();

	size_t 
		num_edit = sizeof( pEdit_SG ) / sizeof( CEdit* ),
		vSize = pGestureClassifier->vMoments_Side.Obj.size();

	for( size_t i = 0; i < vSize; i++ )
	{
		if( i<num_edit )
			if( ReadValueFromEditBox( pEdit_SG[i], &dVal ) )
				pGestureClassifier->vMoments_Side.Obj.at( i ) = dVal;
	}

	pGestureClassifier->vMoments_Side.unlock();
}


void CDialogGestureParameters::OnBnClickedButtonUpdateHuPointing()
{
	// TODO: Add your control notification handler code here
	if( pGestureClassifier == nullptr ) return;

	double dVal = 0.0;

	pGestureClassifier->vMoments_Point.lock();

	size_t 
		num_edit = sizeof( pEdit_PT ) / sizeof( CEdit* ),
		vSize = pGestureClassifier->vMoments_Point.Obj.size();

	for( size_t i = 0; i < vSize; i++ )
	{
		if( i<num_edit )
			if( ReadValueFromEditBox( pEdit_PT[i], &dVal ) )
				pGestureClassifier->vMoments_Point.Obj.at( i ) = dVal;
	}

	pGestureClassifier->vMoments_Point.unlock();
}


void CDialogGestureParameters::OnBnClickedButtonUpdateHuPower()
{
	// TODO: Add your control notification handler code here
	if( pGestureClassifier == nullptr ) return;

	double dVal = 0.0;

	pGestureClassifier->dPower.lock();
	
	size_t 
		num_edit = sizeof( pEdit_Power ) / sizeof( CEdit* ),
		vSize = pGestureClassifier->dPower.Obj.size();

	for( size_t i = 0; i < vSize; i++ )
	{
		if( i<num_edit )
			if( ReadValueFromEditBox( pEdit_Power[i], &dVal ) )
				pGestureClassifier->dPower.Obj.at( i ) = dVal;
	}

	pGestureClassifier->dPower.unlock();
}
