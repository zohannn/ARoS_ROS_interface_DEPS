#include "VisionBotDlg.h"
#include "DialogSegmentation.h"

#include <string>
#include <sstream>

#include "ansi_unicode_definitions.h"
#include "DialogFileOpenSave_types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogSegmentation::CDialogSegmentation(CWnd* pParent /*=nullptr*/)
: CDialog(CDialogSegmentation::IDD, pParent)
, nSelectedColour( 0 )
{
	Create( IDD_DIALOG_SEGMENTATION, pParent );

	parent = pParent;

	CenterWindow();

	m_slider_hue_max.SetRange( 0, 360 );
	m_slider_hue_min.SetRange( 0, 360 );

	m_slider_sat_max.SetRange( 0, 100 );
	m_slider_sat_min.SetRange( 0, 100 );

	m_slider_val_max.SetRange( 0, 100 );
	m_slider_val_min.SetRange( 0, 100 );

	m_spin_erode.SetRange( 0, 32 );
	m_spin_dilate.SetRange( 0, 32 );
}

void CDialogSegmentation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSegmentation)
	DDX_Control(pDX, IDC_RADIO_OPEN, m_radio_open);
	DDX_Control(pDX, IDC_RADIO_CLOSE, m_radio_close);
	DDX_Control(pDX, IDC_SPIN_ERODE, m_spin_erode);
	DDX_Control(pDX, IDC_SPIN_DILATE, m_spin_dilate);
	DDX_Control(pDX, IDC_STATIC_MIN_VAL, m_static_val_min);
	DDX_Control(pDX, IDC_STATIC_MIN_SAT, m_static_sat_min);
	DDX_Control(pDX, IDC_STATIC_MIN_HUE, m_static_hue_min);
	DDX_Control(pDX, IDC_STATIC_MAX_VAL, m_static_val_max);
	DDX_Control(pDX, IDC_STATIC_MAX_SAT, m_static_sat_max);
	DDX_Control(pDX, IDC_STATIC_MAX_HUE, m_static_hue_max);
	DDX_Control(pDX, IDC_SLIDER_MIN_VAL, m_slider_val_min);
	DDX_Control(pDX, IDC_SLIDER_MIN_SAT, m_slider_sat_min);
	DDX_Control(pDX, IDC_SLIDER_MIN_HUE, m_slider_hue_min);
	DDX_Control(pDX, IDC_SLIDER_MAX_VAL, m_slider_val_max);
	DDX_Control(pDX, IDC_SLIDER_MAX_SAT, m_slider_sat_max);
	DDX_Control(pDX, IDC_SLIDER_MAX_HUE, m_slider_hue_max);
	DDX_Control(pDX, IDC_COMBO_COLOUR, m_combo_colour);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogSegmentation, CDialog)
	//{{AFX_MSG_MAP(CDialogSegmentation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MAX_HUE, OnNMCustomdrawSliderMaxHue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MAX_SAT, OnNMCustomdrawSliderMaxSat)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MAX_VAL, OnNMCustomdrawSliderMaxVal)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MIN_HUE, OnNMCustomdrawSliderMinHue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MIN_SAT, OnNMCustomdrawSliderMinSat)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MIN_VAL, OnNMCustomdrawSliderMinVal)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOUR, OnSelchangeComboColour)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FILE, OnButtonLoadFile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FILE, OnButtonSaveFile)
	ON_BN_CLICKED(IDC_RADIO_OPEN, OnRadioOpen)
	ON_BN_CLICKED(IDC_RADIO_CLOSE, OnRadioClose)
	ON_EN_CHANGE(IDC_EDIT_DILATE, OnChangeEditDilate)
	ON_EN_CHANGE(IDC_EDIT_ERODE, OnChangeEditErode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDialogSegmentation::Configure_Dialog( void )
{
	if( parent == nullptr ) return;

	CVisionBotDlg * vision_bot = (CVisionBotDlg *) parent;

	if( vision_bot->pThreadProc == nullptr ) return;
	if( vision_bot->pThreadProc->image_procObject == nullptr ) return;

	vision_bot->pThreadProc->image_procObject->vSegmentedImage.lock();

	size_t nColours = vision_bot->pThreadProc->image_procObject->vSegmentedImage.size();

	vSegImage.assign( nColours, nullptr );

	m_combo_colour.ResetContent();

	for( size_t nColour = 0; nColour<nColours; nColour++ )
	{
		vSegImage.at( nColour ) = &vision_bot->pThreadProc->image_procObject->vSegmentedImage.at( nColour );
		m_combo_colour.InsertString(
			static_cast<int>( nColour ),
			CA2T( vision_bot->pThreadProc->image_procObject->vSegmentedImage.at( nColour ).the_Colour.sName.c_str() )
			);
	}

	vision_bot->pThreadProc->image_procObject->vSegmentedImage.unlock();

	m_combo_colour.SetCurSel( nSelectedColour );

	SelectColour( nSelectedColour );
}

void CDialogSegmentation::OnNMCustomdrawSliderMaxHue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int pos = m_slider_hue_max.GetPos();
	vSegImage.at( nSelectedColour )->uHue[Seg::MAX].set( static_cast<unsigned int>( pos / 2 + 0.5f ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_hue_max.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}

void CDialogSegmentation::OnNMCustomdrawSliderMaxSat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	int pos = m_slider_sat_max.GetPos();
	vSegImage.at( nSelectedColour )->uSat[Seg::MAX].set( static_cast<unsigned int>( pos*2.55f + 0.5f ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_sat_max.SetWindowText( str_aux.str().c_str() );
	
	*pResult = 0;
}

void CDialogSegmentation::OnNMCustomdrawSliderMaxVal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
		
	int pos = m_slider_val_max.GetPos();
	vSegImage.at( nSelectedColour )->uVal[Seg::MAX].set( static_cast<unsigned int>( pos*2.55f + 0.5f ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Max: " << pos;

	m_static_val_max.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}

void CDialogSegmentation::OnNMCustomdrawSliderMinHue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	int pos = m_slider_hue_min.GetPos();
	vSegImage.at( nSelectedColour )->uHue[Seg::MIN].set( static_cast<unsigned int>( pos / 2 ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;
	
	m_static_hue_min.SetWindowText( str_aux.str().c_str() );

	*pResult = 0;
}

void CDialogSegmentation::OnNMCustomdrawSliderMinSat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int pos = m_slider_sat_min.GetPos();
	vSegImage.at( nSelectedColour )->uSat[Seg::MIN].set( static_cast<unsigned int>( pos*2.55f ) );

	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;

	m_static_sat_min.SetWindowText( str_aux.str().c_str() );
	
	*pResult = 0;
}

void CDialogSegmentation::OnNMCustomdrawSliderMinVal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int pos = m_slider_val_min.GetPos();
	vSegImage.at( nSelectedColour )->uVal[Seg::MIN].set( static_cast<unsigned int>( pos*2.55f ) );
	
	std::tstringstream str_aux( _T( "" ) );
	str_aux << "Min: " << pos;

	m_static_val_min.SetWindowText( str_aux.str().c_str() );
	
	*pResult = 0;
}

void CDialogSegmentation::SelectColour( int nColour )
{
	nSelectedColour = nColour;

	LoadColourParameters( nSelectedColour );
}

void CDialogSegmentation::LoadColourParameters( int nColour )
{
	unsigned int parameter = 0;
	float conv = 100.0f/255.0f;

	std::tstringstream str_aux;

	// Hue
	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uHue[Seg::MIN].get() * 2 + 0.5f );
	m_slider_hue_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_hue_min.SetWindowText( str_aux.str().c_str() );

	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uHue[Seg::MAX].get() * 2 + 0.5f );
	m_slider_hue_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_hue_max.SetWindowText( str_aux.str().c_str() );

	// Saturation
	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uSat[Seg::MIN].get()*conv + 0.5f );
	m_slider_sat_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_sat_min.SetWindowText( str_aux.str().c_str() );

	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uSat[Seg::MAX].get()*conv + 0.5f );
	m_slider_sat_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_sat_max.SetWindowText( str_aux.str().c_str() );

	// Value
	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uVal[Seg::MIN].get()*conv + 0.5f );
	m_slider_val_min.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Min: " << parameter;
	m_static_val_min.SetWindowText( str_aux.str().c_str() );

	parameter = static_cast<unsigned int>( vSegImage.at( nSelectedColour )->uVal[Seg::MAX].get()*conv + 0.5f );
	m_slider_val_max.SetPos( parameter );
	str_aux.str( _T( "" ) );
	str_aux << "Max: " << parameter;
	m_static_val_max.SetWindowText( str_aux.str().c_str() );

	//Morphologic operation
	parameter = vSegImage.at( nSelectedColour )->uMorph[Seg::OP].get();
	switch( parameter )
	{
	case MORPH_OPERATION_CLOSE:
		m_radio_close.SetCheck( BST_CHECKED );
		m_radio_open.SetCheck( BST_UNCHECKED );
		break;

	default:
	case MORPH_OPERATION_OPEN:
		m_radio_close.SetCheck( BST_UNCHECKED );
		m_radio_open.SetCheck( BST_CHECKED );
		break;
	}

	parameter = vSegImage.at( nSelectedColour )->uMorph[Seg::ERODE].get();
	m_spin_erode.SetPos( parameter );
	parameter = vSegImage.at( nSelectedColour )->uMorph[Seg::DILATE].get();
	m_spin_dilate.SetPos( parameter );
}

void CDialogSegmentation::OnSelchangeComboColour() 
{
	// TODO: Add your control notification handler code here
	nSelectedColour = m_combo_colour.GetCurSel();
	if( nSelectedColour<0 ) return;
	LoadColourParameters( nSelectedColour );
}

void CDialogSegmentation::OnButtonLoadFile() 
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
		vision_bot->pThreadProc->image_procObject->LoadSegmentationParameters( file_name ); 
		LoadColourParameters( nSelectedColour );	
	}
}

void CDialogSegmentation::OnButtonSaveFile() 
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
		vision_bot->pThreadProc->image_procObject->SaveSegmentationParameters( file_name ); 
	}
}

void CDialogSegmentation::OnRadioOpen() 
{
	// TODO: Add your control notification handler code here
	m_radio_open.SetCheck( BST_CHECKED );
	m_radio_close.SetCheck( BST_UNCHECKED );

	vSegImage.at( nSelectedColour )->uMorph[Seg::OP].set( MORPH_OPERATION_OPEN );
}

void CDialogSegmentation::OnRadioClose() 
{
	// TODO: Add your control notification handler code here
	m_radio_open.SetCheck( BST_UNCHECKED );
	m_radio_close.SetCheck( BST_CHECKED );

	vSegImage.at( nSelectedColour )->uMorph[Seg::OP].set( MORPH_OPERATION_CLOSE );
}

void CDialogSegmentation::OnChangeEditDilate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	if( m_spin_dilate.m_hWnd == nullptr ) return;

	unsigned int param = static_cast<unsigned int>( m_spin_dilate.GetPos() );
	vSegImage.at( nSelectedColour )->uMorph[Seg::DILATE].set( param );
}

void CDialogSegmentation::OnChangeEditErode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if( m_spin_erode.m_hWnd == nullptr ) return;

	unsigned int param = static_cast<unsigned int>( m_spin_erode.GetPos() );
	vSegImage.at( nSelectedColour )->uMorph[Seg::ERODE].set( param );
}
