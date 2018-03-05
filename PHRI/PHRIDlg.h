// PHRIDlg.h : header file
//

#pragma once

#include "YarpCommPHRI.h"
#include "afxwin.h"
#include "ArosArm.h"
#include "ArmThread.h"
#include <yarp/os/Network.h>
#include <sstream>

#define Timer_Vision 10
#define Timer_Arm 20
#define Timer_Force 30
#define Timer_Voice 40
#define Timer_Laser 50
// CPHRIDlg dialog
class CPHRIDlg : public CDialog
{ 
// Construction
public:
	CPHRIDlg(CWnd* pParent = NULL);	// standard constructor
	~CPHRIDlg();	// standard constructor

// Dialog Data
	enum { IDD = IDD_PHRI_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
// Implementation
protected:
	HICON m_hIcon;	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	yarp::os::Network yarp_net;

public:
	//------Modules-------
	CYarpCommPHRI YPHRI;
	CYarpCommPHRI YVIS;
	CYarpCommPHRI YSPE;
	CYarpCommPHRI YHAN;
	CYarpCommPHRI YVOI;
	CYarpCommPHRI YMOB;
	CYarpCommPHRI YOBS;
	//Attributes
	CMenu *menuPHRI;	
	CBitmap bitmap_Conf;
	CBitmap bitmap_Exit;
	CBitmap bitmap_Help;
	CBitmap bitmap_Save;
	CBitmap bitmap_Load;
	CBitmap bitmap_Configuration;
	std::stringstream str_text;
	//Variables
	float time_n,time_p;
	//Functions

	void updateJValue(float *jtheta);
	void updateJValueD(float *jtheta);
	void updateGriper(float grip);
	void updateLaserObstacle(float *obs);
	void readJValueD(float* jtheta);
	void readPValueD(float* pos);//Read desired value for end-effector
	void readRPY(float* pos);//Read desired value Roll Pitch Yaw for end-effector
	void readGripper(float *gripper);
	void readTime(int *time);// Read the time field for for doing movements
	void prepareEditfield();// Load default values
	void updateInformation(float *jtheta);// End-effector info
	void updateInformation();// Other infomation
	void updateForceWrist(float *fW);// update Force at the wrist
	void updateVision();// Important : Update obstacle coordinates in the ArosArm object
	void updateArm();
	void updateForceW();
	void updateVoice();
	void updateLaser();
	void searchObstacle(int *objectType, int nType);
	void searchHandle(int objectType);
	void searchTarget(int objectType);
	void searchLocation(int objectType);
	
	//Important Objects
	CArosArm ArosArm;
	CArmThread ArmThread;
	// Configuration CONNECT button
	CButton BCC;
	// Configuration GO HOME button
	CButton BCGH;
	// Configuration GO REST button
	CButton BGR;
	// Joint movement MOVE button
	CButton BJMM;
	// Arm movement MOVE button
	CButton BAMM;	
	//Time
	CStatic ST;
	//Velocity controlled MOVE movement button
	CButton BMV;
	// Execute button
	CButton BTE;
	//Task Combobox
	CComboBox CTA;
	//GUI objects
	CEdit EJ0;
	CEdit EJ1;
	CEdit EJ2;
	CEdit EJ3;
	CEdit EJ4;
	CEdit EJ5;
	CEdit EJ6;
	CEdit EJ7;
	CEdit EX;
	CEdit EY;
	CEdit EZ;
	CEdit EA;
	CEdit EB;
	CEdit EC;
	CButton CPA;
	CEdit EPA;
	CStatic SX;
	CStatic SY;
	CStatic SZ;
	CStatic SA;
	CStatic SB;
	CStatic SC;
	CStatic SS;
	CStatic SOX;
	CStatic SOY;
	CStatic SOZ;
	CStatic SNO;
	
	CButton CVX;
	CButton CVY;
	CButton CVZ;
	CButton CVA;
	CButton CVB;
	CButton CVC;
	CEdit EVX;
	CEdit EVY;
	CEdit EVZ;
	CEdit EVA;
	CEdit EVB;
	CEdit EVC;
	
	CStatic SJ0;
	CStatic SJ1;
	CStatic SJ2;
	CStatic SJ3;
	CStatic SJ4;
	CStatic SJ5;
	CStatic SJ6;
	CStatic SJ7;
	CEdit ET;
	CButton BJMG;	
	CStatic SFX;
	CStatic SFY;
	CStatic SFZ;
	CStatic STX;
	CStatic STY;
	CStatic STZ;
	CButton CJ0;
	CButton CJ1;
	CButton CJ2;
	CButton CJ3;
	CButton CJ4;
	CButton CJ5;
	CButton CJ6;
	CButton CJ7;
	CEdit EX2;
	CEdit EY2;
	CEdit EZ2;
	CEdit EYAW;
	CEdit EPIT;
	CEdit EROL;
	CStatic SYAW;
	CStatic SPIT;
	CStatic SROL;
	CButton BTC;
	CEdit EVM;
	CEdit EWM;	
	CButton BMM;
	CStatic SOBS1;
	CStatic SOBS2;
	CStatic SOBS3;
	CStatic SOBS4;
	CStatic SOBS5;
	CStatic SVM;
	CStatic SWM;
	CStatic SXTAR;
	CStatic SYTAR;
	CStatic SZTAR;
	CStatic SDTAR;
	CStatic SVDS0;
	CStatic SVDS1;
	CStatic SSTAR;
	CStatic SLVES0;
	CStatic SLVES1;
	CStatic SDT;
	
	//Events
	afx_msg void OnAboutPhri();
	afx_msg void OnBnClickedBcc();// Connect button pressed
	afx_msg void OnBnClickedBjmm();// Joint Move button pressed
	afx_msg void OnBnClickedBcgh(); // Go Home button pressed
	afx_msg void OnBnClickedBTest(); //Test button
	afx_msg void OnBnClickedCpa(); //Using a value for Arm plane angle or automatic searching
	afx_msg void OnBnClickedBamm();// Move to a desured position for the end-effector
	afx_msg void OnBnClickedBmv(); // Move with desire velocity for end-effector
	afx_msg void OnBnClickedBjmg();// Move griper
	afx_msg void OnKinematicsJointlimitavoidance();// Enable/Disable Joint limit avoidance behaviour
	afx_msg void OnKinematicsSingularityavoidance();// Enable/Disable Singularity avoidance behaviour
	afx_msg void OnKinematicsObstacleavoidance();// Enable/Disable Obstacle avoidance behaviour
	afx_msg void OnModulesVision();//Start modules
	afx_msg void OnModulesSpeech();		
	afx_msg void OnModulesHand();
	afx_msg void OnModulesNeck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);//Timer handler		
	afx_msg void OnBnClickedBte();//Task execution	
	afx_msg void OnBnClickedBtc();// Record force value for calibration	
	afx_msg void OnBnClickedBhmm();// Move to the desired Yaw Pitch Roll
	
	afx_msg void OnModulesVoice();
	afx_msg void OnModulesMobile();
	afx_msg void OnBnClickedBmm();
	
	afx_msg void OnModulesLaserrangefinder();		
	CStatic SXTARE;
	CStatic SYTARE;
	CStatic SZTARE;
	afx_msg void OnConfigurationExit();
	virtual void OnCancel();
};
