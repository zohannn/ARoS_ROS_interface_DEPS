#include <afxdialogex.h>
#include <exception>

#include "VisionBotDlg.h"
#include "DialogHumanInfo.h"
#include "DialogUtils.h"
#include "MovementQuantifier.h"
#include "ProcessImage.h"
#include "ProcessThread.h"
#include "string_operations.h"

#ifdef USE_StereoVidere
	#include "StereoVidere.h"
#endif

IMPLEMENT_DYNAMIC(CDialogHumanInfo, CDialog)

CDialogHumanInfo::CDialogHumanInfo(CWnd* pParent /*=NULL*/)
: CDialog(CDialogHumanInfo::IDD, pParent)
{
	Create( IDD_DIALOG_HUMAN_INFO, pParent );
	
	ptr_parent = pParent;

	CenterWindow();

	SetupControls();
}

void CDialogHumanInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_IMAGE_MOVEMENT, m_check_image_movement);
	DDX_Control(pDX, IDC_CHECK_SHOW_IMAGE_STEREO, m_check_image_stereo);
	DDX_Control(pDX, IDC_CHECK_HAND_MOVEMENT, m_check_movement_hand);
	DDX_Control(pDX, IDC_CHECK_BODY_MOVEMENT, m_check_movement_body);
	DDX_Control(pDX, IDC_SPIN_XMIN, m_spin_xmin);
	DDX_Control(pDX, IDC_SPIN_XMAX, m_spin_xmax);
	DDX_Control(pDX, IDC_SPIN_YMIN, m_spin_ymin);
	DDX_Control(pDX, IDC_SPIN_YMAX, m_spin_ymax);
	DDX_Control(pDX, IDC_SPIN_ZMIN, m_spin_zmin);
	DDX_Control(pDX, IDC_SPIN_ZMAX, m_spin_zmax);
	DDX_Control(pDX, IDC_EDIT_MOV_HAND_IDLE, m_edit_mov_hand_idle);
	DDX_Control(pDX, IDC_EDIT_MOV_HAND_LOW, m_edit_mov_hand_low);
	DDX_Control(pDX, IDC_EDIT_MOV_HAND_MEDIUM, m_edit_mov_hand_medium);
	DDX_Control(pDX, IDC_EDIT_MOV_BODY_IDLE, m_edit_mov_body_idle);
	DDX_Control(pDX, IDC_EDIT_MOV_BODY_LOW, m_edit_mov_body_low);
	DDX_Control(pDX, IDC_EDIT_MOV_BODY_MEDIUM, m_edit_mov_body_medium);
}


BEGIN_MESSAGE_MAP(CDialogHumanInfo, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IMAGE_MOVEMENT, &CDialogHumanInfo::OnBnClickedCheckShowImageMovement)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IMAGE_STEREO, &CDialogHumanInfo::OnBnClickedCheckShowImageStereo)
	ON_EN_CHANGE(IDC_EDIT_XMIN, &CDialogHumanInfo::OnEnChangeEditXmin)
	ON_EN_CHANGE(IDC_XMAX, &CDialogHumanInfo::OnEnChangeXmax)
	ON_EN_CHANGE(IDC_EDIT_YMIN, &CDialogHumanInfo::OnEnChangeEditYmin)
	ON_EN_CHANGE(IDC_EDIT_YMAX, &CDialogHumanInfo::OnEnChangeEditYmax)
	ON_EN_CHANGE(IDC_EDIT_ZMIN, &CDialogHumanInfo::OnEnChangeEditZmin)
	ON_EN_CHANGE(IDC_EDIT_ZMAX, &CDialogHumanInfo::OnEnChangeEditZmax)
	ON_EN_CHANGE(IDC_EDIT_MOV_HAND_IDLE, &CDialogHumanInfo::OnEnChangeEditMovHandIdle)
	ON_EN_CHANGE(IDC_EDIT_MOV_HAND_LOW, &CDialogHumanInfo::OnEnChangeEditMovHandLow)
	ON_EN_CHANGE(IDC_EDIT_MOV_HAND_MEDIUM, &CDialogHumanInfo::OnEnChangeEditMovHandMedium)
	ON_EN_CHANGE(IDC_EDIT_MOV_BODY_IDLE, &CDialogHumanInfo::OnEnChangeEditMovBodyIdle)
	ON_EN_CHANGE(IDC_EDIT_MOV_BODY_LOW, &CDialogHumanInfo::OnEnChangeEditMovBodyLow)
	ON_EN_CHANGE(IDC_EDIT_MOV_BODY_MEDIUM, &CDialogHumanInfo::OnEnChangeEditMovBodyMedium)
	ON_BN_CLICKED(IDC_BUTTON_HUMANINFO_SAVE, &CDialogHumanInfo::OnBnClickedButtonHumanInfoSave)
	ON_BN_CLICKED(IDC_BUTTON_HUMANINFO_LOAD, &CDialogHumanInfo::OnBnClickedButtonHumanInfoLoad)
	ON_BN_CLICKED(IDC_CHECK_HAND_MOVEMENT, &CDialogHumanInfo::OnBnClickedCheckHandMovement)
	ON_BN_CLICKED(IDC_CHECK_BODY_MOVEMENT, &CDialogHumanInfo::OnBnClickedCheckBodyMovement)
END_MESSAGE_MAP()


// CDialogHumanInfo message handlers

void CDialogHumanInfo::Configure_Dialog( void )
{
	LoadParameters();
}

void CDialogHumanInfo::SetupControls( void )
{
	m_spin_xmin.SetRange32( 0, 5000 );
	m_spin_xmax.SetRange32( 0, 5000 );

	m_spin_ymin.SetRange32( -3000, 3000 );
	m_spin_ymax.SetRange32( -3000, 3000 );

	m_spin_zmin.SetRange32( 0, 3000 );
	m_spin_zmax.SetRange32( 0, 3000 );
}

bool CDialogHumanInfo::isCtrlValid( CWnd * pCtrl )
{
	return ( pCtrl->m_hWnd != nullptr );
}

void CDialogHumanInfo::LoadParameters( void )
{
	cv::Point3i
		ptLimit_min( 0, 0, 0 ),
		ptLimit_max( 0, 0, 0 );

	if( ptr_parent == nullptr ) return;

	CVisionBotDlg * vision_bot = ( (CVisionBotDlg *) ptr_parent );

#ifdef USE_StereoVidere

	if( vision_bot->pThreadProc == nullptr ) return;

	if( vision_bot->pThreadProc->pStereoProcess != nullptr )
	{
		CStereoVidere * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess->objStereo;

		if( pStereo != nullptr )
		{
			pStereo->ptLimitMin.lock();
			ptLimit_min.x = static_cast<int>( pStereo->ptLimitMin.x );
			ptLimit_min.y = static_cast<int>( pStereo->ptLimitMin.y );
			ptLimit_min.z = static_cast<int>( pStereo->ptLimitMin.z );
			pStereo->ptLimitMin.unlock();

			pStereo->ptLimitMax.lock();
			ptLimit_max.x = static_cast<int>( pStereo->ptLimitMax.x );
			ptLimit_max.y = static_cast<int>( pStereo->ptLimitMax.y );
			ptLimit_max.z = static_cast<int>( pStereo->ptLimitMax.z );
			pStereo->ptLimitMax.unlock();
		}
	}
#endif // USE_StereoVidere

	m_spin_xmin.SetPos32( ptLimit_min.x );
	m_spin_ymin.SetPos32( ptLimit_min.y );
	m_spin_zmin.SetPos32( ptLimit_min.z );

	m_spin_xmax.SetPos32( ptLimit_max.x );
	m_spin_ymax.SetPos32( ptLimit_max.y );
	m_spin_zmax.SetPos32( ptLimit_max.z );

//---------------------------------------------------------------------------
	if( vision_bot->pData != nullptr )
	{
		std::stringstream strm_text( "" );

		CHumanInfo * human_info = &vision_bot->pData->HumanInfo;

		if( human_info != nullptr )
		{
			float 
				fIdle = 0.0f,
				fLow = 0.0f,
				fMedium = 0.0f;

			human_info->vMovParameters_Hand.lock();
			fIdle = human_info->vMovParameters_Hand.Obj.at( Quantity::IDLE );
			fLow = human_info->vMovParameters_Hand.Obj.at( Quantity::LOW ),
			fMedium = human_info->vMovParameters_Hand.Obj.at( Quantity::MEDIUM );
			human_info->vMovParameters_Hand.unlock();

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fIdle;
			m_edit_mov_hand_idle.SetWindowText( CA2T( strm_text.str().c_str() ) );

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fLow;
			m_edit_mov_hand_low.SetWindowText( CA2T( strm_text.str().c_str() ) );

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fMedium;
			m_edit_mov_hand_medium.SetWindowText( CA2T( strm_text.str().c_str() ) );

			human_info->vMovParameters_Body.lock();
			fIdle = human_info->vMovParameters_Body.Obj.at( Quantity::IDLE );
			fLow = human_info->vMovParameters_Body.Obj.at( Quantity::LOW ),
			fMedium = human_info->vMovParameters_Body.Obj.at( Quantity::MEDIUM );
			human_info->vMovParameters_Body.unlock();

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fIdle;
			m_edit_mov_body_idle.SetWindowText( CA2T( strm_text.str().c_str() ) );

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fLow;
			m_edit_mov_body_low.SetWindowText( CA2T( strm_text.str().c_str() ) );

			strm_text.str( "" );
			strm_text.clear();
			strm_text << fMedium;
			m_edit_mov_body_medium.SetWindowText( CA2T( strm_text.str().c_str() ) );

			human_info = nullptr;
		}
	}

	CProcessImage * img_proc = nullptr;

	if( vision_bot->pThreadProc != nullptr )
		img_proc = vision_bot->pThreadProc->image_procObject;

	if( img_proc != nullptr )
	{
		int 
			state_hand = img_proc->bPrintHandMovement.get() ? BST_CHECKED : BST_UNCHECKED,
			state_body = img_proc->bPrintBodyMovement.get() ? BST_CHECKED : BST_UNCHECKED;

		m_check_movement_hand.SetCheck( state_hand );
		m_check_movement_body.SetCheck( state_body );
	}

	img_proc = nullptr;
}

void CDialogHumanInfo::ReadValueFromSpinCtrl( CSpinButtonCtrl * pSpin, CAttachMutex_t< cv::Point3f > * pSemaphore, float * pValue )
{
	float fPos = static_cast<float>( pSpin->GetPos32() );

	pSemaphore->lock();
	*pValue = fPos;
	pSemaphore->unlock();
}

void CDialogHumanInfo::OnBnClickedCheckShowImageMovement()
{
	// TODO: Add your control notification handler code here

	// If the control hasn't been initialized, exit
	if( !isCtrlValid( &m_check_image_movement) ) return;

	// Cast parent to the correct type
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) ptr_parent;

	// Get a pointer to the processing image object
	CMovementQuantifier * ptrMovQuant = &( (CProcessImage *) vision_bot->pThreadProc->image_procObject )->BodyMovementQuantifier;

	int button_state = m_check_image_movement.GetCheck();

	bool button_checked = false;
	
	if( button_state == BST_CHECKED )
		button_checked = true;

	ptrMovQuant->set_Display_Image_Movement( button_checked );
}

void CDialogHumanInfo::OnBnClickedCheckShowImageStereo()
{
	// TODO: Add your control notification handler code here

	// If the control hasn't been initialized, exit
	if( !isCtrlValid( &m_check_image_stereo) ) return;

	// Cast parent to the correct type
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) ptr_parent;

	// Get a pointer to the processing image object
	CMovementQuantifier * ptrMovQuant = &( (CProcessImage *) vision_bot->pThreadProc->image_procObject )->BodyMovementQuantifier;

	int button_state = m_check_image_stereo.GetCheck();

	bool button_checked = false;
	
	if( button_state == BST_CHECKED )
		button_checked = true;

	ptrMovQuant->set_Display_Image_Stereo( button_checked );
}

void CDialogHumanInfo::OnBnClickedCheckHandMovement()
{
	// TODO: Add your control notification handler code here
	// If the control hasn't been initialized, exit
	if( !isCtrlValid( &m_check_movement_hand ) ) return;

	// Cast parent to the correct type
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) ptr_parent;

	// Get a pointer to the processing image object
	CProcessImage * ptrProcImg = ( (CProcessImage *) vision_bot->pThreadProc->image_procObject );

	int button_state = m_check_movement_hand.GetCheck();

	bool button_checked = false;
	
	if( button_state == BST_CHECKED )
		button_checked = true;

	ptrProcImg->bPrintHandMovement.set( button_checked );

	ptrProcImg = nullptr;
	vision_bot = nullptr;
}

void CDialogHumanInfo::OnBnClickedCheckBodyMovement()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	// If the control hasn't been initialized, exit
	if( !isCtrlValid( &m_check_movement_body ) ) return;

	// Cast parent to the correct type
	CVisionBotDlg * vision_bot = (CVisionBotDlg*) ptr_parent;

	// Get a pointer to the processing image object
	CProcessImage * ptrProcImg = ( (CProcessImage *) vision_bot->pThreadProc->image_procObject );

	int button_state = m_check_movement_body.GetCheck();

	bool button_checked = false;
	
	if( button_state == BST_CHECKED )
		button_checked = true;

	ptrProcImg->bPrintBodyMovement.set( button_checked );

	ptrProcImg = nullptr;
	vision_bot = nullptr;
}

void CDialogHumanInfo::OnEnChangeEditXmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_xmin ) ) return;
	
	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_xmin, &pStereo->objStereo->ptLimitMin, &pStereo->objStereo->ptLimitMin.x );
#endif
}

void CDialogHumanInfo::OnEnChangeXmax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_xmax ) ) return;

	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_xmax, &pStereo->objStereo->ptLimitMax, &pStereo->objStereo->ptLimitMax.x );
#endif
}

void CDialogHumanInfo::OnEnChangeEditYmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_ymin ) ) return;

	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_ymin, &pStereo->objStereo->ptLimitMin, &pStereo->objStereo->ptLimitMin.y );
#endif
}

void CDialogHumanInfo::OnEnChangeEditYmax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_ymax ) ) return;

	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_ymax, &pStereo->objStereo->ptLimitMax, &pStereo->objStereo->ptLimitMax.y );
#endif
}

void CDialogHumanInfo::OnEnChangeEditZmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_zmin ) ) return;

	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_zmin, &pStereo->objStereo->ptLimitMin, &pStereo->objStereo->ptLimitMin.z );
#endif
}

void CDialogHumanInfo::OnEnChangeEditZmax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

#ifdef USE_StereoVidere
	if( !isCtrlValid( &m_spin_zmax ) ) return;

	CStereoProcess * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess;

	if( pStereo == nullptr ) return;

	ReadValueFromSpinCtrl( &m_spin_zmax, &pStereo->objStereo->ptLimitMax, &pStereo->objStereo->ptLimitMax.z );
#endif
}

void CDialogHumanInfo::OnEnChangeEditMovHandIdle()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;
	
	if( ReadValueFromEditBox( &m_edit_mov_hand_idle, &fValue ) )
	{
		human_info->vMovParameters_Hand.lock();
		human_info->vMovParameters_Hand.Obj.at( Quantity::IDLE ) = fValue; 
		human_info->vMovParameters_Hand.unlock();
	}
}

void CDialogHumanInfo::OnEnChangeEditMovHandLow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;

	if( ReadValueFromEditBox( &m_edit_mov_hand_low, &fValue ) )
	{
		human_info->vMovParameters_Hand.lock();
		human_info->vMovParameters_Hand.Obj.at( Quantity::LOW ) = fValue; 
		human_info->vMovParameters_Hand.unlock();
	}
}

void CDialogHumanInfo::OnEnChangeEditMovHandMedium()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;

	if( ReadValueFromEditBox( &m_edit_mov_hand_medium, &fValue ) )
	{
		human_info->vMovParameters_Hand.lock();
		human_info->vMovParameters_Hand.Obj.at( Quantity::MEDIUM ) = fValue; 
		human_info->vMovParameters_Hand.unlock();
	}
}

void CDialogHumanInfo::OnEnChangeEditMovBodyIdle()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;

	if( ReadValueFromEditBox( &m_edit_mov_body_idle, &fValue ) )
	{
		human_info->vMovParameters_Body.lock();
		human_info->vMovParameters_Body.Obj.at( Quantity::IDLE ) = fValue; 
		human_info->vMovParameters_Body.unlock();
	}
}

void CDialogHumanInfo::OnEnChangeEditMovBodyLow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;
	
	if( ReadValueFromEditBox( &m_edit_mov_body_low, &fValue ) )
	{
		human_info->vMovParameters_Body.lock();
		human_info->vMovParameters_Body.Obj.at( Quantity::LOW ) = fValue; 
		human_info->vMovParameters_Body.unlock();
	}
}

void CDialogHumanInfo::OnEnChangeEditMovBodyMedium()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( ptr_parent == nullptr ) return;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
	
	float fValue = 0.0f;
	
	if( ReadValueFromEditBox( &m_edit_mov_body_medium, &fValue ) )
	{
		human_info->vMovParameters_Body.lock();
		human_info->vMovParameters_Body.Obj.at( Quantity::MEDIUM ) = fValue; 
		human_info->vMovParameters_Body.unlock();
	}
}

void CDialogHumanInfo::Parse_Parameters_Save( void )
{
	std::stringstream buf;
	
	strFileBuffer = "# Display or not the different images";

	//--------------------------------------------------------------------------
	// Images display
	CMovementQuantifier * mov_quant = &( ( (CVisionBotDlg *) ptr_parent )->pThreadProc->image_procObject->BodyMovementQuantifier );
	
	if( mov_quant != nullptr )
	{
		int
			img_mov = ( mov_quant->get_Display_Image_Movement() ? 1 : 0 ),
			img_stereo = ( mov_quant->get_Display_Image_Stereo() ? 1 : 0 );
	
		mov_quant = nullptr;

		buf.str( "" );
		buf.clear();
		buf << std::endl <<
			"[Images]" << std::endl << 
			"Movement: " << img_mov << std::endl <<
			"Spatial_Segmentation: " << img_stereo << std::endl << std::endl;

		strFileBuffer += buf.str();
	}

	mov_quant = nullptr;

	//--------------------------------------------------------------------------
	// Visual feedback

	CProcessImage * img_proc = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->image_procObject;

	if( img_proc != nullptr )
	{
		int
			hand = img_proc->bPrintHandMovement.get() ? 1 : 0,
			body = img_proc->bPrintBodyMovement.get() ? 1 : 0;

		buf.str( "" );
		buf.clear();
		buf << 
			"# Control information printed on the displayed image" << std::endl <<
			"[Visual_Feedback]" << std::endl <<
			"Hand_Movement: " << hand << std::endl <<
			"Body_Movement: " << body << std::endl << std::endl;
	
		strFileBuffer += buf.str();
	}

	img_proc = nullptr;

	//--------------------------------------------------------------------------
	// Spatial segmentation
	cv::Point3i 
		ptMin( 0, 0, 0 ),
		ptMax( 0, 0, 0 );

	buf.str( "" );
	buf.clear();
	buf <<
		"# X, Y and Z define the boundaries that comprise the region where the user is (in mm)" << std::endl <<
		"[Spatial_segmentation_limits]" << std::endl;

#ifdef USE_StereoVidere
	if( ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess != nullptr )
	{
		CStereoVidere * stereo_proc = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess->objStereo;

		stereo_proc->ptLimitMin.lock();
		ptMin.x = static_cast<int>( stereo_proc->ptLimitMin.x );
		ptMin.y = static_cast<int>( stereo_proc->ptLimitMin.y );
		ptMin.z = static_cast<int>( stereo_proc->ptLimitMin.z );
		stereo_proc->ptLimitMin.unlock();

		stereo_proc->ptLimitMax.lock();
		ptMax.x = static_cast<int>( stereo_proc->ptLimitMax.x );
		ptMax.y = static_cast<int>( stereo_proc->ptLimitMax.y );
		ptMax.z = static_cast<int>( stereo_proc->ptLimitMax.z );
		stereo_proc->ptLimitMax.unlock();

		stereo_proc = nullptr;
	}
#endif

	buf <<
		"X: " << ptMin.x << " " << ptMax.x << std::endl <<
		"Y: " << ptMin.y << " " << ptMax.y << std::endl <<
		"Z: " << ptMin.z << " " << ptMax.z << std::endl << std::endl;

	strFileBuffer += buf.str();

	//--------------------------------------------------------------------------
	// Movement limits

	size_t 
		num_par = 0,
		iPar = 0;

	CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );

	buf.str( "" );
	buf.clear();
	buf << "[Movement_limits]" << std::endl <<
		"Body: ";

	human_info->vMovParameters_Body.lock();
	
	num_par = human_info->vMovParameters_Body.Obj.size();
	for( iPar=0 ; iPar<num_par ; iPar++ )
		buf << human_info->vMovParameters_Body.Obj.at( iPar ) << " ";
	
	human_info->vMovParameters_Body.unlock();
	
	buf << std::endl << "Hands: ";
	
	human_info->vMovParameters_Hand.lock();

	num_par = human_info->vMovParameters_Hand.Obj.size();
	for( iPar=0 ; iPar<num_par ; iPar++ )
		buf << human_info->vMovParameters_Hand.Obj.at( iPar ) << " ";

	human_info->vMovParameters_Hand.unlock();

	buf << std::endl;

	human_info = nullptr;

	strFileBuffer += buf.str();
}

bool CDialogHumanInfo::Parse_Parameters_Load( void )
{
	if( ptr_parent == nullptr ) return false;

	bool bSuccess = true;

	//--------------------------------------------------------------------------
	// Image display

	CMovementQuantifier * mov_quant = &( ( (CVisionBotDlg *) ptr_parent )->pThreadProc->image_procObject->BodyMovementQuantifier );

	if( mov_quant != nullptr )
	{
		yarp::os::Bottle bot;

		bot = prop_ConfigFile.findGroup( "Images" );
		
		bSuccess = bot.size() > 0;
		
		int 
			nMov = bot.find( "Movement:" ).asInt(),
			nSeg = bot.find( "Spatial_Segmentation:" ).asInt();

		mov_quant->set_Display_Image_Movement( ( nMov == 1 ) );
		
		mov_quant->set_Display_Image_Stereo( ( nSeg == 1 ) );

		mov_quant = nullptr;

		bot.clear();
	}

	CProcessImage * img_proc = ( ( (CVisionBotDlg *) ptr_parent )->pThreadProc->image_procObject );

	if( img_proc != nullptr )
	{
		yarp::os::Bottle bot;

		bot = prop_ConfigFile.findGroup( "Visual_Feedback" );

		bSuccess = bot.size() > 0;

		int
			nHand = bot.find( "Hand_Movement:" ).asInt(),
			nBody = bot.find( "Body_Movement:" ).asInt();

		img_proc->bPrintHandMovement.set( ( nHand == 1 ) );

		img_proc->bPrintBodyMovement.set( ( nBody == 1 ) );

		img_proc = nullptr;

		bot.clear();
	}

	return bSuccess;
}

void CDialogHumanInfo::OnBnClickedButtonHumanInfoSave()
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
		Save_parameters_to_file( file_name );
	}
}

void CDialogHumanInfo::OnBnClickedButtonHumanInfoLoad()
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
		Load_parameters_from_file( file_name );

		CHumanInfo * human_info = &( ( (CVisionBotDlg *) ptr_parent )->pData->HumanInfo );
		
		human_info->Load_parameters_from_file( file_name );
		
		human_info = nullptr;

		#ifdef USE_StereoVidere
		if( ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess != nullptr )
		{
			CStereoVidere * pStereo = ( (CVisionBotDlg *) ptr_parent )->pThreadProc->pStereoProcess->objStereo;

			pStereo->Load_parameters_from_file( file_name );

			pStereo = nullptr;
		}
		#endif
	
		LoadParameters();
	}
}


