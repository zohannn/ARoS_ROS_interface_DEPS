// PHRIDlg.cpp : implementation file
#include "stdafx.h"
#include "PHRI.h"
#include "PHRIDlg.h"
#include "Matrix.h"
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <time.h>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
	/*
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedOk();
	*/
public:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPHRIDlg dialog




CPHRIDlg::CPHRIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPHRIDlg::IDD, pParent)
	, YPHRI( "/aros/PHRI", "/aros/lwa7dof", false ),YVIS( "/aros/Vis","/aros/vision" , false ),YSPE("/aros/Spe", "/aros/audio", false ),YHAN("/aros/Han", "/aros/barretthand", false ),YVOI( "/aros/Voi", "/aros/speechrecognition", false ),YMOB( "/aros/Mob", "/Dumbo/Locomotion", false ),YOBS( "/aros/Obs", "/Dumbo/Obstacles", false ),ArosArm(),ArmThread()
	//, YPHRI( "/aros/PHRI", "/aros/lwa7dof", false ),YVIS( "/aros/Vis", "/dumbo/vision" "/aros/vision", false ),YSPE("/aros/Spe", "/aros/audio", false ),YHAN("/aros/Han", "/aros/barretthand", false ),YVOI( "/aros/Voi", "/aros/speechrecognition", false ),YMOB( "/aros/Mob", "/Dumbo/Locomotion", false ),ArosArm(),ArmThread()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//YComm = new CYarpCommPHRI( "/dumbo/PHRI", "/aros/lwa7dof", false );
	//system("start yarpserver");
	//system("start D:\\VMH\\Mechatronics\\UMinho\\Aros\\Amtec_Jumbo.bat" );
}

void CPHRIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BCC, BCC);
	DDX_Control(pDX, IDC_BCGH, BCGH);
	DDX_Control(pDX, IDC_BCGR, BGR);
	DDX_Control(pDX, IDC_BJMM, BJMM);
	DDX_Control(pDX, IDC_BAMM, BAMM);
	DDX_Control(pDX, IDC_EJ0, EJ0);
	DDX_Control(pDX, IDC_EJ1, EJ1);
	DDX_Control(pDX, IDC_EJ2, EJ2);
	DDX_Control(pDX, IDC_EJ3, EJ3);
	DDX_Control(pDX, IDC_EJ4, EJ4);
	DDX_Control(pDX, IDC_EJ5, EJ5);
	DDX_Control(pDX, IDC_EJ6, EJ6);
	DDX_Control(pDX, IDC_EJ7, EJ7);
	DDX_Control(pDX, IDC_EX, EX);
	DDX_Control(pDX, IDC_EY, EY);
	DDX_Control(pDX, IDC_EZ, EZ);
	DDX_Control(pDX, IDC_EA, EA);
	DDX_Control(pDX, IDC_EB, EB);
	DDX_Control(pDX, IDC_EC, EC);
	DDX_Control(pDX, IDC_CPA, CPA);
	DDX_Control(pDX, IDC_EPA, EPA);
	DDX_Control(pDX, IDC_SX, SX);
	DDX_Control(pDX, IDC_SY, SY);
	DDX_Control(pDX, IDC_SZ, SZ);
	DDX_Control(pDX, IDC_SA, SA);
	DDX_Control(pDX, IDC_SB, SB);
	DDX_Control(pDX, IDC_SC, SC);
	DDX_Control(pDX, IDC_SS, SS);
	DDX_Control(pDX, IDC_SOX, SOX);
	DDX_Control(pDX, IDC_SOY, SOY);
	DDX_Control(pDX, IDC_SOZ, SOZ);
	DDX_Control(pDX, IDC_SNO, SNO);
	DDX_Control(pDX, IDC_CVX, CVX);
	DDX_Control(pDX, IDC_CVY, CVY);
	DDX_Control(pDX, IDC_CVZ, CVZ);
	DDX_Control(pDX, IDC_CVA, CVA);
	DDX_Control(pDX, IDC_CVB, CVB);
	DDX_Control(pDX, IDC_CVC, CVC);
	DDX_Control(pDX, IDC_EVX, EVX);
	DDX_Control(pDX, IDC_EVY, EVY);
	DDX_Control(pDX, IDC_EVZ, EVZ);
	DDX_Control(pDX, IDC_EVA, EVA);
	DDX_Control(pDX, IDC_EVB, EVB);
	DDX_Control(pDX, IDC_EVC, EVC);
	DDX_Control(pDX, IDC_SJ0, SJ0);
	DDX_Control(pDX, IDC_SJ1, SJ1);
	DDX_Control(pDX, IDC_SJ2, SJ2);
	DDX_Control(pDX, IDC_SJ3, SJ3);
	DDX_Control(pDX, IDC_SJ4, SJ4);
	DDX_Control(pDX, IDC_SJ5, SJ5);
	DDX_Control(pDX, IDC_SJ6, SJ6);
	DDX_Control(pDX, IDC_SJ7, SJ7);
	DDX_Control(pDX, IDC_ET, ET);
	DDX_Control(pDX, IDC_BJMG, BJMG);
	DDX_Control(pDX, IDC_ST, ST);
	DDX_Control(pDX, IDC_BMV, BMV);
	DDX_Control(pDX, IDC_BTE, BTE);
	DDX_Control(pDX, IDC_CTASK, CTA);
	DDX_Control(pDX, IDC_SFX, SFX);
	DDX_Control(pDX, IDC_SFY, SFY);
	DDX_Control(pDX, IDC_SFZ, SFZ);
	DDX_Control(pDX, IDC_STX, STX);
	DDX_Control(pDX, IDC_STY, STY);
	DDX_Control(pDX, IDC_STZ, STZ);
	DDX_Control(pDX, IDC_BTC, BTC);
	DDX_Control(pDX, IDC_CHECK1, CJ0);
	DDX_Control(pDX, IDC_CHECK2, CJ1);
	DDX_Control(pDX, IDC_CHECK3, CJ2);
	DDX_Control(pDX, IDC_CHECK4, CJ3);
	DDX_Control(pDX, IDC_CHECK5, CJ4);
	DDX_Control(pDX, IDC_CHECK6, CJ5);
	DDX_Control(pDX, IDC_CHECK7, CJ6);
	DDX_Control(pDX, IDC_CHECK8, CJ7);
	DDX_Control(pDX, IDC_EX2, EX2);
	DDX_Control(pDX, IDC_EY2, EY2);
	DDX_Control(pDX, IDC_EZ2, EZ2);
	DDX_Control(pDX, IDC_EYAW, EYAW);
	DDX_Control(pDX, IDC_EPIT, EPIT);
	DDX_Control(pDX, IDC_EROL, EROL);
	DDX_Control(pDX, IDC_SYAW, SYAW);
	DDX_Control(pDX, IDC_SPIT, SPIT);
	DDX_Control(pDX, IDC_SROL, SROL);
	DDX_Control(pDX, IDC_EVM, EVM);
	DDX_Control(pDX, IDC_EWM, EWM);
	DDX_Control(pDX, IDC_BMM, BMM);
	DDX_Control(pDX, IDC_OBS1, SOBS1);
	DDX_Control(pDX, IDC_OBS2, SOBS2);
	DDX_Control(pDX, IDC_OBS3, SOBS3);
	DDX_Control(pDX, IDC_OBS4, SOBS4);
	DDX_Control(pDX, IDC_OBS5, SOBS5);

	DDX_Control(pDX, IDC_SVM, SVM);
	DDX_Control(pDX, IDC_SWM, SWM);
	DDX_Control(pDX, IDC_SXTAR, SXTAR);
	DDX_Control(pDX, IDC_SYTAR, SYTAR);
	DDX_Control(pDX, IDC_VDS0, SVDS0);
	DDX_Control(pDX, IDC_VDS1, SVDS1);
	DDX_Control(pDX, IDC_SZTAR, SZTAR);
	DDX_Control(pDX, IDC_SDTAR, SDTAR);
	DDX_Control(pDX, IDC_SSTAR, SSTAR);
	DDX_Control(pDX, IDC_LVES0, SLVES0);
	DDX_Control(pDX, IDC_LVES1, SLVES1);
	DDX_Control(pDX, IDC_SDT, SDT);
	DDX_Control(pDX, IDC_SXTARE, SXTARE);
	DDX_Control(pDX, IDC_SYTARE, SYTARE);
	DDX_Control(pDX, IDC_SZTARE, SZTARE);
}

BEGIN_MESSAGE_MAP(CPHRIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ABOUT_PHRI, &CPHRIDlg::OnAboutPhri)
	ON_BN_CLICKED(IDC_BCC, &CPHRIDlg::OnBnClickedBcc)
	ON_BN_CLICKED(IDC_BJMM, &CPHRIDlg::OnBnClickedBjmm)
	ON_BN_CLICKED(IDC_BCGH, &CPHRIDlg::OnBnClickedBcgh)
	ON_BN_CLICKED(IDC_BCGR, &CPHRIDlg::OnBnClickedBTest)
	ON_BN_CLICKED(IDC_CPA, &CPHRIDlg::OnBnClickedCpa)
	ON_BN_CLICKED(IDC_BAMM, &CPHRIDlg::OnBnClickedBamm)
	ON_BN_CLICKED(IDC_BMV, &CPHRIDlg::OnBnClickedBmv)
	ON_BN_CLICKED(IDC_BJMG, &CPHRIDlg::OnBnClickedBjmg)
	ON_COMMAND(ID_KINEMATICS_JOINTLIMITAVOIDANCE, &CPHRIDlg::OnKinematicsJointlimitavoidance)
	ON_COMMAND(ID_KINEMATICS_SINGULARITYAVOIDANCE, &CPHRIDlg::OnKinematicsSingularityavoidance)
	ON_COMMAND(ID_KINEMATICS_OBSTACLEAVOIDANCE, &CPHRIDlg::OnKinematicsObstacleavoidance)
	ON_COMMAND(ID_MODULES_VISION, &CPHRIDlg::OnModulesVision)
	ON_COMMAND(ID_MODULES_SPEECH, &CPHRIDlg::OnModulesSpeech)
	ON_WM_TIMER()
	ON_COMMAND(ID_MODULES_HAND, &CPHRIDlg::OnModulesHand)
	ON_BN_CLICKED(IDC_BTE, &CPHRIDlg::OnBnClickedBte)
	ON_BN_CLICKED(IDC_BTC, &CPHRIDlg::OnBnClickedBtc)
	ON_BN_CLICKED(IDC_BHMM, &CPHRIDlg::OnBnClickedBhmm)
	ON_COMMAND(ID_MODULES_VOICE, &CPHRIDlg::OnModulesVoice)
	ON_COMMAND(ID_MODULES_MOBILE, &CPHRIDlg::OnModulesMobile)
	ON_BN_CLICKED(IDC_BMM, &CPHRIDlg::OnBnClickedBmm)
	ON_COMMAND(ID_MODULES_LASERRANGEFINDER, &CPHRIDlg::OnModulesLaserrangefinder)
	ON_COMMAND(ID_CONFIGURATION_EXIT, &CPHRIDlg::OnConfigurationExit)
END_MESSAGE_MAP()
// CPHRIDlg message handlers

BOOL CPHRIDlg::OnInitDialog()
{ 
	CDialog::OnInitDialog();
    
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	
	menuPHRI=GetMenu();
	
	ASSERT(bitmap_Conf.LoadBitmapA(IDB_CONF));
	ASSERT(bitmap_Configuration.LoadBitmapA(IDB_CONFIGURATION));
	ASSERT(bitmap_Exit.LoadBitmapA(IDB_EXIT));
	ASSERT(bitmap_Save.LoadBitmapA(IDB_SAVE));
	ASSERT(bitmap_Load.LoadBitmapA(IDB_LOAD));
	ASSERT(bitmap_Help.LoadBitmapA(IDB_HELP));
	ASSERT(menuPHRI->SetMenuItemBitmaps(ID_ABOUT_PHRI,MF_BYCOMMAND,&bitmap_Help,&bitmap_Help));
	ASSERT(menuPHRI->SetMenuItemBitmaps(ID_CONFIGURATION_EXIT,MF_BYCOMMAND,&bitmap_Exit,&bitmap_Exit));
	ASSERT(menuPHRI->SetMenuItemBitmaps(ID_CONFIGURATION_LOAD,MF_BYCOMMAND,&bitmap_Load,&bitmap_Load));
	ASSERT(menuPHRI->SetMenuItemBitmaps(ID_CONFIGURATION_SAVE,MF_BYCOMMAND,&bitmap_Save,&bitmap_Save));
	
	// TODO: Add extra initialization here
	SS.SetWindowTextA("");//Clear status
	//Set how digits are displayed
	str_text.precision(2);
	str_text.setf(std::ios_base::fixed , std::ios_base::floatfield);
	//---------Assign modules for the execute thread-----------------	
	// Pan-tilt modules is controlled through Vision modules	
	ArmThread.YPHRI=&(this->YPHRI);
	ArmThread.YMOB=&(this->YMOB);
	ArmThread.YOBS=&(this->YOBS);
	ArmThread.YSPE=&(this->YSPE);
	ArmThread.YVIS=&(this->YVIS);
	ArmThread.HandThread.mod_type=1;
	ArmThread.HandThread.YMOD=&(this->YHAN);	
	ArmThread.ArosArm=&(this->ArosArm);
	//----------------
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPHRIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CPHRIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPHRIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CPHRIDlg::OnAboutPhri()
{
	// Open About dialog
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//-------------------Internal function
CPHRIDlg::~CPHRIDlg()
{
	if(YOBS.connected==1)
	{
		YOBS.Send(RANGE_FINDER_STOP);
	}

	if( YPHRI.isOpened() )
		YPHRI.close();

	if( YVIS.isOpened() )
		YVIS.close();

	if( YSPE.isOpened() )
		YSPE.close();

	if( YHAN.isOpened() )
		YHAN.close();

	if( YVOI.isOpened() )
		YVOI.close();

	if( YMOB.isOpened() )
		YMOB.close();

	if( YOBS.isOpened() )
		YOBS.close();

	yarp_net.fini();
}
void CPHRIDlg::prepareEditfield()
{
	EJ0.SetWindowTextA("0");
	EJ1.SetWindowTextA("0");
	EJ2.SetWindowTextA("0");
	EJ3.SetWindowTextA("0");
	EJ4.SetWindowTextA("0");
	EJ5.SetWindowTextA("0");
	EJ6.SetWindowTextA("0");
	EPA.SetWindowTextA("0");
	CPA.SetCheck(BST_CHECKED);
	ET.SetWindowTextA("10000");
	CVX.SetCheck(BST_CHECKED);
	CVY.SetCheck(BST_CHECKED);
	CVZ.SetCheck(BST_CHECKED);
	CVA.SetCheck(BST_UNCHECKED);
	CVB.SetCheck(BST_UNCHECKED);
	CVC.SetCheck(BST_UNCHECKED);
	EVX.SetWindowTextA("0");
	EVY.SetWindowTextA("0");
	EVZ.SetWindowTextA("0");
	EVA.SetWindowTextA("0");
	EVB.SetWindowTextA("0");
	EVC.SetWindowTextA("0");	
}
void CPHRIDlg::updateJValue(float *jtheta)
{	
	str_text.str("");
	str_text<<jtheta[0]*180/M_PI;
	SJ0.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[1]*180/M_PI;
	SJ1.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[2]*180/M_PI;
	SJ2.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[3]*180/M_PI;
	SJ3.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[4]*180/M_PI;
	SJ4.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[5]*180/M_PI;
	SJ5.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[6]*180/M_PI;
	SJ6.SetWindowTextA(str_text.str().c_str());		
}
void CPHRIDlg::updateLaserObstacle(float *obs)
{	
	str_text.str("");
	str_text<<obs[10];
	SOBS1.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<obs[31];
	SOBS2.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<obs[42];
	SOBS3.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<obs[52];
	SOBS4.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<obs[73];
	SOBS5.SetWindowTextA(str_text.str().c_str());
	
}
void CPHRIDlg::updateJValueD(float *jtheta)
{	
	str_text.str("");
	str_text<<jtheta[0]*180/M_PI;
	EJ0.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[1]*180/M_PI;
	EJ1.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[2]*180/M_PI;
	EJ2.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[3]*180/M_PI;
	EJ3.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[4]*180/M_PI;
	EJ4.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[5]*180/M_PI;
	EJ5.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<jtheta[6]*180/M_PI;
	EJ6.SetWindowTextA(str_text.str().c_str());		
}
void CPHRIDlg::updateInformation(float *jtheta)
{
	float pos[6],ypr[3];
	ArosArm.GeoKine(jtheta,pos,ypr);
	str_text.str("");
	str_text<<pos[0];
	SX.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<pos[1];
	SY.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<pos[2];
	SZ.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<pos[3]*180/M_PI;
	SA.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<pos[4]*180/M_PI;
	SB.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<pos[5]*180/M_PI;
	SC.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ypr[0]*180/M_PI;
	SYAW.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ypr[1]*180/M_PI;
	SPIT.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ypr[2]*180/M_PI;
	SROL.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArmThread.dmob[0]*1000;
	SVM.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArmThread.dmob[1]*180/M_PI;
	SWM.SetWindowTextA(str_text.str().c_str());	
}
void CPHRIDlg::updateInformation()
{	
	str_text.str("");
	str_text<<ArmThread.dmob[0]*1000;
	SVM.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArmThread.dmob[1]*180/M_PI;
	SWM.SetWindowTextA(str_text.str().c_str());
	
	str_text.str("");
	str_text<<ArosArm.L_ves[0]*180/M_PI;
	SLVES0.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArosArm.L_ves[1];
	SLVES1.SetWindowTextA(str_text.str().c_str());

	str_text.str("");
	str_text<<ArosArm.M_ds;//*180/M_PI;
	SVDS0.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArosArm.F_ds;	
	SVDS1.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArmThread.dtt;
	SDT.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArosArm.targetp[0];
	SXTARE.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArosArm.targetp[1];
	SYTARE.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<ArosArm.targetp[2];
	SZTARE.SetWindowTextA(str_text.str().c_str());	
}
void CPHRIDlg::updateGriper(float grip)
{
	str_text.str("");
	str_text<<grip;
	SJ7.SetWindowTextA(str_text.str().c_str());
	EJ7.SetWindowTextA(str_text.str().c_str());
}
void CPHRIDlg::readJValueD(float *jtheta)
{
	int buf_size;
	char *buf;
	float data;
	buf_size = EJ0.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ0.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[0]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ1.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ1.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[1]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ2.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ2.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[2]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ3.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ3.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[3]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ4.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ4.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[4]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ5.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ5.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[5]=data/180*M_PI;
	delete[] buf;
	buf_size = EJ6.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ6.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	jtheta[6]=data/180*M_PI;
	delete[] buf;
	buf = NULL;	
}
void CPHRIDlg::readTime(int *time)
{
	int buf_size;
	char *buf;
	float data;
	buf_size = ET.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	ET.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	time[0]=static_cast<int>( data );
	delete[] buf;
	buf = NULL;
}
void CPHRIDlg::readPValueD(float *pos)
{
	int buf_size;
	char *buf;
	float data;
	buf_size = EX.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EX.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[0]=data;
	delete[] buf;
	buf_size = EY.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EY.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[1]=data;
	delete[] buf;
	buf_size = EZ.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EZ.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[2]=data;
	delete[] buf;
	buf_size = EA.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EA.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[3]=data/180*M_PI;
	delete[] buf;
	buf_size = EB.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EB.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[4]=data/180*M_PI;
	delete[] buf;
	buf_size = EC.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EC.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[5]=data/180*M_PI;
	delete[] buf;
	buf = NULL;
}
void CPHRIDlg::readRPY(float *pos)
{
	int buf_size;
	char *buf;
	float data;
	buf_size = EX2.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EX2.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[0]=data;
	delete[] buf;
	buf_size = EY2.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EY2.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[1]=data;
	delete[] buf;
	buf_size = EZ2.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EZ2.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[2]=data;
	delete[] buf;
	buf_size = EYAW.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EYAW.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[3]=data/180*M_PI;
	delete[] buf;
	buf_size = EPIT.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EPIT.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[4]=data/180*M_PI;
	delete[] buf;
	buf_size = EROL.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EROL.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &data );
	pos[5]=data/180*M_PI;
	delete[] buf;
	buf = NULL;
}
void CPHRIDlg::readGripper(float *gripper)
{
	int buf_size;
	char *buf;
	float grip;
	buf_size = EJ7.GetWindowTextLengthA()+ 1;
	buf= new char[buf_size];
	EJ7.GetWindowTextA(buf,buf_size);
	sscanf( buf, "%f", &grip );
	gripper[0]=grip;
	delete[] buf;
	buf = NULL;
}
void CPHRIDlg::updateForceWrist(float *fW)
{
	str_text.str("");
	str_text<<"FX: ";  
	str_text<<fW[0];
	SFX.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<"FY: ";
	str_text<<fW[1];
	SFY.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<"FZ: ";
	str_text<<fW[2];
	SFZ.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<"TX: ";
	str_text<<fW[3];
	STX.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<"TY: ";
	str_text<<fW[4];
	STY.SetWindowTextA(str_text.str().c_str());
	str_text.str("");
	str_text<<"TZ: ";
	str_text<<fW[5];
	STZ.SetWindowTextA(str_text.str().c_str());
}
//---------------------------------------
void CPHRIDlg::OnBnClickedBcc()
{
  // Configuration CONNECT button pressed		
  if(YPHRI.connected)
  {
	BCC.SetWindowTextA("Connect");	
	YPHRI.connected=0;	
	YPHRI.close();
	SS.SetWindowTextA("Arm: Disconnected!");	
  }else
  {
    //float jtheta;
	BCC.SetWindowTextA("Disconnect");
	YPHRI.connected=1;	
	if(YPHRI.open())
	{		
		SS.SetWindowTextA("Arm: Connected successfully!");
		YPHRI.Send(AMTEC_GET_POS_ALL);		
		updateJValue(YPHRI.jtheta);
		prepareEditfield();		
		updateInformation(YPHRI.jtheta);
		YPHRI.Send(AMTEC_GET_GRIPPER);
		updateGriper(YPHRI.gripper);		
	}else
	{
		SS.SetWindowTextA("Arm: Failed to connect!");
	}	
  } 
}
void CPHRIDlg::OnBnClickedBcgh()
{
	// Configuration GO HOME button pressed
	//Be carefull, all joints will be set to zeros simultaneously
	if(YPHRI.connected)
	{		
		YPHRI.msg.uCommand=AMTEC_MOVE_STEP_SAME_TIME_WAIT;
		YPHRI.msg.uParam.resize(0);
		YPHRI.msg.fData.resize(8);
		for(int i=0;i<7;i++)
		{
		YPHRI.msg.fData[i]=0;
		}
		int time;
		readTime(&time);
		YPHRI.msg.fData[7]=static_cast<float>( time );
		YPHRI.Send(YPHRI.msg);
		updateJValue(YPHRI.jtheta);
		updateInformation(YPHRI.jtheta);						
	}
}

void CPHRIDlg::OnBnClickedBTest()
{
	//Close hand
	YHAN.Send(BARRETTHAND_TORQUECLOSE); 
}

void CPHRIDlg::OnBnClickedBjmm()
{
	//Joint movement MOVE button pressed
	if(YPHRI.connected)
	{
		float jtheta[7],flag[8],grip;
		int count=-1;
		for(int i=0;i<8;i++) flag[i]=0;
		if(CJ0.GetCheck()==BST_CHECKED) {
			flag[0]=1;
			count=0;
		}
		if(CJ1.GetCheck()==BST_CHECKED) {
			flag[1]=1;
			count=1;
		}
		if(CJ2.GetCheck()==BST_CHECKED) {
			flag[2]=1;
			count=2;
		}
		if(CJ3.GetCheck()==BST_CHECKED) {
			flag[3]=1;
			count=3;
		}
		if(CJ4.GetCheck()==BST_CHECKED) {
			flag[4]=1;
			count=4;
		}
		if(CJ5.GetCheck()==BST_CHECKED) {
			flag[5]=1;
			count=5;
		}
		if(CJ6.GetCheck()==BST_CHECKED) {
			flag[6]=1;
			count=6;
		}
		if(CJ7.GetCheck()==BST_CHECKED) {
			flag[7]=1;
			count=7;
		}
		readJValueD(jtheta);
		int time;
		readTime(&time);		
		for(int i=0;i<8;i++)
		{
			if((i==7)&&(flag[7]==1))
			{				
				readGripper(&grip);
				if((grip>=0)&&(grip<0.06))
				{
				YPHRI.msg.uCommand=AMTEC_CLOSEGRIPPER;
				YPHRI.msg.uParam.resize(0);
				YPHRI.msg.fData.resize(1);
				YPHRI.msg.fData[0]=grip;
				YPHRI.Send(YPHRI.msg);
				YPHRI.Send(AMTEC_GET_GRIPPER);
				updateGriper(YPHRI.gripper);
				}
				continue;
			}
			if(i==count)
			{
				YPHRI.msg.uCommand=AMTEC_MOVE_STEP_WAIT;
				YPHRI.msg.uParam.resize(0);
				YPHRI.msg.fData.resize(3);
				YPHRI.msg.fData[0]=static_cast<float>( i );
				YPHRI.msg.fData[1]=jtheta[i];
				YPHRI.msg.fData[2] = static_cast<float>( time );
				YPHRI.Send(YPHRI.msg);
				continue;
			}
			if(flag[i]==1)
			{
				YPHRI.msg.uCommand=AMTEC_MOVE_STEP;
				YPHRI.msg.uParam.resize(0);
				YPHRI.msg.fData.resize(3);
				YPHRI.msg.fData[0]=static_cast<float>( i );
				YPHRI.msg.fData[1]=jtheta[i];
				YPHRI.msg.fData[2]=static_cast<float>( time );
				YPHRI.Send(YPHRI.msg);
			}
			
		}		
		YPHRI.Send(AMTEC_GET_POS_ALL);
		updateJValue(YPHRI.jtheta);
		updateInformation(YPHRI.jtheta);		
	}
}
void CPHRIDlg::OnBnClickedCpa()
{
	// Using the Arm angle or not
	if(CPA.GetCheck()==BST_CHECKED)
	{
		EPA.EnableWindow(1);		
	}else
	{
		EPA.SetWindowTextA("0");
		EPA.EnableWindow(0);
	}		
}

void CPHRIDlg::OnBnClickedBamm()
{	
	// Arm movement MOVE button clicked
	float pos[6],jtheta[7],plane_angle;
	int buf_size;
	char *buf;
	int flag_OK;//YPHRI.connected
	if(YPHRI.connected)
	{		
		readPValueD(pos);
		if(CPA.GetCheck()==BST_CHECKED)
		{			
			buf_size = EPA.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EPA.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &plane_angle);
			plane_angle=plane_angle*M_PI/180;
			delete[] buf;
			buf=NULL;
			flag_OK=ArosArm.InvGeoKine(pos,jtheta,&plane_angle,0,0);
		}else
		{
			flag_OK=ArosArm.InvGeoKine(pos,jtheta,&plane_angle,1,0);
		};
		if(flag_OK==1)
		{
			updateJValueD(jtheta);
			SS.SetWindowTextA("Arm: Let's move!");
			YPHRI.msg.uCommand=AMTEC_MOVE_STEP_SAME_TIME_WAIT;
			YPHRI.msg.uParam.resize(0);
			YPHRI.msg.fData.resize(8);
			for(int i=0;i<7;i++)
			{
				YPHRI.msg.fData[i]=jtheta[i];
			}
			int time;
			readTime(&time);
			YPHRI.msg.fData[7]=static_cast<float>( time );
			YPHRI.Send(YPHRI.msg);
			YPHRI.Send(AMTEC_GET_POS_ALL);
			updateJValue(YPHRI.jtheta);
			updateInformation(YPHRI.jtheta);
			
		}else
		{
			// Can not move to that position
			SS.SetWindowTextA("Arm: Can not go there!");
		};		
	}	
}
//--------------------Menu events
void CPHRIDlg::OnKinematicsJointlimitavoidance()
{
	// Enable/Disable Joint limit avoidance behaviour
	UINT state = menuPHRI->GetMenuState(ID_KINEMATICS_JOINTLIMITAVOIDANCE, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	if (state & MF_CHECKED)
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_JOINTLIMITAVOIDANCE, MF_UNCHECKED | MF_BYCOMMAND);
		ArosArm.jlimEnable=0;
	}
    else
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_JOINTLIMITAVOIDANCE, MF_CHECKED | MF_BYCOMMAND);
		ArosArm.jlimEnable=1;	
	}
}

void CPHRIDlg::OnKinematicsObstacleavoidance()
{
	// Enable/Disable Obstacle avoidance behaviour
	UINT state = menuPHRI->GetMenuState(ID_KINEMATICS_OBSTACLEAVOIDANCE, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	if (state & MF_CHECKED)
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_OBSTACLEAVOIDANCE, MF_UNCHECKED | MF_BYCOMMAND);
		ArosArm.obsEnable=0;
	}
    else
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_OBSTACLEAVOIDANCE, MF_CHECKED | MF_BYCOMMAND);
		ArosArm.obsEnable=1;
	}
	  
	
}

void CPHRIDlg::OnKinematicsSingularityavoidance()
{
	// Enable/Disable Singularity avoidance behaviour

	UINT state = menuPHRI->GetMenuState(ID_KINEMATICS_SINGULARITYAVOIDANCE, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	if (state & MF_CHECKED)
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_SINGULARITYAVOIDANCE, MF_UNCHECKED | MF_BYCOMMAND);
		ArosArm.singEnable=0;
	}
    else
	{
		menuPHRI->CheckMenuItem(ID_KINEMATICS_SINGULARITYAVOIDANCE, MF_CHECKED | MF_BYCOMMAND);
		ArosArm.singEnable=1;
	}
}

void CPHRIDlg::OnModulesVision()
{
	//Load Vision modules	
  if(YVIS.connected)
  {	
	YVIS.close();
	SS.SetWindowTextA("Vision: Disconnected!");	
	UINT state = menuPHRI->GetMenuState(ID_MODULES_VISION, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	if (state & MF_CHECKED)
		menuPHRI->CheckMenuItem(ID_MODULES_VISION, MF_UNCHECKED | MF_BYCOMMAND);
    else
      menuPHRI->CheckMenuItem(ID_MODULES_VISION, MF_CHECKED | MF_BYCOMMAND);
	KillTimer(Timer_Vision); 
	YVIS.connected=0;
  }else
  {    	
	if(YVIS.open())
	{		
		YVIS.connected=1;	
		SS.SetWindowTextA("Vision: Connected successfully!");
		UINT state = menuPHRI->GetMenuState(ID_MODULES_VISION, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
		  menuPHRI->CheckMenuItem(ID_MODULES_VISION, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_VISION, MF_CHECKED | MF_BYCOMMAND);
		ArosArm.start=GetTickCount();
		time_p=GetTickCount()/1000.0f;
		SetTimer(Timer_Vision,53,NULL);
	}else
	{
		SS.SetWindowTextA("Vision: Failed to connect!");		
	}	
  } 
}

void CPHRIDlg::OnModulesSpeech()
{
	//Load Speech modules
	if(YSPE.connected)
	{	
		SS.SetWindowTextA("Audio: Disconnected!");
		YSPE.msg.uCommand=AUDIO_MESSAGE;
		YSPE.msg.strText="Audio Disconnected";	
		YSPE.Send(YSPE.msg,true);
		YSPE.close();			
		UINT state = menuPHRI->GetMenuState(ID_MODULES_SPEECH, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
			menuPHRI->CheckMenuItem(ID_MODULES_SPEECH, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_SPEECH, MF_CHECKED | MF_BYCOMMAND);
		YSPE.connected=0;
	}else
	{    		
		if(YSPE.open())
		{		
			YSPE.connected=1;
			SS.SetWindowTextA("Audio: Connected successfully!");
			YSPE.msg.uCommand=AUDIO_MESSAGE;
			YSPE.msg.strText="Audio Connected successfully";	
			YSPE.Send(YSPE.msg);
			UINT state = menuPHRI->GetMenuState(ID_MODULES_SPEECH, MF_BYCOMMAND);
			ASSERT(state != 0xFFFFFFFF);
			if (state & MF_CHECKED)
			  menuPHRI->CheckMenuItem(ID_MODULES_SPEECH, MF_UNCHECKED | MF_BYCOMMAND);
			else
			  menuPHRI->CheckMenuItem(ID_MODULES_SPEECH, MF_CHECKED | MF_BYCOMMAND);

		}else
		{
			SS.SetWindowTextA("Audio: Failed to connect!");		
		}	
	 } 
}
void CPHRIDlg::OnModulesHand()
{	
	//Load Hand modules
	if(YHAN.connected)
	{	
		SS.SetWindowTextA("Hand: Disconnected!");		
		YHAN.close();			
		UINT state = menuPHRI->GetMenuState(ID_MODULES_HAND, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
			menuPHRI->CheckMenuItem(ID_MODULES_HAND, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_HAND, MF_CHECKED | MF_BYCOMMAND);
		YHAN.connected=0;
	}else
	{    		
		if(YHAN.open())
		{		
			YHAN.connected=1;
			SS.SetWindowTextA("Hand: Connected successfully!");
			
			YHAN.msg.uCommand=BARRETTHAND_SETOPENTARGET;
			YHAN.msg.uParam.resize(0);
			YHAN.msg.fData.resize(4);
			YHAN.msg.fData[0]=0.6857f;
			YHAN.msg.fData[1]=30.0f;
			YHAN.msg.fData[2]=30.0f;
			YHAN.msg.fData[3]=30.0f;
			YHAN.Send(YHAN.msg);	
			
			YHAN.msg.uCommand=BARRETTHAND_MOVEFINGERS;
			YHAN.msg.uParam.resize(0);
			YHAN.msg.fData.resize(1);
			YHAN.msg.fData[0]=40;			
			YHAN.Send(YHAN.msg);			
			/*
			if(YHAN.jpos[1]>YHAN.jpos0[1]+5)
			{
				YHAN.Send(BARRETTHAND_OPEN);
			}	
			*/
			UINT state = menuPHRI->GetMenuState(ID_MODULES_HAND, MF_BYCOMMAND);
			ASSERT(state != 0xFFFFFFFF);
			if (state & MF_CHECKED)
			  menuPHRI->CheckMenuItem(ID_MODULES_HAND, MF_UNCHECKED | MF_BYCOMMAND);
			else
			  menuPHRI->CheckMenuItem(ID_MODULES_HAND, MF_CHECKED | MF_BYCOMMAND);
		}else
		{
			SS.SetWindowTextA("Hand: Failed to connect!");		
		}	
	 }
}
void CPHRIDlg::OnModulesVoice()
{
	//Load Voice modules
	if(YVOI.connected)
	{	
		SS.SetWindowTextA("Voice: Disconnected!");		
		YVOI.close();			
		UINT state = menuPHRI->GetMenuState(ID_MODULES_VOICE, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
			menuPHRI->CheckMenuItem(ID_MODULES_VOICE, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_VOICE, MF_CHECKED | MF_BYCOMMAND);
		YVOI.connected=0;
	}else
	{    		
		if(YVOI.open())
		{		
			YVOI.connected=1;
			SS.SetWindowTextA("Voice: Connected successfully!");			

			UINT state = menuPHRI->GetMenuState(ID_MODULES_VOICE, MF_BYCOMMAND);
			ASSERT(state != 0xFFFFFFFF);
			if (state & MF_CHECKED)
			  menuPHRI->CheckMenuItem(ID_MODULES_VOICE, MF_UNCHECKED | MF_BYCOMMAND);
			else
			  menuPHRI->CheckMenuItem(ID_MODULES_VOICE, MF_CHECKED | MF_BYCOMMAND);
		}else
		{
			SS.SetWindowTextA("Voice: Failed to connect!");		
		}	
	 }
}
void CPHRIDlg::OnModulesMobile()
{
	//Load Mobile platform modules
	if(YMOB.connected)
	{	
		SS.SetWindowTextA("Mobile: Disconnected!");		
		YMOB.close();			
		UINT state = menuPHRI->GetMenuState(ID_MODULES_MOBILE, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
			menuPHRI->CheckMenuItem(ID_MODULES_MOBILE, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_MOBILE, MF_CHECKED | MF_BYCOMMAND);
		YMOB.connected=0;
	}else
	{		
		if(YMOB.open())
		{		
			YMOB.connected=1;
			SS.SetWindowTextA("Mobile: Connected successfully!");			

			UINT state = menuPHRI->GetMenuState(ID_MODULES_MOBILE, MF_BYCOMMAND);
			ASSERT(state != 0xFFFFFFFF);
			if (state & MF_CHECKED)
			  menuPHRI->CheckMenuItem(ID_MODULES_MOBILE, MF_UNCHECKED | MF_BYCOMMAND);
			else
			  menuPHRI->CheckMenuItem(ID_MODULES_MOBILE, MF_CHECKED | MF_BYCOMMAND);
			EVM.SetWindowTextA("0");
			EWM.SetWindowTextA("0");
		}else
		{
			SS.SetWindowTextA("Mobile: Failed to connect!");		
		}	
	 }
}
void CPHRIDlg::OnModulesLaserrangefinder()
{
	//Load Laser range finder
	if(YOBS.connected)
	{	
		SS.SetWindowTextA("Laser: Disconnected!");		
		YOBS.close();	
		YOBS.Send(RANGE_FINDER_STOP);
		UINT state = menuPHRI->GetMenuState(ID_MODULES_LASERRANGEFINDER, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
			menuPHRI->CheckMenuItem(ID_MODULES_LASERRANGEFINDER, MF_UNCHECKED | MF_BYCOMMAND);
		else
		  menuPHRI->CheckMenuItem(ID_MODULES_LASERRANGEFINDER, MF_CHECKED | MF_BYCOMMAND);
		YOBS.connected=0;
		ArosArm.avoidobs=0;
	}else
	{  		
		if(YOBS.open())
		{		
			YOBS.connected=1;
			SS.SetWindowTextA("Laser: Connected successfully!");			
			
			UINT state = menuPHRI->GetMenuState(ID_MODULES_LASERRANGEFINDER, MF_BYCOMMAND);
			ASSERT(state != 0xFFFFFFFF);
			if (state & MF_CHECKED)
			  menuPHRI->CheckMenuItem(ID_MODULES_LASERRANGEFINDER, MF_UNCHECKED | MF_BYCOMMAND);
			else
			  menuPHRI->CheckMenuItem(ID_MODULES_LASERRANGEFINDER, MF_CHECKED | MF_BYCOMMAND);
			//Configuration
			YOBS.msg.uCommand=RANGE_FINDER_SETUP_STEP;
			YOBS.msg.uParam.resize(3);
			YOBS.msg.fData.resize(0);
			YOBS.msg.uParam[0]=45;
			YOBS.msg.uParam[1]=725;
			YOBS.msg.uParam[2]=8;//85 sector
			YOBS.Send(YOBS.msg);
			YOBS.Send(RANGE_FINDER_START_CONTINUOUS_SCAN);
			ArosArm.avoidobs=1;
		}else
		{
			SS.SetWindowTextA("Laser: Failed to connect!");	
			ArosArm.avoidobs=0;
		}	
	 }
}
//-------------------------------End Menu events----------------------
void CPHRIDlg::OnBnClickedBmv()
{//Move with desire velocity
	if(!ArmThread.started)	{		
		
		int buf_size;
		char *buf;
		float data;
		//Mask velocity components that doesn't need to be controlled		
		int flag=0;
		if(CVX.GetCheck()==BST_CHECKED)
		{			
			ArmThread.mask[0]=1;
			buf_size = EVX.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVX.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[0]=data;
			if(abs(data)>1) flag=1;		
			delete[] buf;			
		}else
		{
			ArmThread.mask[0]=0;
		};
		if(CVY.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[1]=1;
			buf_size = EVY.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVY.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[1]=data;
			if(abs(data)>1) flag=1;			
			delete[] buf;			
		}else
		{
			ArmThread.mask[1]=0;
		};
		if(CVZ.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[2]=1;
			buf_size = EVZ.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVZ.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[2]=data;
			if(abs(data)>1) flag=1;			
			delete[] buf;
		}else
		{
			ArmThread.mask[2]=0;
		};
		if(CVA.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[3]=1;
			buf_size = EVA.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVA.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[3]=data/180*M_PI;
			if(abs(data)>1) flag=1;			
			delete[] buf;
		}else
		{
			ArmThread.mask[3]=0;
		};
		if(CVB.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[4]=1;
			buf_size = EVB.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVB.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[4]=data/180*M_PI;
			if(abs(data)>1) flag=1;			
			delete[] buf;
		}else
		{
			ArmThread.mask[4]=0;
		};
		if(CVC.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[5]=1;
			buf_size = EVC.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVC.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[5]=data/180*M_PI;
			if(abs(data)>1) flag=1;			
			delete[] buf;
		}else
		{
			ArmThread.mask[5]=0;
		};
		buf = NULL;	
		
		ArmThread.mode=0;//Move with a desired velocity
		ArosArm.flag_ypr=0;// 1: Move with Roll pitch yaw velocity		
		if(flag==1)
		{		
			ArmThread.start();
			BMV.SetWindowTextA("Stop");			
			SetTimer(Timer_Arm,100,NULL);
		}else
		{
			SS.SetWindowTextA("Arm: doesn't want to move");			
		};		
	}else
	{
		ArmThread.stop=1;		
	}	
}
void CPHRIDlg::OnBnClickedBjmg()
{
	// Move gripper
	if(YPHRI.connected)
	{
		float grip;
		readGripper(&grip);
		if((grip>=0)&&(grip<0.06))
		{
		YPHRI.msg.uCommand=AMTEC_CLOSEGRIPPER;
		YPHRI.msg.uParam.resize(0);
		YPHRI.msg.fData.resize(1);
		YPHRI.msg.fData[0]=grip;
		YPHRI.Send(YPHRI.msg);
		YPHRI.Send(AMTEC_GET_GRIPPER);
		updateGriper(YPHRI.gripper);
		}
	}
}
void CPHRIDlg::OnBnClickedBhmm()
{
	// Move to desired position with Roll pitch Yaw
	float pos[6],jtheta[7],plane_angle;
	int buf_size;
	char *buf;
	int flag_OK;
	if(YPHRI.connected)
	{		
		readRPY(pos);
		if(CPA.GetCheck()==BST_CHECKED)
		{			
			buf_size = EPA.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EPA.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &plane_angle);
			plane_angle=plane_angle*M_PI/180;
			delete[] buf;
			buf=NULL;
			flag_OK=ArosArm.InvGeoKine(pos,jtheta,&plane_angle,0,1);
		}else
		{
			flag_OK=ArosArm.InvGeoKine(pos,jtheta,&plane_angle,1,1);
		};
		if(flag_OK==1)
		{
			updateJValueD(jtheta);
			SS.SetWindowTextA("Arm: Let's move!");
			YPHRI.msg.uCommand=AMTEC_MOVE_STEP_SAME_TIME_WAIT;
			//YPHRI.msg.uCommand=AMTEC_MOVE_STEP_ALL_SAME_TIME;
			YPHRI.msg.uParam.resize(0);
			YPHRI.msg.fData.resize(8);
			for(int i=0;i<7;i++)
			{
				YPHRI.msg.fData[i]=jtheta[i];
			}
			int time;
			readTime(&time);
			YPHRI.msg.fData[7]=static_cast<float>( time );
			YPHRI.Send(YPHRI.msg);
			YPHRI.Send(AMTEC_GET_POS_ALL);
			updateJValue(YPHRI.jtheta);
			updateInformation(YPHRI.jtheta);
			
		}else
		{
			// Can not move to that position
			SS.SetWindowTextA("Arm: Can not go there!");
		};		
	}	
}
void CPHRIDlg::OnBnClickedBmm()
{
	// Move mobile platform
	if(!ArmThread.started)
	{		
		//int count;		
		int buf_size;
		char *buf;
		float data;		
		buf_size = EVM.GetWindowTextLengthA()+ 1;
		buf= new char[buf_size];
		EVM.GetWindowTextA(buf,buf_size);
		sscanf( buf, "%f", &data );
		ArmThread.dmob[0]=data/1000;
		delete[] buf;
		buf_size = EWM.GetWindowTextLengthA()+ 1;
		buf= new char[buf_size];
		EWM.GetWindowTextA(buf,buf_size);
		sscanf( buf, "%f", &data );
		ArmThread.dmob[1]=data*M_PI/180;
		delete[] buf;		
		buf = NULL;			
		ArmThread.mode=8;//Move mobile platform
		if(YMOB.connected==1)
		{		
			ArmThread.start();
			BMM.SetWindowTextA("Stop");			
			
		}else
		{
			SS.SetWindowTextA("Platform: doesn't want to move");			
		};		
	}else
	{
		ArmThread.stop=1;	
		BMM.SetWindowTextA("Mobile");	
	}		

}
void CPHRIDlg::searchObstacle(int *objectType, int nType)
{
	YVIS.refresh=0;
	YVIS.obs_num=0;
	YVIS.mode=1; // Search obstacles
	for(int i=0;i<nType;i++)
	{
		YVIS.object_type=objectType[i];
		YVIS.msg.uCommand=VISION_BOT_GET_POSITION_OBJECT_TYPE;
		YVIS.msg.uParam.resize(1);
		YVIS.msg.fData.resize(0);
		YVIS.msg.uParam[0]=objectType[i];
		YVIS.Send(YVIS.msg);
	}
	for(int i=0;i<YVIS.obs_num;i++)
	{
		ArosArm.obs_array[i][0]=YVIS.obs[i][0];
		ArosArm.obs_array[i][1]=YVIS.obs[i][1];
		ArosArm.obs_array[i][2]=YVIS.obs[i][2];
		ArosArm.obs_array[i][3]=YVIS.obs[i][3];
		ArosArm.obs_f[i]=YVIS.obs_f[i];
	}
	ArosArm.obs_num=YVIS.obs_num;	
}
void CPHRIDlg::searchHandle(int objectType)
{
	ArosArm.see_target=0;
	YVIS.mode=2;// Search handle
	YVIS.object_type=objectType;

	YVIS.msg.uCommand=VISION_BOT_GET_POSITION_OBJECT_TYPE;
	
	YVIS.msg.uParam.resize(1);
	YVIS.msg.fData.resize(0);
	YVIS.msg.uParam[0]=objectType;
	YVIS.Send(YVIS.msg);
	if(YVIS.tar_num>0)
	{
		ArosArm.see_target=1;
		ArosArm.target[0]=YVIS.target[0][0];
		ArosArm.target[1]=YVIS.target[0][1];
		ArosArm.target[2]=YVIS.target[0][2];		
	}			
}
void CPHRIDlg::searchTarget(int objectType)
{
	ArmThread.see_tar=0;
	YVIS.mode=2;
	YVIS.object_type=objectType;
	
	YVIS.msg.uCommand=VISION_BOT_GET_POSITION_OBJECT_TYPE;
	YVIS.msg.uParam.resize(1);
	YVIS.msg.fData.resize(0);
	YVIS.msg.uParam[0]=objectType;
	YVIS.Send(YVIS.msg);
	int j=0;
	int cond=0;
	while(j<YVIS.tar_num)
	{		
		if((abs(YVIS.target[j][0])<200)&&(abs(YVIS.target[j][1])<200))
		{
			ArmThread.vtar[0]=0;
			ArmThread.vtar[1]=0;
			ArmThread.vtar[2]=0;
			
			ArmThread.target[0]=YVIS.target[j][0]+8;// Compensate for error from vision and geometric, etc, in grasping task
			//ArmThread.target[0]=YVIS.target[j][0];
			ArmThread.target[1]=YVIS.target[j][1];
			ArmThread.target[2]=YVIS.target[j][2]+4;
			ArmThread.see_tar=1;
			cond=1;
			break;
		}else
		{
			j++;
		}
	}
	if(cond==0)
	{
		ArmThread.vtar[0]=0;
		ArmThread.vtar[1]=0;
		ArmThread.vtar[2]=0;
	}
}
void CPHRIDlg::searchLocation(int objectType)
{
	ArmThread.see_tar=0;
	YVIS.mode=2;
	YVIS.object_type=objectType;
	
	YVIS.msg.uCommand=VISION_BOT_GET_POSITION_OBJECT_TYPE;
	YVIS.msg.uParam.resize(1);
	YVIS.msg.fData.resize(0);
	YVIS.msg.uParam[0]=objectType;
	YVIS.Send(YVIS.msg);
	int j=0;
	int cond=0;
	while(j<YVIS.tar_num)
	{
		
		if((abs(YVIS.target[j][0])<900)&&(abs(YVIS.target[j][1])<900))
		{
			ArmThread.vtar[0]=YVIS.target[j][0]+15-ArmThread.target[0];
			ArmThread.vtar[1]=YVIS.target[j][1]+2-ArmThread.target[1];
			ArmThread.vtar[2]=YVIS.target[j][2]+20-ArmThread.target[2];

			ArmThread.target[0]=YVIS.target[j][0]+15;
			ArmThread.target[1]=YVIS.target[j][1]+2;
			ArmThread.target[2]=YVIS.target[j][2]+20;
			ArmThread.see_tar=1;
			cond=1;
			break;
		}else
		{
			j++;
		}
	}
	if(cond==0)
	{
		ArmThread.vtar[0]=0;
		ArmThread.vtar[1]=0;
		ArmThread.vtar[2]=0;
	}
}
void CPHRIDlg::updateVision()  // IMPORTANT
{
	int objectTypes[2];
	objectTypes[0]=OBJECT_MAGENTA_SMALL;
	objectTypes[1]=OBJECT_MAGENTA_BIG;
	//objectTypes[0]=OBJECT_RED;
	time_n=GetTickCount()/1000.0f;
	//Search obstacles
	searchObstacle(objectTypes,2);	
	if(ArmThread.search_handle==1)
	{
		//Search handle		
		searchHandle(OBJECT_GREEN);		
	}
	if(ArmThread.search_tar==1)
	{
		//Search target
		searchTarget(OBJECT_GREEN);
	}
	if(ArmThread.search_loc==1)
	{
		//Search location
		if(ArmThread.HandThread.catched==1)
		{
			searchLocation(OBJECT_RED);
		}
	}
	if(YVIS.neck==1)
	{
		YVIS.msg.uCommand=VISION_BOT_AROS_PAN_TILT_INTERFACE;
		YVIS.msg.uParam.resize(1);
		YVIS.msg.uParam[0]=PANTILT_MOVE_ANGLE_ABS_PANTILT;
		YVIS.msg.fData.resize(2);
		YVIS.msg.fData[0]=YVIS.v_pan;
		YVIS.msg.fData[1]=YVIS.v_tilt;					
		YVIS.Send(YVIS.msg);
		YVIS.neck=0;
	}
	time_p=time_n;
	/*
	//An obstacle represents the table during the drawing tasks ,COMMENT IT for other tasks
	int j=ArosArm.obs_num;	
	ArosArm.obs_num=j+1;
	ArosArm.obs_array[j][0]=0;
	ArosArm.obs_array[j][1]=0;
	ArosArm.obs_array[j][2]=-50;
	ArosArm.obs_array[j][3]=15;		
	ArosArm.obs_f[j]=-3;
	*/	
	double tim=(GetTickCount()-ArosArm.start)/1000.0;
	str_text.str("");	
	str_text<<"No: ";
	str_text<<ArosArm.obs_num;
	SNO.SetWindowTextA(str_text.str().c_str());	
	str_text.str("");	
	str_text<<"Ox: ";
	str_text<<ArosArm.obs_array[0][0];
	//str_text<<ArmThread.target[0];
	//str_text<<ArmThread.location[0];
	SOX.SetWindowTextA(str_text.str().c_str());	
	str_text.str("");	
	str_text<<"Oy: ";
	str_text<<ArosArm.obs_array[0][1];
	//str_text<<ArmThread.target[1];
	//str_text<<ArmThread.location[1];
	SOY.SetWindowTextA(str_text.str().c_str());	
	str_text.str("");	
	str_text<<"Oz: ";
	str_text<<ArosArm.obs_array[0][2];
	//str_text<<ArmThread.target[2];
	//str_text<<ArmThread.location[2];
	SOZ.SetWindowTextA(str_text.str().c_str());	
	str_text.str("");	
	str_text<<"Time: ";
	str_text<<tim;
	ST.SetWindowTextA(str_text.str().c_str());	

	if(ArosArm.see_target==1)
	{
		SSTAR.SetWindowTextA("See the handle!");
	}else
	{
		SSTAR.SetWindowTextA("Not see the handle!");
	}
	str_text.str("");	
	str_text<<ArosArm.target[0];	
	SXTAR.SetWindowTextA(str_text.str().c_str());	
	str_text.str("");	
	str_text<<ArosArm.target[1];	
	SYTAR.SetWindowTextA(str_text.str().c_str());
	str_text.str("");	
	str_text<<ArosArm.target[2];	
	SZTAR.SetWindowTextA(str_text.str().c_str());
	str_text.str("");	
	str_text<<ArosArm.target[3]*180/M_PI;	
	SDTAR.SetWindowTextA(str_text.str().c_str());
}
void CPHRIDlg::updateArm()
{
	if(ArmThread.started)
	{
		updateJValue(ArmThread.jtheta);
		updateInformation(ArmThread.jtheta);
		updateInformation();
		str_text.str("");	
		str_text<<"Time: ";
		str_text<<ArmThread.tim-ArmThread.tims;
		SS.SetWindowTextA(str_text.str().c_str());	
		updateForceWrist(ArosArm.fW);
		updateLaserObstacle(ArosArm.obs_g);
	}else
	{
		BMV.SetWindowTextA("Arm");
		BTE.SetWindowTextA("Execute");
		KillTimer(Timer_Arm); 

		/*if((ArmThread.HandThread.catched==1)&&(ArmThread.modep==2))//Catched sucessfully
		{	//Place the target
			ArmThread.mode=3;
			ArosArm.control_e=1;
			ArmThread.start();			
			SetTimer(Timer_Arm,100,NULL);
			ArmThread.modep=3;
		}*/
		/*
		if((ArmThread.HandThread.catched==0)&&(ArmThread.modep==3)) //Pursue target again
		{//Pursue the target
			ArmThread.mode=2;
			ArosArm.control_e=1;
			ArmThread.start();			
			SetTimer(Timer_Arm,100,NULL);
			ArmThread.modep=2;
		}
		*/		
	}
}
void CPHRIDlg::updateForceW()
{
	//float jtheta[7];
	YPHRI.Send(AMTEC_GET_FORCE_TORQUE);
	YPHRI.Send(AMTEC_GET_POS_ALL);
	ArosArm.fForceCompensated(YPHRI.jtheta,YPHRI.fW,ArosArm.fW);
	//ArosArm.fForceProcessing(YPHRI.jtheta,YPHRI.fW,tim);
	//ArosArm.fForceEnd(YPHRI.jtheta,YPHRI.fW,ArosArm.fW);	
	updateJValue(YPHRI.jtheta);
	//updateForceWrist(YPHRI.fW);
	//YPHRI.fW[5]=sqrt(YPHRI.fW[0]*YPHRI.fW[0]+YPHRI.fW[1]*YPHRI.fW[1]+YPHRI.fW[2]*YPHRI.fW[2]);
	//ArmThread.fW[5]=sqrt(ArmThread.fW[0]*ArmThread.fW[0]+ArmThread.fW[1]*ArmThread.fW[1]+ArmThread.fW[2]*ArmThread.fW[2]);
	updateForceWrist(ArosArm.fW);
	//updateForceWrist(YPHRI.fW);
	
}
void CPHRIDlg::updateVoice()
{
	YVOI.Send(VOICE_MESSAGE);
	str_text.str("");
	str_text<<"Command: ";
	str_text<<YVOI.command;
	SS.SetWindowTextA(str_text.str().c_str());
	
}
void CPHRIDlg::updateLaser()
{
	YOBS.Send(RANGE_FINDER_GET_LAST_SCAN);		
	updateLaserObstacle(YOBS.obs_g);
	
}
void CPHRIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Timer handling
	switch(nIDEvent)
	{
	case Timer_Vision:
		updateVision();
		break;
	case Timer_Arm:
		updateArm();
		break;
	case Timer_Force:
		updateForceW();
		break;
	case Timer_Voice:
		updateVoice();
		break;
	case Timer_Laser:
		updateLaser();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
void CPHRIDlg::OnBnClickedBtc()
{
	// Record force from the force sensor for compensating the weight and orientation of the end-effector
	
	//Down -> up : +90 -> -90
	//UP -> DOWN : -90 -> +90	
	std::ofstream fdforce,fdjpos;
	fdforce.open("dataforce.txt");
	fdjpos.open("datajpos.txt");

	YPHRI.Send(AMTEC_GET_POS_ALL);
	YPHRI.msg.uCommand=AMTEC_MOVE_VEL_JOINT;
	YPHRI.msg.uParam.resize(1);
	YPHRI.msg.fData.resize(1);
	/*
	YPHRI.msg.uParam[0]=5;
	YPHRI.msg.fData[0]=-M_PI/30;
	YPHRI.Send(YPHRI.msg);
	while(YPHRI.jtheta[5]>(-M_PI/2)) //(YPHRI.jtheta[5]>(-M_PI/2))  //(YPHRI.jtheta[5]<(M_PI/2))
	{
		YPHRI.Send(AMTEC_GET_POS_ALL);
		YPHRI.Send(AMTEC_GET_FORCE_TORQUE);
		ArosArm.fForceEnd(YPHRI.jtheta,YPHRI.fW,ArmThread.fW);
		for (int i=0;i<6;i++)
		{
			fdforce << ArmThread.fW[i] << " ";
		}
		fdforce << ";";
		fdjpos << YPHRI.jtheta[5] << ";";

	}
	*/
	YPHRI.msg.uParam[0]=6;
	YPHRI.msg.fData[0]=M_PI/50;
	YPHRI.Send(YPHRI.msg);
	while(YPHRI.jtheta[6]<(M_PI*160/180)) //(YPHRI.jtheta[5]>(-M_PI/2))  //   YPHRI.jtheta[6]>(-M_PI*160/180)
	{
		YPHRI.Send(AMTEC_GET_POS_ALL);
		YPHRI.Send(AMTEC_GET_FORCE_TORQUE);
		ArosArm.fForceEnd(YPHRI.jtheta,YPHRI.fW,ArosArm.fW);
		for (int i=0;i<6;i++)
		{
			fdforce << ArosArm.fW[i] << " ";
		}
		fdforce << ";";
		fdjpos << YPHRI.jtheta[6] << ";";

	}
	YPHRI.Send(AMTEC_SOFT_STOP);
}
// Task execution   IMPORTANT
void CPHRIDlg::OnBnClickedBte()
{
	if(!ArmThread.started)
	{   //Perform a task		
		int buf_size;
		char *buf;
		float data;
		int task;
		//Mask velocity components that doesn't need to be controlled			
		if(CVX.GetCheck()==BST_CHECKED)
		{			
			ArmThread.mask[0]=1;
			buf_size = EVX.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVX.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[0]=data;			
			delete[] buf;
		}else
		{
			ArmThread.mask[0]=0;
		};
		if(CVY.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[1]=1;
			buf_size = EVY.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVY.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[1]=data;			
			delete[] buf;
		}else
		{
			ArmThread.mask[1]=0;
		};
		if(CVZ.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[2]=1;
			buf_size = EVZ.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVZ.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[2]=data;			
			delete[] buf;
		}else
		{
			ArmThread.mask[2]=0;
		};
		if(CVA.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[3]=1;
			buf_size = EVA.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVA.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[3]=data/180*M_PI;			
			delete[] buf;
		}else
		{
			ArmThread.mask[3]=0;
		};
		if(CVB.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[4]=1;
			buf_size = EVB.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVB.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[4]=data/180*M_PI;			
			delete[] buf;
		}else
		{
			ArmThread.mask[4]=0;
		};
		if(CVC.GetCheck()==BST_CHECKED)
		{
			ArmThread.mask[5]=1;
			buf_size = EVC.GetWindowTextLengthA()+ 1;
			buf= new char[buf_size];
			EVC.GetWindowTextA(buf,buf_size);
			sscanf( buf, "%f", &data );
			ArmThread.dpos0[5]=data/180*M_PI;			
			delete[] buf;
		}else
		{
			ArmThread.mask[5]=0;
		};
		buf = NULL;	
		task=CTA.GetCurSel();
		ArosArm.start=GetTickCount();
		switch (task)
		{
			case 0://Draw an ellipse
				ArmThread.mode=1;
				ArosArm.control_e=1;
				ArosArm.flag_ypr=0;
				ArmThread.modep=1;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}	
				ArmThread.mask[3]=0;
				ArmThread.mask[4]=0;
				ArmThread.mask[5]=0;
				
				break;
			case 1://Pursue a target
				ArmThread.mode=2;
				ArosArm.control_e=1;
				ArosArm.flag_ypr=0;
				ArmThread.modep=2;
				break;
			case 2://Force reation
				ArmThread.mode=4;	
				ArosArm.control_e=0;
				ArosArm.flag_ypr=0;
				ArmThread.modep=4;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}
				break;
			case 3://Interaction
				ArmThread.mode=5;	
				ArosArm.control_e=0;
				ArosArm.flag_ypr=1;
				ArmThread.modep=5;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}				
				break; 
			
			case 4:// Take me around
				ArmThread.mode=6;				
				ArmThread.modep=6;	
				ArosArm.flag_ypr=1;
				ArosArm.control_e=0;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}
				ArmThread.mask[4]=0;//Release pitch
				break;
			case 5:// Mobile autonomous driving
				ArmThread.mode=8;				
				ArmThread.modep=8;	
				break;
			case 6:// Object transportation
				// Take robot to the object
				ArmThread.mode=9;				
				ArmThread.modep=9;	
				ArosArm.flag_ypr=1;
				ArosArm.control_e=0;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}
				ArmThread.mask[4]=0;//Release pitch
				break;
			case 7:// Transformer
				ArmThread.mode=10;				
				ArmThread.modep=10;	
				ArosArm.flag_ypr=1;
				ArosArm.control_e=0;
				for(int i=0;i<6;i++)
				{
					ArmThread.mask[i]=1;
					ArmThread.dpos[i]=0;
				}	
				ArmThread.mask[4]=0;//Release pitch
				break;		
			
			default:
				break;
		}
		if(task>=0)
		{			
			ArmThread.start();
			BTE.SetWindowTextA("Stop");			
			SetTimer(Timer_Arm,100,NULL);
		}else
		{
			SS.SetWindowTextA("Arm: doesn't want to move");			
		};
		
	}else
	{
		ArmThread.stop=1;
		ArmThread.mode=0;
	}
}



void CPHRIDlg::OnConfigurationExit()
{
	// TODO: Add your command handler code here

	CDialog::OnCancel();
}


void CPHRIDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}
