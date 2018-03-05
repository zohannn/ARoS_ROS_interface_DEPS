#include "ArosArm.h"
#include "Matrix.h"

//------------Constructor
CArosArm::CArosArm(void)
{
	//Initialize params

	//-------------------ARoS------------------
	
	L1=340.0f+40.0f;
	L3=395.0f;
	L5=370.0f;
	//L7=260;
	L7=350.0f; // When catching an object
	//L7=420; // When mount the pen for drawing
	//-----Force calibration
	Lf=95.0f;
	theta_f=M_PI/2; //Aros
	// Parameter for compensating the end effector orientation and weight
	spline[0][0][0]=-24.9100f; spline[0][0][1]=1.1611f; spline[0][0][2]=0.9144f; spline[0][0][3]=-0.0993f; spline[0][0][4]=-0.0362f; spline[0][1][0]=6.9160f; spline[0][1][1]=0.4978f; spline[0][1][2]=-0.6573f; spline[0][1][3]=-0.0706f; spline[0][1][4]=0.0490f;  spline[0][2][0]=3.4820f; spline[0][2][1]=-0.2326f; spline[0][2][2]=0.6930f; spline[0][2][3]= -0.0027f;  spline[0][2][4]=-0.1082f; spline[0][3][0]=-1.0560f; spline[0][3][1]=-0.5108f; spline[0][3][2]=0.5457f; spline[0][3][3]=0.0812f; spline[0][3][4]=-0.0400f;  spline[0][4][0]=2.0080f; spline[0][4][1]=-0.7855f; spline[0][4][2]=-1.8592f; spline[0][4][3]=0.1021f; spline[0][4][4]=0.1681f;                                                                
	spline[1][0][0]=7.6480f; spline[1][0][1]=0.1199f; spline[1][0][2]=0.2094f; spline[1][0][3]=-0.0078f; spline[1][0][4]=-0.0425f; spline[1][1][0]=6.7647f; spline[1][1][1]=-0.7946f; spline[1][1][2]=-1.2467f; spline[1][1][3]=0.0476f; spline[1][1][4]=0.0601f;  spline[1][2][0]=-1.1127f; spline[1][2][1]=0.1468f; spline[1][2][2]=-0.8895f; spline[1][2][3]=-0.0031f;  spline[1][2][4]=0.1232f; spline[1][3][0]=-4.6571f; spline[1][3][1]=-0.7716f; spline[1][3][2]=0.2412f; spline[1][3][3]=0.1378f; spline[1][3][4]=-0.0009f;  spline[1][4][0]=-5.2419f; spline[1][4][1]=-0.1955f; spline[1][4][2]=0.6587f; spline[1][4][3]=0.0107f; spline[1][4][4]=-0.0792f;
	spline[2][0][0]=1.6280f; spline[2][0][1]=1.8924f; spline[2][0][2]=-0.3877f; spline[2][0][3]=-0.2062f; spline[2][0][4]=-0.0066f; spline[2][1][0]=0.1079f; spline[2][1][1]=-0.6278f; spline[2][1][2]=-0.1458f; spline[2][1][3]=0.0697f; spline[2][1][4]=0.0115f;  spline[2][2][0]=0.6519f; spline[2][2][1]=0.4943f; spline[2][2][2]=-0.7602f; spline[2][2][3]=0.0004f;  spline[2][2][4]=0.1190f; spline[2][3][0]=-0.2274f; spline[2][3][1]=0.4564f; spline[2][3][2]=0.1831f; spline[2][3][3]= -0.0509f; spline[2][3][4]=-0.0157f;  spline[2][4][0]=-0.1994f; spline[2][4][1]=-0.5659f; spline[2][4][2]=0.4272f; spline[2][4][3]=0.0047f; spline[2][4][4]=-0.0730f;
	spline[3][0][0]=1.8000f; spline[3][0][1]=0.0056f; spline[3][0][2]=-0.0689f; spline[3][0][3]=-0.0008f; spline[3][0][4]=0.0070f; spline[3][1][0]=0.1612f; spline[3][1][1]=0.0627f; spline[3][1][2]=0.0014f; spline[3][1][3]=-0.0076f; spline[3][1][4]=0.0021f;  spline[3][2][0]=-0.4790f; spline[3][2][1]=0.0300f; spline[3][2][2]=0.0188f; spline[3][2][3]=-0.0063f;  spline[3][2][4]=-0.0011f; spline[3][3][0]=-0.0937f; spline[3][3][1]=0.0020f; spline[3][3][2]=0.0008f; spline[3][3][3]= -0.0007f; spline[3][3][4]=-0.0006f;  spline[3][4][0]=-1.2535f; spline[3][4][1]=-0.0278f; spline[3][4][2]=0.0096f; spline[3][4][3]=0.0060f; spline[3][4][4]=-0.0021f;
	/*
	//-----------------Dumbo-----------------
	L1=400.0;
	L3=395.0;
	L5=370.0;
	L7=300;	
	//-----Force calibration
	Lf=95.0;
	theta_f=-M_PI/2; //Dumbo
	// Parameter for compensating the end effector orientation and weight
	spline[0][0][0]=-23.9100; spline[0][0][1]=  0.3578; spline[0][0][2]= 0.5405 ; spline[0][0][3]=-0.0218; spline[0][0][4]= -0.0465; spline[0][1][0]= 5.0738; spline[0][1][1]= 0.6641 ; spline[0][1][2]= -0.5387; spline[0][1][3]=-0.0704; spline[0][1][4]=0.0545;  spline[0][2][0]=3.9475; spline[0][2][1]= 1.1015; spline[0][2][2]=  -0.1693 ; spline[0][2][3]=   -0.1904;  spline[0][2][4]= -0.0126; spline[0][3][0]=  -0.4938; spline[0][3][1]=-0.6548; spline[0][3][2]= 0.5075; spline[0][3][3]=0.0779; spline[0][3][4]=-0.0502;  spline[0][4][0]= 0.1325; spline[0][4][1]= -1.4922; spline[0][4][2]=  -0.5968; spline[0][4][3]= 0.2076; spline[0][4][4]= 0.0822;                                                                
	spline[1][0][0]= 7.5150; spline[1][0][1]= 0.4812; spline[1][0][2]=-0.3407; spline[1][0][3]=  -0.0612; spline[1][0][4]=0.0247; spline[1][1][0]=5.1113; spline[1][1][1]=  -0.2096; spline[1][1][2]= -0.2306; spline[1][1][3]= 0.0204; spline[1][1][4]=  0.0067;  spline[1][2][0]=  -2.3515 ; spline[1][2][1]= -0.9371 ; spline[1][2][2]= 0.7792; spline[1][2][3]= 0.1383 ;  spline[1][2][4]=  -0.0840; spline[1][3][0]=-4.4328; spline[1][3][1]=  -0.3746; spline[1][3][2]= -0.0520; spline[1][3][3]=0.0360; spline[1][3][4]= 0.0092;  spline[1][4][0]=  -2.8250; spline[1][4][1]=  0.3958  ; spline[1][4][2]=-0.5955; spline[1][4][3]= -0.0757; spline[1][4][4]= 0.0786	;
	spline[2][0][0]= -0.3076; spline[2][0][1]=0.8579; spline[2][0][2]=0.1884 ; spline[2][0][3]= -0.0956; spline[2][0][4]= -0.0479; spline[2][1][0]=0.0199; spline[2][1][1]= 0.4298; spline[2][1][2]=-0.2838; spline[2][1][3]= -0.0383; spline[2][1][4]= 0.0351;  spline[2][2][0]=1.0019; spline[2][2][1]= -0.6679; spline[2][2][2]=  -1.3236; spline[2][2][3]=0.0743;  spline[2][2][4]= 0.1624; spline[2][3][0]=0.0605; spline[2][3][1]=-0.2058; spline[2][3][2]= 0.2808; spline[2][3][3]=  0.0089; spline[2][3][4]=  -0.0394;  spline[2][4][0]=  0.2983; spline[2][4][1]= 0.5216; spline[2][4][2]=  0.7294 ; spline[2][4][3]=-0.0617; spline[2][4][4]= -0.0875	;
	spline[3][0][0]= 1.7210; spline[3][0][1]= -0.0385; spline[3][0][2]= 0.0159; spline[3][0][3]=0.0046; spline[3][0][4]= -0.0020 ; spline[3][1][0]= 0.2334; spline[3][1][1]= 0.1219; spline[3][1][2]=  0.0360; spline[3][1][3]=  -0.0127; spline[3][1][4]= -0.0035;  spline[3][2][0]=  -0.4500; spline[3][2][1]=  -0.0347; spline[3][2][2]=  0.0340 ; spline[3][2][3]= 0.0033;  spline[3][2][4]=  -0.0051 ; spline[3][3][0]= -0.0818; spline[3][3][1]=  0.0204; spline[3][3][2]= -0.0308; spline[3][3][3]=   -0.0033; spline[3][3][4]= 0.0032;  spline[3][4][0]= -1.2484 ; spline[3][4][1]=0.0463 ; spline[3][4][2]= -0.0624 ; spline[3][4][3]=  -0.0053 ; spline[3][4][4]=0.0081;
	*/
	//----------------------------
	joint_offset[0]=0;joint_offset[1]=-M_PI/2;joint_offset[2]=0;joint_offset[3]=-M_PI/2;joint_offset[4]=0;joint_offset[5]=0;joint_offset[6]=0;
	joint_limit[0][0]=M_PI/180*(-165+5);joint_limit[0][1]=M_PI/180*(-15+5);joint_limit[0][2]=M_PI/180*(-165+5);joint_limit[0][3]=M_PI/180*(-25+5);joint_limit[0][4]=M_PI/180*(-165+5);joint_limit[0][5]=M_PI/180*(-120+5);joint_limit[0][6]=M_PI/180*(-165+5);
	joint_limit[1][0]=M_PI/180*(165-5);joint_limit[1][1]=M_PI/180*(181-5);joint_limit[1][2]=M_PI/180*(165-5);joint_limit[1][3]=M_PI/180*(196-5);joint_limit[1][4]=M_PI/180*(165-5);joint_limit[1][5]=M_PI/180*(120-5);joint_limit[1][6]=M_PI/180*(165-5);
	//velocity_max[0]=M_PI/180*52.2;velocity_max[1]=M_PI/180*52.2;velocity_max[2]=M_PI/180*52.2;velocity_max[3]=M_PI/180*41.2;velocity_max[4]=M_PI/180*41.2;velocity_max[5]=M_PI/180*240;velocity_max[6]=M_PI/180*360;
	velocity_max[0]=M_PI/180*40;velocity_max[1]=M_PI/180*40;velocity_max[2]=M_PI/180*40;velocity_max[3]=M_PI/180*30;velocity_max[4]=M_PI/180*30;velocity_max[5]=M_PI/180*150;velocity_max[6]=M_PI/180*250;
	
	control_e=0;
	//---------------Joint limit coefficients
	Kj1=0.4f;
	Kj2=0.02f;
	Kj3=0.6f;
	Kj4=0.2f;
	//--------------Singularity coefficients
	Ks1=1.0f;
	Ks2=0.2f;
	Ks3=0.2f;
	//--------------Obstacle coefficients
	Ko1=90.0f;
	Ko2=0.4f;
	Ko3=0.2f;
	//-----Offset and size range for obstacles
	Koff1=30.0f;
	Koff2=30.0f;	
	Kor=1.0f;
	//-----Dynamic filter
	K_fm=3.0f;
	K_fr=0.1f;
	//--------------Interation of behaviours
	Kn1=0.5f;
	Kn2=0.2f;
	//------------
	jlimEnable=1;
	singEnable=1;
	obsEnable=1;
	obs_num=0;	
	for(int i=0;i<6;i++)
	{
		fW[i]=0.0f;
		fW0[i]=0.0f;
		fWp[i]=0.0f;
		dposp[i]=0.0f;
	}
	vmobp[0]=0.0f;
	vmobp[1]=0.0f;
	// Force filtering
	K_fmax=10.0f/(2.0f*M_PI);
	K_fmin=2.0f/(2.0f*M_PI);
	K_for=3.0f;
	K_mor=0.1f;
	
	//Eliminate secondary torque
	flag_torquez=0;
	flag_torquey=0;

	
	// Mobile platform
	ltar=0.1f;
	beta1=0.1f;
	beta2=50.0f;
	lfor=0.1f;
	rrobot=30.0f;//cm
	sensor_number=85;
	sensor_sector=240.0f*M_PI/85.0f/180.0f;
	target[0]=0.0f;
	target[1]=0.0f;
	for(int i=0;i<85;i++)
	{
		obs_angle[i]=-120.0f*M_PI/180.0f+(i+0.5f)*sensor_sector;
		obs_g[i]=200.0f;
	}
	// Pid for roll pitch yaw
	Kpw=0.05f;
	Tiw=100.0f;
	Tdw=0.0001f;
	Nw=3.0f;
	for(int i=0;i<3;i++)
	{
		errYPR[i]=0.0f;
		errYPRp[i]=0.0f;
		dyprp[i]=0.0f;
	}
	flag_ypr=0;
	// Force limit
	Fmax[0]=30.0f;Fmax[1]=30.0f;Fmax[2]=30.0f;Fmax[3]=4.0f;Fmax[4]=4.0f;Fmax[5]=4.0f;
	Fmin[0]=3.0f;Fmin[1]=3.0f;Fmin[2]=1.0f;Fmin[3]=0.4f;Fmin[4]=0.4f;Fmin[5]=0.4f;
	//Flag
	Mcontrol=0;
	
	normal=1;
	flag_comp=1;
	flag_hurt=0;	
	target[0]=100;//Values for safety , any number 
	target[1]=0;
	target[2]=-20;	
	avoidobs=0;
}
void CArosArm::fReset(float tim)
{
	timp=tim;
	tims=tim;	
	timEp=tim;	
	timIntegrate=tim;
	for(int i=0;i<6;i++)
	{
		fWp[i]=0;	
		vendp[i]=0;
	}
	for(int i=0;i<3;i++)
	{
		errYPR[i]=0;
		errYPRp[i]=0;
		dyprp[i]=0;
	}
	F_dsp=0;
	M_dsp=0;
	for(int i=0;i<3;i++)
	{
		dL_ves[i]=0;
	}
	vmobp[0]=0;
	vmobp[1]=0;
}
//------------Destructor
CArosArm::~CArosArm(void)
{

}
//------------------------------------Internal functions
CMatrix<float> CArosArm::fTransMatrix(float a, float alpha, float d, float theta)
{
	float data[16]={cosf(theta),-sinf(theta),0,a,cosf(alpha)*sinf(theta),cosf(alpha)*cosf(theta),-sinf(alpha),-sinf(alpha)*d,sinf(alpha)*sinf(theta),sinf(alpha)*cosf(theta),cosf(alpha),cosf(alpha)*d,0,0,0,1};
	CMatrix<float> TransMatrix(4,4,data);
	return TransMatrix;
}
CMatrix<float> CArosArm::fTransRPY()
{
	float data[36]={1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,cosf(ypr[0])/cosf(ypr[1]),sinf(ypr[0])/cosf(ypr[1]),0, 0,0,0,sinf(ypr[0]),-cosf(ypr[0]),0, 0,0,0,-cosf(ypr[0])*sinf(ypr[1])/cosf(ypr[1]),-sinf(ypr[0])*sinf(ypr[1])/cosf(ypr[1]),1};
	//float data[36]={1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,cosf(ypr[0]),sinf(ypr[0]),0, 0,0,0,sinf(ypr[0]),-cosf(ypr[0]),0, 0,0,0,-cosf(ypr[0])*sinf(ypr[1]),-sinf(ypr[0])*sinf(ypr[1]),1};
	
	CMatrix<float> TransMatrix(6,6,data);
	return TransMatrix;
}
//---------
CMatrix<float> CArosArm::fJacobian(float *jtheta)
{
	//float exam;
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T07(4,4),Jacobian(6,7);
	CMatrix<float> colA(3,1),colB(3,1),colC(3,1);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T67,&T07);
	colA=T07(0,3,2,3);
	//-----------
	colB=T01(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,0,0,0,2,0);
	colC=T01(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,0,0,0,2,0);
	//-----------
	colB=T02(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,1,0,0,2,0);
	colC=T02(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,1,0,0,2,0);	
	//-----------
	colB=T03(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,2,0,0,2,0);	
	colC=T03(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,2,0,0,2,0);	
	//-----------
	colB=T04(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,3,0,0,2,0);	
	colC=T04(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,3,0,0,2,0);	
	//-----------
	colB=T05(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,4,0,0,2,0);	
	colC=T05(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,4,0,0,2,0);	
	//-----------
	colB=T06(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,5,0,0,2,0);	
	colC=T06(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,5,0,0,2,0);	
	//-----------
	colB=T07(0,2,2,2);
	fCopyMatrix(&Jacobian,&colB,3,6,0,0,2,0);	
	colC=T07(0,3,2,3);
	colA.Sub(&colC,&colC);
	colB.cross(&colC,&colB);
	fCopyMatrix(&Jacobian,&colB,0,6,0,0,2,0);	
	return Jacobian;
}

int CArosArm::fCheckRange(float jangle,int index)
{
	float jtheta=jangle-joint_offset[index];
	if((jtheta>joint_limit[0][index])&&(jtheta<joint_limit[1][index]))
	{
		return 1;
	}else
	{
		return 0;
	}	
}
int CArosArm::fCheckRangeAbsolute(float jangle,int index)
{	
	if((jangle>joint_limit[0][index])&&(jangle<joint_limit[1][index]))
	{
		return 1;
	}else
	{
		return 0;
	}	
}
void CArosArm::fForceCompensated(float *jtheta,float *fE,float *fE_b)
{
	//float temp,Zp[3];
	float Xf[3],Yf[3],Zf[3],fP[6],fP_t[6],ztheta,zphi,s_b,c_b;
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T6f(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T0f(4,4),T07(4,4);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T6f=fTransMatrix(0,-M_PI /2,Lf,jtheta[6]+joint_offset[6]+theta_f);	
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T6f,&T0f);
	T06.Mul(&T67,&T07);	
	for(int i=0;i<3;i++)
	{
		Xf[i]=T0f.get_Element(i,0);	
		Yf[i]=T0f.get_Element(i,1);
		Zf[i]=T0f.get_Element(i,2);	
		Oef[i]=T07.get_Element(i,3);
		Owr[i]=T05.get_Element(i,3);
		Oel[i]=T03.get_Element(i,3);
	}	
	//Convert force from sensor to base reference frame
	for(int i=0;i<3;i++)
	{
		fE_b[i]=fE[0]*Xf[i]+fE[1]*Yf[i]+fE[2]*Zf[i];
	}
	for(int i=3;i<6;i++)
	{
		fE_b[i]=fE[3]*Xf[i-3]+fE[4]*Yf[i-3]+fE[5]*Zf[i-3];
	}
	//Compensate for the end-effector
	// Yaw Pitch Roll
	s_b=T07.get_Element(2,2);
	c_b=sqrtf(1-s_b*s_b);
	if (c_b>0.01)
	{
		ypr[1]=atan2f(s_b,c_b);
		ypr[0]=atan2f(T07.get_Element(1,2)/c_b,T07.get_Element(0,2)/c_b);
		ypr[2]=atan2f(-T07.get_Element(2,1)/c_b,T07.get_Element(2,0)/c_b);
	}else
	{
		if(s_b>0)
		{
			ypr[1]=M_PI/2;
			ypr[0]=atan2f(T07.get_Element(1,3),T07.get_Element(0,3));
			ypr[2]=atan2f(T07.get_Element(0,1),-T07.get_Element(1,1));
		}else
		{
			ypr[1]=-M_PI/2;
			ypr[0]=atan2f(T07.get_Element(1,3),T07.get_Element(0,3));
			ypr[2]=atan2f(-T07.get_Element(0,1),-T07.get_Element(1,1));
		}
	}	
	ztheta=sinf(ypr[1]); //Pitch		
	zphi=ypr[2]; //Roll
	// Compensating force in local base reference frame
	fP_t[2]=fSpline2D(ztheta,zphi,0);//Fz
	fP_t[0]=fSpline2D(ztheta,zphi,1);//Fx
	fP_t[1]=fSpline2D(ztheta,zphi,2);//Fy
	fP_t[4]=fSpline2D(ztheta,zphi,3);//Ty	
	fP_t[3]=0;//Tx	
	fP_t[5]=0;//Tz
	//Convert compensating force to base reference frame	
	Xf[0]=cosf(ypr[0]);
	Xf[1]=sinf(ypr[0]);
	Xf[2]=0;
	Zf[0]=0;
	Zf[1]=0;
	Zf[2]=1;
	fcrossprod(Zf,Xf,Yf);	
	for(int i=0;i<3;i++)
	{
		fP[i]=fP_t[0]*Xf[i]+fP_t[1]*Yf[i]+fP_t[2]*Zf[i];
	}
	for(int i=3;i<6;i++)
	{
		fP[i]=fP_t[3]*Xf[i-3]+fP_t[4]*Yf[i-3]+fP_t[5]*Zf[i-3];
	}	
	//Compensating
	for(int i=0;i<6;i++)
	{
		fE_b[i]=fE_b[i]-fP[i];
	}
	//Eliminate secondary torque around z axis resulted from forces	
	if(flag_torquez==1)
	{
		fP[1]=fE_b[0]*Yf[0]+fE_b[1]*Yf[1];
		fE_b[5]=fE_b[5]-fP[1]*0.18f;	
				
		//fE_b[0]=fE_b[0]+abs(fE_b[5])/0.045*Xf[0];
		//fE_b[1]=fE_b[1]+abs(fE_b[5])/0.045*Xf[1];		
	}
	if(flag_torquey==1)
	{
		fP[0]=fE_b[2]*0.18f;
		fE_b[3]=fE_b[3]+fP[0]*Yf[0];
		fE_b[4]=fE_b[4]+fP[0]*Yf[1];
	}	
}
void CArosArm::fForceEnd(float *jtheta,float *fE,float *fE_b)
{
	float Xf[3],Yf[3],Zf[3];
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T6f(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T0f(4,4);
	T01=fTransMatrix(0.0f,M_PI /2.0f,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0.0f,M_PI /2.0f,0.0f,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0.0f,-M_PI /2.0f,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0.0f,M_PI /2.0f,0.0f,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0.0f,-M_PI /2.0f,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0.0f,M_PI /2.0f,0.0f,jtheta[5]+joint_offset[5]);
	T6f=fTransMatrix(0.0f,-M_PI /2.0f,Lf,jtheta[6]+joint_offset[6]+theta_f);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T6f,&T0f);
	for(int i=0;i<3;i++)
	{
		Xf[i]=T0f.get_Element(i,0);	
		Yf[i]=T0f.get_Element(i,1);
		Zf[i]=T0f.get_Element(i,2);		
	}
	//Convert to base reference frame
	for(int i=0;i<3;i++)
	{
		fE_b[i]=fE[0]*Xf[i]+fE[1]*Yf[i]+fE[2]*Zf[i];
	}
	for(int i=3;i<6;i++)
	{
		fE_b[i]=fE[3]*Xf[i-3]+fE[4]*Yf[i-3]+fE[5]*Zf[i-3];
	}
}
float CArosArm::fSpline2D(float theta, float phi, int comp)
{
	float r=0,x,y;
	for(int i=0;i<5;i++)
	{
		if(i==0) 
			x=1;
		else
			x=x*theta;
		for(int j=0;j<5;j++)
		{
			if(j==0) 
				y=1;
			else
				y=y*phi;
			r=r+spline[comp][i][j]*x*y;
		}
	}
	return r;

}
void CArosArm::fForceProcessing(float *jtheta,float *fE, float tim)
{
	//float temp, temp1;
	float K_f, alpha_f, dtim;
	fForceCompensated(jtheta,fE,fW);
	for(int i=0;i<6;i++)
	{
		fW[i]=fW[i]-fW0[i];
	}
	dtim=tim-timfp;
	if(dtim<0.001f) dtim=0.001f;
	timfp=tim;
	// Filter Force	
	for(int i=0;i<3;i++)
	{
		K_f=(K_fmax-K_fmin)*exp(-abs(fW[i]-fWp[i])/K_for)+K_fmin;
		alpha_f=(dtim)/(dtim+K_f);
		fW[i]=alpha_f*fW[i]+(1-alpha_f)*fWp[i];
	}
	// Filter Moment
	for(int i=3;i<6;i++)
	{
		K_f=(K_fmax-K_fmin)*exp(-abs(fW[i]-fWp[i])/K_mor)+K_fmin;
		alpha_f=(dtim)/(dtim+K_f);
		fW[i]=alpha_f*fW[i]+(1-alpha_f)*fWp[i];
	}
	fCopy(fWp,fW,6);
}
void CArosArm::fVelSmoothing(float *dpos,float tim)
{
	float dtim,K_f,alpha_f;
	dtim=tim-timEp;
	if(dtim<0.001f) dtim=0.001f;
	timEp=tim;
	for(int i=0;i<6;i++)
	{
		K_f=5.0f/(2.0f*M_PI);
		alpha_f=(dtim)/(dtim+K_f);
		dpos[i]=alpha_f*dpos[i]+(1-alpha_f)*dposp[i];
	}
	fCopy(dposp,dpos,6);
}
void CArosArm::fForceStartComp(float *jtheta,float *fE,float tim)
{
	//float temp,temp1,
	float K_f,alpha_f,dtim;	
	fForceEnd(jtheta,fE,fW);
	for(int i=0;i<6;i++)
	{
		fW[i]=fW[i]-fW0[i];
	}
	dtim=tim-timfp;
	if(dtim<0.001f) dtim=0.001f;
	timfp=tim;
	// Filter Force	
	for(int i=0;i<3;i++)
	{
		K_f=(K_fmax-K_fmin)*exp(-abs(fW[i]-fWp[i])/K_for)+K_fmin;
		alpha_f=(dtim)/(dtim+K_f);
		fW[i]=alpha_f*fW[i]+(1-alpha_f)*fWp[i];
	}
	// Filter Moment
	for(int i=3;i<6;i++)
	{
		K_f=(K_fmax-K_fmin)*exp(-abs(fW[i]-fWp[i])/K_mor)+K_fmin;
		alpha_f=(dtim)/(dtim+K_f);
		fW[i]=alpha_f*fW[i]+(1-alpha_f)*fWp[i];
	}
	fCopy(fWp,fW,6);
}
void CArosArm::fMobile(float *dmob, float tim)
{
	float vmob[2],temp,temp1;	
	//float sensor_angle,phi,vec[3];
	float lamda, sigma, K_f, alpha_f;
	float dt;	
	//int sign_d;		
	//dt=tim-timIntegrate;
	dt=0.08f;	

	M_ds=0.0f;//Rotation
	F_ds=0.0f;//Linear	

	L_ves[0]=acosf((Oel[1]+L1)/L3);
	if(Oel[0]<0.0f) L_ves[0]=2.0f*M_PI-L_ves[0];
	L_ves0[0]=130.0f*M_PI/180.0f;

	if(avoidobs==1)
	{		
		for(int i=0;i<sensor_number;i++)
		{
			if((i==25)||(i==26)||(i==27)||(i==58)||(i==59)||(i==60)||(i==61)) continue;				
			//Obstacle avoidance
			lamda=45*exp(-(obs_g[i]-0)/30);	//35   80  45
			if(L_ves[0]<L_ves0[0])
			{
				lamda=(65+2*(L_ves0[0]-L_ves[0]))*exp(-(obs_g[i]-0)/30);//80  45
			}			
			if(obs_angle[i]>0)
			{
				sigma=atan(tan(sensor_sector/2)+rrobot/obs_g[i]);
			}else
			{
				//sigma=atan(tan(sensor_sector/2)+40.0/obs_g[i]);
				sigma=atan(tan(sensor_sector/2)+rrobot/obs_g[i]);				
			}			
			//sigma=atan(tan(sensor_sector/2)+rrobot/obs_g[i]);
			temp=-lamda*obs_angle[i]*exp(-obs_angle[i]*obs_angle[i]/(2*sigma*sigma));
			M_ds=M_ds+temp;
			temp1=200*exp(-obs_g[i]/20);
			temp=-temp1*cos(obs_angle[i]);
			F_ds=F_ds+temp;
		}			
		//Filter dynamic Force, moment
		if(dt<0.001f) dt=0.001f;
		K_f=1.0f/(2.0f*M_PI);
		alpha_f=(dt)/(dt+K_f);
		M_ds=alpha_f*M_ds+(1.0f-alpha_f)*M_dsp;	
		M_dsp=M_ds;
		F_ds=alpha_f*F_ds+(1.0f-alpha_f)*F_dsp;	
		F_dsp=F_ds;
	}

	dLmax[0]=20.0f*M_PI/180.0f;
	dLmax[1]=20.0f;
	dLmax[2]=20.0f*M_PI/180.0f;
	L_limit[0][0]=90.0f*M_PI/180.0f;  L_limit[0][1]=250.0f*M_PI/180.0f;
	L_limit[1][0]=560.0f;  L_limit[1][1]=750.0f;
	L_limit[2][0]=110.0f*M_PI/180.0f;  L_limit[2][1]=260.0f*M_PI/180.0f;//20=>160	

	if(Mcontrol==0)// Human leads the way
	{			
		//Wrist phi2		
		temp=fprod(Oel,Owr,Oef,Owr);
		L_ves[2]=acosf(temp);		
		
		//Elbow L2
		L_ves[1]=sqrt(Owr[0]*Owr[0]+(Owr[1]+L1)*(Owr[1]+L1)+Owr[2]*Owr[2]);	
	    L_ves0[1]=sqrt(L3*L3+L5*L5-2*L3*L5*cosf(M_PI*110.0f/180.0f));//626  541  760
		temp1=1.0f/(1+exp(-(L_ves[1]-580.0f)/3.0f))+1.0f/(1+exp((L_ves[1]-700.0f)/3.0f))-1.0f;	

		k_e[1]=30.0f+70.0f*(1.0f/(1+exp(-(L_ves[1]-580.0f)/3.0f))+1.0f/(1+exp((L_ves[1]-700.0f)/3.0f))-1.0f);	
		//10  150
												
		k_m[1]=1;
		k_v[1]=2*sqrt(k_e[1]*k_m[1]);
		F_ves[1]=-temp1*F_ds;
		
		temp=F_ves[1]-k_e[1]*(L_ves[1]-L_ves0[1]);
		temp=abs(temp);
		k_m[1]=1.0f+sqrt((abs(temp)/600.0f));
		
		k_v[1]=2.0f*sqrt(k_e[1]*k_m[1]);
		ddL_ves[1]=(F_ves[1]-k_v[1]*dL_ves[1]-k_e[1]*(L_ves[1]-L_ves0[1]) )/k_m[1];
		dL_ves[1]=dL_ves[1]+ddL_ves[1]*dt;		

		//Shoulder phi1		
		L_ves[0]=acosf((Oel[1]+L1)/L3);
		if(Oel[0]<0) L_ves[0]=2.0f*M_PI-L_ves[0];
	    L_ves0[0]=130.0f*M_PI/180.0f;		
		temp=1.0f/(1.0f+exp(-(L_ves[0]/M_PI*180.0f-100.0f)/3.0f))+1.0f/(1.0f+exp((L_ves[0]/M_PI*180.0f-220.0f)/3.0f))-1.0f;
		
		k_e[0]=10.0f+40.0f*(1.0f/(1.0f+exp(-(L_ves[0]/M_PI*180.0f-100.0f)/2.0f))+1.0f/(1.0f+exp((L_ves[0]/M_PI*180.0f-220.0f)/2.0f))-1.0f);	
		//5  100
		k_m[0]=1.0f;
		k_v[0]=2.0f*sqrt(k_e[0]*k_m[0]);
		F_ves[0]=temp*M_ds;
		
		temp=F_ves[0]-k_e[0]*(L_ves[0]-L_ves0[0]);
		k_m[0]=1.0f+sqrt(abs(temp)/(10.0f*M_PI/180.0f));		
		k_v[0]=2.0f*sqrt(k_e[0]*k_m[0]);

		ddL_ves[0]=(F_ves[0]-k_v[0]*dL_ves[0]-k_e[0]*(L_ves[0]-L_ves0[0]) )/k_m[0];
		dL_ves[0]=dL_ves[0]+ddL_ves[0]*dt;
		/*
		if(abs(dL_ves[0])>15*M_PI/180)
		{
			dL_ves[0]=15*M_PI/180*dL_ves[0]/abs(dL_ves[0]);
		}
		*/
		if(flag_obs==1)
		{
			if((F_ds>-10.0f)&&(L_ves[0]<160.0f*M_PI/180.0f))
			{
				flag_obs=-1;
				flag_say=1;
			}

		}
		if((L_ves[0]>180.0f*M_PI/180.0f)&&(F_ds<-100.0f))
		{
			if(flag_obs==0)
			{
				flag_obs=1;
				flag_say=1;
			}			
		}
		if(flag_obs==2)
		{
			if((F_ds>-10.0f))
			{
				flag_obs=-2;
				flag_say=1;
			}
		}
		if((L_ves[0]<100*M_PI/180)&&(F_ds<-100))
		{
			if(flag_obs==0)
			{
				flag_obs=2;
				flag_say=1;
			}			
		}
		if(flag_hurt==1)
		{
			if((L_ves[1]<670)&&(L_ves[1]>L_ves0[1]))
			{
				flag_hurt=-1;
				flag_say=1;
			}			
		}	
		if(flag_hurt==2)
		{
			if((L_ves[1]>600)&&(L_ves[1]<L_ves0[1]))
			{
				flag_hurt=-2;
				flag_say=1;
			}		
		}	
		if((L_ves[1]>690)&&(F_ds>-10))
		{
			if(flag_hurt==0)
			{
				flag_hurt=1;
				flag_say=1;
			}			
		}
		if((L_ves[1]<560)&&(F_ds>-10))
		{
			if(flag_hurt==0)
			{
				flag_hurt=2;
				flag_say=1;
			}			
		}
		if(flag_hurt==3)
		{
			if(((L_ves[0]>120*M_PI/180)&&(L_ves[0]<150*M_PI/180))&&(F_ds>-10))
			{
				flag_hurt=-3;
				flag_say=1;			
			}	
		}	
		if(((L_ves[0]<110*M_PI/180)||(L_ves[0]>160*M_PI/180))&&(F_ds>-10))
		{
			if(flag_hurt==0)
			{
				flag_hurt=3;
				flag_say=1;
			}			
		}		
	//Converter		
	//Rotate
	vmob[1]=dL_ves[0];
	//Linear velocity		
	vmob[0]=-dL_ves[1]*Owr[0]/L_ves[1];//-dL_ves[1]/2;
	/*
	//Filter mobile velocity
	if(dt<0.001) dt=0.001;
	K_f=0.5/(2*M_PI);
	alpha_f=(dt)/(dt+K_f);
	vmob[0]=alpha_f*vmob[0]+(1-alpha_f)*vmobp[0];	
	vmobp[0]=vmob[0];
	vmob[1]=alpha_f*vmob[1]+(1-alpha_f)*vmobp[1];	
	vmobp[1]=vmob[1];
	*/	
	}else //Robot leads the way
	{

	}	
	dmob[0]=vmob[0];
	dmob[1]=vmob[1];
}
void CArosArm::fEndEffector(float *dmob,float *dpos,float tim,int mode)
{
	//float temp1,phi;
	float vend[6],vrot[3],F_ext[6],F_imp[6],temp;
	float err,dypr[3],dt,dist,Oend[3];	
	//mode=0->Hold and drag; mode=1->target heading; mode=2->transport;  mode=3-> Lift handle; mode=4->Take me around
	switch (mode)
	{
	case 0: //mode=0->Hold and drag
		Fmax[0]=30.0f;Fmax[1]=30.0f;Fmax[2]=30.0f;Fmax[3]=0.6f;Fmax[4]=0.6f;Fmax[5]=0.6f;
		Fmin[0]=2.0f;Fmin[1]=2.0f;Fmin[2]=2.0f;Fmin[3]=0.2f;Fmin[4]=0.2f;Fmin[5]=0.2f;		
		if((see_target==1)&&(Oef[2]<-50.0f))
		{
			get_target=1;
		}else
		{
			get_target=0;
		}
		for(int i=0;i<6;i++)
		{
			if(abs(fW[i])>Fmin[i])
			{
				if(fW[i]>0)
				{
					F_ext[i]=fW[i]-Fmin[i];
				}else
				{
					F_ext[i]=fW[i]+Fmin[i];
				}
				get_target=0;
			}else
			{
				F_ext[i]=0;
			}			
		}
		temp=sqrt(F_ext[0]*F_ext[0]+F_ext[1]*F_ext[1]+F_ext[2]*F_ext[2]);
		if(temp>30)
		{
			for(int i=0;i<3;i++)
			{
				F_ext[i]=30*F_ext[i]/temp;
			}
		}
		temp=sqrt(F_ext[3]*F_ext[3]+F_ext[4]*F_ext[4]+F_ext[5]*F_ext[5]);
		if(temp>0.6)
		{
			for(int i=3;i<6;i++)
			{
				F_ext[i]=0.6f*F_ext[i]/temp;
			}
		}		
		dt=tim-timIntegrate;	
		

		for(int i=0;i<3;i++)
		{
			F_imp[i]=F_ext[i];
		}
		F_imp[3]=cosf(ypr[0])/cosf(ypr[1])*F_ext[3]+sinf(ypr[0])/cosf(ypr[1])*F_ext[4];//Roll
		F_imp[4]=sinf(ypr[0])*F_ext[3]-cosf(ypr[0])*F_ext[4];//Pitch
		F_imp[5]=-cosf(ypr[0])*tanf(ypr[1])*F_ext[3]-sinf(ypr[0])*tanf(ypr[1])*F_ext[4]+F_ext[5];//Yaw

		k_force=10.0f;
		k_moment=0.8f;

		vend[0]=F_ext[0]*k_force;
		vend[1]=F_ext[1]*k_force;		
		vend[2]=F_ext[2]*k_force; 

		vend[3]=F_imp[3]*k_moment;//Roll
		//vend[3]=-0.5*(ypr[2]+90*M_PI/180); 
		
		vend[4]=-0.5f*(ypr[1]-10.0f*M_PI/180.0f); 	//Pitch
		//  110 < M_PI-jtheta_n[5]  <250
		vend[5]=F_ext[5]*k_moment; //Yaw
		temp=M_PI-jtheta_n[5];
		if((vend[5]>0)&&(temp>260*M_PI/180))
		{
			vend[5]=-0.01f;
		}
		if((vend[5]<0)&&(temp<80*M_PI/180))
		{
			vend[5]=0.01f;
		}
		
		break;
	case 1:		//mode=1->target heading
		// PID control roll angle =0
		Kpw=5.0f;
		Tiw=100.0f;
		Tdw=0.0001f;
		Nw=3.0f;
		dt=tim-timIntegrate;	
		err=-ypr[2]-M_PI/2.0f;	
		errYPR[2]=errYPR[2]+err*dt;
		dypr[2]=Kpw*err;//Kpw*(1*err+errYPR[2]/Tiw)+Nw*Tdw*(err-errYPRp[2])/(Nw*dt+Tdw)+Tdw*dyprp[2]/(Nw*dt+Tdw);
		errYPRp[2]=err;
		dyprp[2]=dypr[2];
		//Impedance
		k_force=8.0f;
		k_moment=0.5f;		
		Oend[0] = Oef[0] / 10.0f; Oend[1] = Oef[1] / 10.0f; Oend[2] = Oef[2] / 10.0f;
		// Estimate its coordinates
		//vmobp[0]=0;vmobp[1]=0;
		fCalculateTarget(vmobp,targetp,dt);
		fCopy(target,targetp,3);
		dist=sqrt((target[0]-Oend[0])*(target[0]-Oend[0])+(target[1]-Oend[1])*(target[1]-Oend[1])+(target[2]-Oend[2])*(target[2]-Oend[2]) );
		if(dist<1)
		{
			flag_grasp=1;			
		}
		for(int i=0;i<3;i++)
		{
			vend[i]=80.0f*(target[i]-Oend[i])/dist*(1-exp(-tim/2.0f) );
		}	
		if(abs(dypr[2])>0.3f)
		{
			dypr[2]=0.3f*dypr[2]/abs(dypr[2]);
		}
		vend[3]=dypr[2]; // Wroll
		vend[4]=0.0f; // Wpitch		
		vend[5]=0.0f; // Wyaw -0.05*(ypy[0]-target[3]);
		
		break;
	case 2:  //mode=2->transport
		Fmax[0]=30.0f;Fmax[1]=30.0f;Fmax[2]=30.0f;Fmax[3]=0.6f;Fmax[4]=0.6f;Fmax[5]=0.6f;
		Fmin[0]=2.0f;Fmin[1]=2.0f;Fmin[2]=2.0f;Fmin[3]=0.2f;Fmin[4]=0.2f;Fmin[5]=0.2f;
		finish=0;
		if(Oef[2]<-230.0f)
		{
			finish=1;
		}
		for(int i=0;i<6;i++)
		{
			if(abs(fW[i])>Fmin[i])
			{
				if(fW[i]>0.0f)
				{
					F_ext[i]=fW[i]-Fmin[i];
				}else
				{
					F_ext[i]=fW[i]+Fmin[i];
				}
			
			}else
			{
				F_ext[i]=0.0f;
			}			
		}
		temp=sqrt(F_ext[0]*F_ext[0]+F_ext[1]*F_ext[1]+F_ext[2]*F_ext[2]);
		if(temp>30.0f)
		{
			for(int i=0;i<3;i++)
			{
				F_ext[i]=30.0f*F_ext[i]/temp;
			}
		}
		temp=sqrt(F_ext[3]*F_ext[3]+F_ext[4]*F_ext[4]+F_ext[5]*F_ext[5]);
		if(temp>0.6f)
		{
			for(int i=3;i<6;i++)
			{
				F_ext[i]=0.6f*F_ext[i]/temp;
			}
		}		
		dt=tim-timIntegrate;	

		for(int i=0;i<3;i++)
		{
			F_imp[i]=F_ext[i];
		}
		F_imp[3]=cosf(ypr[0])/cosf(ypr[1])*F_ext[3]+sinf(ypr[0])/cosf(ypr[1])*F_ext[4];//Roll
		F_imp[4]=sinf(ypr[0])*F_ext[3]-cosf(ypr[0])*F_ext[4];//Pitch
		F_imp[5]=-cosf(ypr[0])*tanf(ypr[1])*F_ext[3]-sinf(ypr[0])*tanf(ypr[1])*F_ext[4]+F_ext[5];//Yaw

		k_force=10.0f;
		k_moment=0.8f;

		vend[0]=F_ext[0]*k_force;
		vend[1]=F_ext[1]*k_force;		
		vend[2]=F_ext[2]*k_force*2; 

		vend[3]=-0.5f*(ypr[2]+M_PI/2.0f);//Roll
		//vend[3]=-0.5*(ypr[2]+90*M_PI/180); 
		
		vend[4]=0.0f;//-0.5*(ypr[1]-10*M_PI/180);  //-0.5*(ypr[1]);	//Pitch
		//  110 < M_PI-jtheta_n[5]  <250
		vend[5]=F_ext[5]*k_moment; //Yaw
		temp=M_PI-jtheta_n[5];
		if((vend[5]>0)&&(temp>260.0f*M_PI/180.0f))
		{
			vend[5]=-0.01f;
		}
		if((vend[5]<0)&&(temp<80.0f*M_PI/180.0f))
		{
			vend[5]=0.01f;
		}
		
		break;
	case 3:	 // Lift the handle	
		
		vend[0]=0.0f;
		vend[1]=0.0f;
		if(tim<tims+5)
		{
			vend[2]=80.0f*(tim-tims)/5;
		}		
		if(tim>tims+5)
		{
			vend[2]=80.0f;
		}		
		vend[3]=0.0f; // Wroll
		vend[4]=0.0f; // Wpitch		
		vend[5]=0.0f; // Wyaw -0.05*(ypy[0]-target[3]);
		
		break;
	case 4:  //mode=4->Take me around
		Fmax[0]=30.0f;Fmax[1]=30.0f;Fmax[2]=30.0f;Fmax[3]=0.6f;Fmax[4]=0.6f;Fmax[5]=0.6f;
		Fmin[0]=2.0f;Fmin[1]=2.0f;Fmin[2]=2.0f;Fmin[3]=0.2f;Fmin[4]=0.2f;Fmin[5]=0.2f;		
		for(int i=0;i<6;i++)
		{
			if(abs(fW[i])>Fmin[i])
			{
				if(fW[i]>0)
				{
					F_ext[i]=fW[i]-Fmin[i];
				}else
				{
					F_ext[i]=fW[i]+Fmin[i];
				}
				get_target=0;
			}else
			{
				F_ext[i]=0;
			}			
		}
		temp=sqrt(F_ext[0]*F_ext[0]+F_ext[1]*F_ext[1]+F_ext[2]*F_ext[2]);
		if(temp>30)
		{
			for(int i=0;i<3;i++)
			{
				F_ext[i]=30*F_ext[i]/temp;
			}
		}
		temp=sqrt(F_ext[3]*F_ext[3]+F_ext[4]*F_ext[4]+F_ext[5]*F_ext[5]);
		if(temp>0.6)
		{
			for(int i=3;i<6;i++)
			{
				F_ext[i]=0.6f*F_ext[i]/temp;
			}
		}		
		dt=tim-timIntegrate;		

		for(int i=0;i<3;i++)
		{
			F_imp[i]=F_ext[i];
		}
		F_imp[3]=cosf(ypr[0])/cosf(ypr[1])*F_ext[3]+sinf(ypr[0])/cosf(ypr[1])*F_ext[4];//Roll
		F_imp[4]=sinf(ypr[0])*F_ext[3]-cosf(ypr[0])*F_ext[4];//Pitch
		F_imp[5]=-cosf(ypr[0])*tanf(ypr[1])*F_ext[3]-sinf(ypr[0])*tanf(ypr[1])*F_ext[4]+F_ext[5];//Yaw

		k_force=10.0f;
		k_moment=0.8f;

		vend[0]=F_ext[0]*k_force;
		vend[1]=F_ext[1]*k_force;		
		vend[2]=F_ext[2]*k_force; 

		//vend[3]=F_imp[3]*k_moment;//Roll	
		vend[3]=-0.5f*(ypr[2]+M_PI/2.0f);//Roll
		vend[4]=0.0f; 	//Pitch
		//  110 < M_PI-jtheta_n[5]  <250
		vend[5]=F_ext[5]*k_moment; //Yaw
		temp=M_PI-jtheta_n[5];
		if((vend[5]>0)&&(temp>260.0f*M_PI/180.0f))
		{
			vend[5]=-0.01f;
		}
		if((vend[5]<0)&&(temp<80.0f*M_PI/180.0f))
		{
			vend[5]=0.01f;
		}		
		break;		
	}	
	//vmob[1]=0;
	//vmob[0]=0;
	//Compensation
	if(flag_comp==1)
	{
		vrot[0]=Oef[1];
		vrot[1]=-Oef[0];
		vrot[2]=0.0f;

		vend[0]=vend[0]+vrot[0]*dmob[1]-dmob[0];
		vend[1]=vend[1]+vrot[1]*dmob[1];
		vend[5]=vend[5]-dmob[1];
	}	
	fCopy(dpos,vend,6);	
	dmob[0]=dmob[0]/1000.0f;
	dmob[1]=dmob[1];
	vmobp[0]=dmob[0];
	vmobp[1]=dmob[1];
}
void CArosArm::fMobileDriving(float *dmob)
{
	//float sensor_angle;
	float vmob[2],lamda,sigma,temp,temp1,dist;
	
	M_ds=0.0f;//Rotation
	F_ds=0.0f;//Linear	
	
	for(int i=0;i<sensor_number;i++)
	{
		if((i==25)||(i==26)||(i==27)||(i==58)||(i==59)||(i==60)||(i==61)) continue;
			
		//Obstacle avoidance
		if(obs_angle[i]<0)
		{
			lamda=2.0f*exp(-(obs_g[i]-50.0f)/20.0f);	
		}else
		{
			lamda=1.0f*exp(-(obs_g[i]-50.0f)/20.0f);		
		}		
		sigma=atan(tan(sensor_sector/2)+rrobot/obs_g[i]);
		temp=-lamda*obs_angle[i]*exp(-obs_angle[i]*obs_angle[i]/(2*sigma*sigma));
		M_ds=M_ds+temp;
		temp1=200.0f*exp(-obs_g[i]/20.0f);
		temp=-temp1*cos(obs_angle[i]);

		F_ds=F_ds+temp;
	}
	dist=sqrt(target[0]*target[0]+target[1]*target[1]);
	temp=ltar*target[1]/dist;
	vmob[1]=M_ds+temp;	
	if(abs(vmob[1])>0.25f)
	{
		vmob[1]=0.25f*vmob[1]/abs(vmob[1]);
	}
	vmob[0]=0.0f;//30+F_ds;
	vmob[1]=M_ds;	

	dmob[0]=vmob[0]/1000.0f;
	dmob[1]=vmob[1];
}
void CArosArm::fReaction(float *dpos, int *tomove)
{
	float F_ext[6],F_imp[6],temp;
	tomove[0]=0;
	
	Fmax[0]=20.0f;Fmax[1]=20.0f;Fmax[2]=20.0f;Fmax[3]=0.6f;Fmax[4]=0.6f;Fmax[5]=0.6f;
	Fmin[0]=2.0f;Fmin[1]=2.0f;Fmin[2]=2.0f;Fmin[3]=0.2f;Fmin[4]=0.2f;Fmin[5]=0.2f;
			
	for(int i=0;i<6;i++)
	{
		if(abs(fW[i])>Fmin[i])
		{
			tomove[0]=1;
			if(fW[i]>0.0f)
			{
				F_ext[i]=fW[i]-Fmin[i];
			}else
			{
				F_ext[i]=fW[i]+Fmin[i];
			}
			
		}else
		{
			F_ext[i]=0.0f;
		}			
	}
	temp=sqrt(F_ext[0]*F_ext[0]+F_ext[1]*F_ext[1]+F_ext[2]*F_ext[2]);
	if(temp>20.0f)
	{
		for(int i=0;i<3;i++)
		{
			F_ext[i]=20.0f*F_ext[i]/temp;
		}
	}
	temp=sqrt(F_ext[3]*F_ext[3]+F_ext[4]*F_ext[4]+F_ext[5]*F_ext[5]);
	if(temp>0.6f)
	{
		for(int i=3;i<6;i++)
		{
			F_ext[i]=0.6f*F_ext[i]/temp;
		}
	}

	if(flag_ypr==0)
	{
		k_force=10.0f;
		k_moment=0.8f;
		dpos[0]=F_ext[0]*k_force;
		dpos[1]=F_ext[1]*k_force;
		dpos[2]=F_ext[2]*k_force;
		dpos[3]=F_ext[3]*k_moment;
		dpos[4]=F_ext[4]*k_moment;
		dpos[5]=F_ext[5]*k_moment;	
	}else
	{

		for(int i=0;i<3;i++)
		{
			F_imp[i]=F_ext[i];
		}
		F_imp[3]=cosf(ypr[0])/cosf(ypr[1])*F_ext[3]+sinf(ypr[0])/cosf(ypr[1])*F_ext[4];//Roll
		F_imp[4]=sinf(ypr[0])*F_ext[3]-cosf(ypr[0])*F_ext[4];//Pitch
		F_imp[5]=-cosf(ypr[0])*tanf(ypr[1])*F_ext[3]-sinf(ypr[0])*tanf(ypr[1])*F_ext[4]+F_ext[5];//Yaw

		k_force=10.0f;
		k_moment=0.8f;

		dpos[0]=F_ext[0]*k_force;
		dpos[1]=F_ext[1]*k_force;		
		dpos[2]=F_ext[2]*k_force;
		dpos[3]=F_imp[3]*k_moment;//Roll		
		dpos[4]=-0.5f*(ypr[1]-0.0f*M_PI/180.0f); //Pitch
		dpos[5]=F_ext[5]*k_moment; //Yaw		
	}
}
void CArosArm::fPanTilt(float *tar,float *pan, float *tilt)
{
	float dist;
	// x:+20  z:+10
	dist=sqrt((tar[0]-20)*(tar[0]-20)+tar[1]*tar[1]+(tar[2]-10)*(tar[2]-10) );
	pan[0]=atan2f(tar[1],tar[0]-20)*180/M_PI;// -150  150
	tilt[0]=asinf((tar[2]-10)/dist)*180/M_PI-15;  // -45  30
	if(pan[0]>150) pan[0]=150;
	if(pan[0]<-150) pan[0]=-150;
	if(tilt[0]>30) tilt[0]=30;
	if(tilt[0]<-45) tilt[0]=-45;
}
void CArosArm::fCalculateTarget(float *dmob,float *pos,float dtim)
{
	float dpos[2];	
	dpos[0]=-dmob[0]*dtim*100;
	dpos[1]=-dmob[1]*dtim;
	float data[16]={cosf(dpos[1]),-sinf(dpos[1]),0,dpos[0],sinf(dpos[1]),cos(dpos[1]),0,0, 0,0,1,0, 0,0,0,1};
	CMatrix<float> TransMatrix(4,4,data),tar(4,1),result(4,1);
	tar(0,0)=pos[0];
	tar(1,0)=pos[1];
	tar(2,0)=0;
	tar(3,0)=1;
	TransMatrix.Mul(&tar,&result);
	pos[0]=result(0,0);
	pos[1]=result(1,0);
}

void CArosArm::fEndEffectorInfo(float *jtheta,float *X7, float *Y7, float *Z7)
{
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T07(4,4);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T67,&T07);
	for(int i=0;i<3;i++)
	{
		X7[i]=T07.get_Element(i,0);
		Y7[i]=T07.get_Element(i,1);
		Z7[i]=T07.get_Element(i,2);
	}
}
void CArosArm::fEndEffectorInfo(float *jtheta,float *O7)
{
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T07(4,4);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T67,&T07);
	for(int i=0;i<3;i++)
	{
		O7[i]=T07.get_Element(i,3)/10;
	}
}
void CArosArm::fPosition(float *jtheta,float *shoulder,float *elbow,float *wrist,float *hand)
{
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T07(4,4);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T67,&T07);
	//Get S,E,W,H position in cm
	shoulder[0]=T01.get_Element(0,3)/10;
	shoulder[1]=T01.get_Element(1,3)/10;
	shoulder[2]=T01.get_Element(2,3)/10;
	elbow[0]=T03.get_Element(0,3)/10;
	elbow[1]=T03.get_Element(1,3)/10;
	elbow[2]=T03.get_Element(2,3)/10;
	wrist[0]=T05.get_Element(0,3)/10;
	wrist[1]=T05.get_Element(1,3)/10;
	wrist[2]=T05.get_Element(2,3)/10;
	hand[0]=T07.get_Element(0,3)/10;
	hand[1]=T07.get_Element(1,3)/10;
	hand[2]=T07.get_Element(2,3)/10;
}
float CArosArm::fprod(float *V1,float *V2,float *V3,float *V4)
{
	float result=(V1[0]-V2[0])*(V3[0]-V4[0])+(V1[1]-V2[1])*(V3[1]-V4[1])+(V1[2]-V2[2])*(V3[2]-V4[2]);
	return result;
}
void CArosArm::fcrossprod(float *V1,float *V2, float *result)
{
	float r[3];
	r[0]=V1[1]*V2[2]-V2[1]*V1[2];
	r[1]=V1[2]*V2[0]-V2[2]*V1[0];
	r[2]=V1[0]*V2[1]-V2[0]*V1[1];
	result[0]=r[0];
	result[1]=r[1];
	result[2]=r[2];
}
void CArosArm::fCopy(float *dist,float *source,int length)
{
	for (int i=0;i<length;i++ )
		dist[i]=source[i];
}
void CArosArm::fCopyMatrix(CMatrix<float> *dist,CMatrix<float> *source,int rowd,int cold,int rows0,int cols0,int rows1,int cols1)
{
	int i=0,j,m,n;
	for(m=rows0;m<=rows1;m++ )
	{
		j=0;
		for(n=cols0;n<=cols1;n++ )
		{
			dist->set_Element(rowd+i,cold+j,source->get_Element(m,n));
			j++;
		}
		i++;
	}
		
}
//------------------------------------Kinematics----------------------------------
//------------Direct Geometric kinematrics
void CArosArm::GeoKine(float *jtheta,float *pos,float *ypr0)
{
	float s_b,c_b;
	CMatrix<float> T01(4,4),T12(4,4),T23(4,4),T34(4,4),T45(4,4),T56(4,4),T67(4,4),T02(4,4),T03(4,4),T04(4,4),T05(4,4),T06(4,4),T07(4,4);
	T01=fTransMatrix(0,M_PI /2,L1,jtheta[0]+joint_offset[0]);
	T12=fTransMatrix(0,M_PI /2,0,jtheta[1]+joint_offset[1]);
	T23=fTransMatrix(0,-M_PI /2,L3,jtheta[2]+joint_offset[2]);
	T34=fTransMatrix(0,M_PI /2,0,jtheta[3]+joint_offset[3]);
	T45=fTransMatrix(0,-M_PI /2,L5,jtheta[4]+joint_offset[4]);
	T56=fTransMatrix(0,M_PI /2,0,jtheta[5]+joint_offset[5]);
	T67=fTransMatrix(0,-M_PI /2,L7,jtheta[6]+joint_offset[6]);
	T01.Mul(&T12,&T02);
	T02.Mul(&T23,&T03);
	T03.Mul(&T34,&T04);
	T04.Mul(&T45,&T05);
	T05.Mul(&T56,&T06);
	T06.Mul(&T67,&T07);
	pos[0]=T07.get_Element(0,3);
	pos[1]=T07.get_Element(1,3);
	pos[2]=T07.get_Element(2,3);
	// Euler fixed angle
	s_b=-T07.get_Element(2,0);
	c_b=sqrtf(1-s_b*s_b);
	if (c_b>0.01)
	{
		pos[4]=atan2f(s_b,c_b);
		pos[3]=atan2f(T07.get_Element(1,0)/c_b,T07.get_Element(0,0)/c_b);
		pos[5]=atan2f(T07.get_Element(2,1)/c_b,T07.get_Element(2,2)/c_b);
	}else
	{
		if(s_b>0)
		{
			pos[4]=M_PI/2;
			pos[3]=0;
			pos[5]=atan2f(T07.get_Element(0,1),T07.get_Element(1,1));
		}else
		{
			pos[4]=-M_PI/2;
			pos[3]=0;
			pos[5]=-atan2f(T07.get_Element(0,1),T07.get_Element(1,1));
		}
	}	
	// Yaw Pitch Roll
	s_b=T07.get_Element(2,2);
	c_b=sqrtf(1-s_b*s_b);
	if (c_b>0.01)
	{
		ypr0[1]=atan2f(s_b,c_b);
		ypr0[0]=atan2f(T07.get_Element(1,2)/c_b,T07.get_Element(0,2)/c_b);
		ypr0[2]=atan2f(-T07.get_Element(2,1)/c_b,T07.get_Element(2,0)/c_b);
	}else
	{
		if(s_b>0)
		{
			ypr0[1]=M_PI/2;
			ypr0[0]=0;
			ypr0[2]=atan2f(T07.get_Element(0,1),-T07.get_Element(1,1));
		}else
		{
			ypr0[1]=-M_PI/2;
			ypr0[0]=0;
			ypr0[2]=atan2f(-T07.get_Element(0,1),-T07.get_Element(1,1));
		}
	}	
	
}
//------------Inverse Geometric kinematrics
int CArosArm::InvGeoKine(float* pos,float* jtheta,float* planeangle,int flag,int mode)
{
	float alpha,beta,gamma,xe,ye,ze,L_SW,c4,s2,vnorm,SC,R,s6,plane_angle;
	float W[3],Center[3],E[3];
	float jtheta1[7],jtheta2[7];
	int a,b, notfound;
	//float exam;
	CMatrix<float> vtemp(1,3),plane_u(1,3),plane_v(1,3),x4(1,3),y4(1,3),z4(1,3),temp(3,3),R04(3,3),R07(3,3),R47(3,3);
	xe=pos[0];ye=pos[1];ze=pos[2];alpha=pos[3];beta=pos[4];gamma=pos[5];
	if(mode==0)
	{	
		R07.set_Element(0,0,cosf(alpha)*cosf(beta));R07.set_Element(0,1,cosf(alpha)*sinf(beta)*sinf(gamma)-sinf(alpha)*cosf(gamma));R07.set_Element(0,2,cosf(alpha)*sinf(beta)*cosf(gamma)+sinf(alpha)*sinf(gamma));
		R07.set_Element(1,0,sinf(alpha)*cosf(beta));R07.set_Element(1,1,sinf(alpha)*sinf(beta)*sinf(gamma)+cosf(alpha)*cosf(gamma));R07.set_Element(1,2,sinf(alpha)*sinf(beta)*cosf(gamma)-cosf(alpha)*sinf(gamma));
		R07.set_Element(2,0,-sinf(beta));R07.set_Element(2,1,cosf(beta)*sinf(gamma));R07.set_Element(2,2,cosf(beta)*cosf(gamma));
	}
	else
	{
		R07.set_Element(0,0,-cosf(alpha)*sinf(beta)*cosf(gamma)+sinf(alpha)*sinf(gamma) );R07.set_Element(0,1,cosf(alpha)*sinf(beta)*sinf(gamma)+sinf(alpha)*cosf(gamma));R07.set_Element(0,2,cosf(alpha)*cosf(beta));
		R07.set_Element(1,0,-sinf(alpha)*sinf(beta)*cosf(gamma)-cosf(alpha)*sinf(gamma) );R07.set_Element(1,1,sinf(alpha)*sinf(beta)*sinf(gamma)-cosf(alpha)*cosf(gamma));R07.set_Element(1,2,sinf(alpha)*cosf(beta));
		R07.set_Element(2,0,cosf(beta)*cosf(gamma));R07.set_Element(2,1,-cosf(beta)*sinf(gamma));R07.set_Element(2,2,sinf(beta));
	}
	W[0]=xe-L7*R07.get_Element(0,2);
	W[1]=ye-L7*R07.get_Element(1,2);
	W[2]=ze-L7*R07.get_Element(2,2);
	L_SW=sqrtf(W[0]*W[0]+(W[1]+L1)*(W[1]+L1)+W[2]*W[2]);
	c4=(L_SW*L_SW-L3*L3-L5*L5)/(2*L3*L5);
	if ((c4<=1)&&(c4>=-1))
	{
		
		jtheta1[3]=acosf(c4);
		if(jtheta1[3]>0) jtheta1[3]=-jtheta1[3];
		if(fCheckRange(jtheta1[3],3)==0)
		{
			if(fCheckRange(-jtheta1[3],3)==1)
				jtheta1[3]=-jtheta1[3];
			else
				// Joint 4 is out of range
				return -1;
		};
		plane_u.set_Element(0,0,W[0]);plane_u.set_Element(0,1,W[1]+L1);plane_u.set_Element(0,2,W[2]);
		vtemp.set_Element(0,0,0);vtemp.set_Element(0,1,0);vtemp.set_Element(0,2,1);
		plane_u.cross(&vtemp,&plane_u);
		//vnorm=sqrtf(plane_u.get_Element(0,0)*plane_u.get_Element(0,0)+plane_u.get_Element(0,1)*plane_u.get_Element(0,1)+plane_u.get_Element(0,2)*plane_u.get_Element(0,2));
		vnorm=plane_u.norm();
		plane_u.set_Element(0,0,plane_u.get_Element(0,0)/vnorm);plane_u.set_Element(0,1,plane_u.get_Element(0,1)/vnorm);plane_u.set_Element(0,2,plane_u.get_Element(0,2)/vnorm);
		vtemp.set_Element(0,0,W[0]);vtemp.set_Element(0,1,W[1]+L1);vtemp.set_Element(0,2,W[2]);
		plane_u.cross(&vtemp,&plane_v);
		//vnorm=sqrtf(plane_v.get_Element(0,0)*plane_v.get_Element(0,0)+plane_v.get_Element(0,1)*plane_v.get_Element(0,1)+plane_v.get_Element(0,2)*plane_v.get_Element(0,2));
		vnorm=plane_v.norm();
		plane_v.set_Element(0,0,plane_v.get_Element(0,0)/vnorm);plane_v.set_Element(0,1,plane_v.get_Element(0,1)/vnorm);plane_v.set_Element(0,2,plane_v.get_Element(0,2)/vnorm);
		SC=L3*(L3*L3+L_SW*L_SW-L5*L5)/(2*L3*L_SW);    
		R=sqrtf(L3*L3-SC*SC);
		Center[0]=SC*W[0]/L_SW;Center[1]=-L1+SC*(W[1]+L1)/L_SW;Center[2]=SC*W[2]/L_SW;
		notfound=1;
		if(flag==1) plane_angle=0;
		else plane_angle=planeangle[0];
		while(notfound)
		{
			notfound=0;
			float A=R*plane_u.get_Element(0,0)*cosf(plane_angle);
			A=plane_v.get_Element(0,0)*sinf(plane_angle);
			E[0]=Center[0]+R*(plane_u.get_Element(0,0)*cosf(plane_angle)-plane_v.get_Element(0,0)*sinf(plane_angle));
			E[1]=Center[1]+R*(plane_u.get_Element(0,1)*cosf(plane_angle)-plane_v.get_Element(0,1)*sinf(plane_angle));
			E[2]=Center[2]+R*(plane_u.get_Element(0,2)*cosf(plane_angle)-plane_v.get_Element(0,2)*sinf(plane_angle));
			y4.set_Element(0,0,W[0]-E[0]);y4.set_Element(0,1,W[1]-E[1]);y4.set_Element(0,2,W[2]-E[2]);
			vtemp.set_Element(0,0,E[0]);vtemp.set_Element(0,1,E[1]+L1);vtemp.set_Element(0,2,E[2]);
			y4.cross(&vtemp,&z4);			
			//vnorm=sqrtf(z4.get_Element(0,0)*z4.get_Element(0,0)+z4.get_Element(0,1)*z4.get_Element(0,1)+z4.get_Element(0,2)*z4.get_Element(0,2));
			vnorm=z4.norm();
			z4.set_Element(0,0,z4.get_Element(0,0)/vnorm);z4.set_Element(0,1,z4.get_Element(0,1)/vnorm);z4.set_Element(0,2,z4.get_Element(0,2)/vnorm);
			//vnorm=sqrtf(y4.get_Element(0,0)*y4.get_Element(0,0)+y4.get_Element(0,1)*y4.get_Element(0,1)+y4.get_Element(0,2)*y4.get_Element(0,2));
			vnorm=y4.norm();
			y4.set_Element(0,0,y4.get_Element(0,0)/vnorm);y4.set_Element(0,1,y4.get_Element(0,1)/vnorm);y4.set_Element(0,2,y4.get_Element(0,2)/vnorm);
			y4.cross(&z4,&x4);				
			temp.set_Element(0,0,cosf(jtheta1[3]));temp.set_Element(1,0,-sinf(jtheta1[3]));temp.set_Element(2,0,0);
			temp.set_Element(0,1,0);temp.set_Element(1,1,0);temp.set_Element(2,1,-1);
			temp.set_Element(0,2,sinf(jtheta1[3]));temp.set_Element(1,2,cosf(jtheta1[3]));temp.set_Element(2,2,0);
			R04.set_Element(0,0,x4.get_Element(0,0));R04.set_Element(0,1,y4.get_Element(0,0));R04.set_Element(0,2,z4.get_Element(0,0));
			R04.set_Element(1,0,x4.get_Element(0,1));R04.set_Element(1,1,y4.get_Element(0,1));R04.set_Element(1,2,z4.get_Element(0,1));
			R04.set_Element(2,0,x4.get_Element(0,2));R04.set_Element(2,1,y4.get_Element(0,2));R04.set_Element(2,2,z4.get_Element(0,2));
			R04.Transpose(&R47);
			R04.Mul(&temp);
			jtheta2[3]=jtheta1[3];
			//------------------------
			jtheta1[1]=atan2f(sqrtf(1-R04.get_Element(1,2)*R04.get_Element(1,2)),-R04.get_Element(1,2));
			s2=sin(jtheta1[1]);
			if((s2>0.01)||(s2<-0.01))
			{
				jtheta1[0]=atan2f(-R04.get_Element(2,2)/s2,-R04.get_Element(0,2)/s2);
				jtheta1[2]=atan2f(R04.get_Element(1,1)/s2,-R04.get_Element(1,0)/s2);
			}else
			{
				jtheta1[0]=0;
				jtheta1[2]=atan2f(R04.get_Element(2,0),R04.get_Element(0,0));
			};//--------------------------
			jtheta2[1]=atan2f(-sqrtf(1-R04.get_Element(1,2)*R04.get_Element(1,2)),-R04.get_Element(1,2));
			s2=sin(jtheta2[1]);
			if((s2>0.01)||(s2<-0.01))
			{
				jtheta2[0]=atan2f(-R04.get_Element(2,2)/s2,-R04.get_Element(0,2)/s2);
				jtheta2[2]=atan2f(R04.get_Element(1,1)/s2,-R04.get_Element(1,0)/s2);
			}else
			{
				jtheta2[0]=0;
				jtheta2[2]=atan2f(R04.get_Element(2,0),R04.get_Element(0,0));
			};//--------------------------
			a = fCheckRange(jtheta1[0],0)*fCheckRange(jtheta1[1],1)*fCheckRange(jtheta1[2],2);
			b = fCheckRange(jtheta2[0],0)*fCheckRange(jtheta2[1],1)*fCheckRange(jtheta2[2],2);
			if ((a==1)&&(b==1))
			{
				if((abs(jtheta1[1])+abs(jtheta1[2])+abs(jtheta1[3]))>(abs(jtheta2[1])+abs(jtheta2[2])+abs(jtheta2[3])))
				{
					fCopy(jtheta1,jtheta2,7);   
				};
			}else
				{
					if(b==1)
					fCopy(jtheta1,jtheta2,7);
					else
					{
						if(a==0) 
						{
							notfound=1; //Joint 1,2,3 out of range  
							if(flag==0) return -2; 
						}
					};
				};//-------------------------------
			fCopy(jtheta2,jtheta1,7);			
			R47.Mul(&R07);			
			jtheta1[5]=atan2f(sqrt(1-R47.get_Element(1,2)*R47.get_Element(1,2)),R47.get_Element(1,2));
			s6=sinf(jtheta1[5]);
			if ((s6>0.01)||(s6<-0.01))
			{
				jtheta1[4]=atan2f(R47.get_Element(2,2)/s6,-R47.get_Element(0,2)/s6);
				jtheta1[6]=atan2f(-R47.get_Element(1,1)/s6,R47.get_Element(1,0)/s6);
			}else
			{
				jtheta1[4]=0;
				jtheta1[6]=atan2(-R47.get_Element(2,0),R47.get_Element(0,0));
			};//---------------------
			jtheta2[5]=atan2f(-sqrt(1-R47.get_Element(1,2)*R47.get_Element(1,2)),R47.get_Element(1,2));
			s6=sinf(jtheta2[5]);
			if ((s6>0.01)||(s6<-0.01))
			{
				jtheta2[4]=atan2f(R47.get_Element(2,2)/s6,-R47.get_Element(0,2)/s6);
				jtheta2[6]=atan2f(-R47.get_Element(1,1)/s6,R47.get_Element(1,0)/s6);
			}else
			{
				jtheta2[4]=0;
				jtheta2[6]=atan2(-R47.get_Element(2,0),R47.get_Element(0,0));
			};//--------------------
			a=fCheckRange(jtheta1[4],4)*fCheckRange(jtheta1[5],5)*fCheckRange(jtheta1[6],6);
			b=fCheckRange(jtheta2[4],4)*fCheckRange(jtheta2[5],5)*fCheckRange(jtheta2[6],6);
			if ((a==1)&&(b==1))
			{
				if((abs(jtheta1[4])+abs(jtheta1[5])+abs(jtheta1[6]))>(abs(jtheta2[4])+abs(jtheta2[5])+abs(jtheta2[6])))
				{
					//memcpy(jtheta1,jtheta2,7*sizeof(float));
					fCopy(jtheta1,jtheta2,7);
				};
			}else
				{
					if(b==1)
					fCopy(jtheta1,jtheta2,7);
					else
					{
						if(a==0) {
							notfound=1; //Joint 5,6,7 out of range   
							if(flag==0) return -3; 
						}
					};
				};
			if((plane_angle<10*M_PI/9)&&(flag==1)&&(notfound!=0)) plane_angle=plane_angle+20*M_PI/180;
			
		}//-------------------------------
		if(notfound==0)
		{
		fCopy(jtheta,jtheta1,7);
		jtheta[1]=jtheta[1]-joint_offset[1];
		jtheta[3]=jtheta[3]-joint_offset[3];
		planeangle[0]=plane_angle;
		return 1;//Found solution
		}
		return -4;//No solution
	}else
	{
		//Not in the workspace
		return 0;
	}
	
}
//------------Direct Differential kinematrics
void CArosArm::DiffKine(float *jtheta,float* djtheta,float* dpos)
{
	CMatrix<float> JacobianFull(6,7),Jacobian(6,7),V(6,1);
	CMatrix<float> dJoint(7,1,djtheta),RPYTrans(6,6);
	//float data[42];
	JacobianFull=fJacobian(jtheta);
	if(flag_ypr==1)
	{// Roll pitch yaw
		RPYTrans=fTransRPY();
		RPYTrans.Mul(&JacobianFull,&JacobianFull);
	}		
	JacobianFull.Mul(&dJoint,&V);
	V.get_DataArray(dpos);
}
//------------Inverse Differential kinematrics
int CArosArm::InvDiffKine(float* dpos,float *jtheta,float* djtheta,int *mask, float tim)
{
	int cond=1;
	for(int i=0;i<7;i++)
	{
		cond=cond*fCheckRangeAbsolute(jtheta[i],i);
		if(cond==0) {	
			//Joint limits exceed
			return -25;			
		}
	}
	// Internal variables
	CMatrix<float> JacobianFull(6,7),dneed(7,1),RPYTrans(6,6);
	int count=0;
	float determinant,criterion,temp,temp1,coeff,coeff_filter,dtim;
	float Sh[3],El[3],Wr[3],Ha[3];
	float Ob[3],Ob_s;
	float Sh1[3],El1[3],Wr1[3],Ha1[3],OO[3];
	float X7[3],Y7[3],Z7[3],X71[3],Y71[3],Z71[3];
	float vj7;
	float dEl[7][3],dWr[7][3]; 
	float jtheta1[7],dnull[7];
	JacobianFull=fJacobian(jtheta);	
	//Mask all velocity components that doesn't need to be controlled
	for(int i=0;i<6;i++)
	{
		if(mask[i]==1) count++;		
	}

	CMatrix<float> Jacobian(count,7),J(count,count),JT(7,count),Jt(7,count),Jinv(count,count),Jnull(7,7);
	CMatrix<float> V(count,1),dnulls(7,1),dnullj(7,1),dnulloE(7,1),dnulloW(7,1),dnulloSE(7,1),dnulloEW(7,1),dnulloWH(7,1),grad_criterion(7,1);
	
	if(flag_ypr==1)
	{// Roll pitch yaw , from geometric jacobian to analytic jacobian that is necessary to follow the desidered velocity
		RPYTrans=fTransRPY();
		RPYTrans.Mul(&JacobianFull,&JacobianFull); // Ja=Ta(rpy)*Jg
	}
	count=0;
	for(int i=0;i<6;i++)
		{
			if(mask[i]==1)
			{
				fCopyMatrix(&Jacobian,&JacobianFull,count,0,i,0,i,6);				
				V(count,0)=dpos[i];
				count++;
			}
		}
	//--------------	
	Jacobian.Transpose(&JT);	
	Jacobian.Mul(&JT,&J);	
	determinant=J.Determinant();
	if(abs(determinant)<1000000)
	{
		// Singularity
		for(int i=0;i<7;i++)	djtheta[i]=0;
		return -10;
	}else
	{
		J.Inverse(&Jinv);
		JT.Mul(&Jinv,&Jt);
		Jt.Mul(&Jacobian,&Jnull);
		for(int i=0;i<7;i++)
		{
			for(int j=0;j<7;j++)
			{
				if(i==j) Jnull(i,j)=1-Jnull(i,j);
				else Jnull(i,j)=-Jnull(i,j);
			}
		}		
		Jt.Mul(&V,&dneed);
		//---Avoid singularity
		if(singEnable==1)
		{
			temp=sqrt(J.Determinant());
			//Calculate gradient of criterion
			for(int i=0;i<7;i++)
			{
				fCopy(jtheta1,jtheta,7);
				jtheta1[i]=jtheta1[i]+0.001f;
				JacobianFull=fJacobian(jtheta1);
				count=0;
				for(int j=0;j<7;j++)
				{
					if(mask[j]==1)
					{
						fCopyMatrix(&Jacobian,&JacobianFull,count,0,j,0,j,6);			
						count++;
					}
				}
				Jacobian.Transpose(&JT);
				Jacobian.Mul(&JT,&J);
				grad_criterion(i,0)=Ks1*(sqrt(J.Determinant())-temp)*1000.0f*pow(10.0f,-8);
			}
			Jnull.Mul(&grad_criterion,&dnulls);
			//Get max absolute of dnulls
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulls(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ks2*(1.0f-exp(-(temp)/Ks3))/(temp+0.001f)/(1.0f+exp(-(tim-tims)/2.0f+2.0f));
			dnulls.Mul(coeff,&dnulls);			
		}		
		//---Avoid joint limit
		if(jlimEnable==1)
		{
			for(int i=0;i<7;i++)
			{
				/*
				if(i==3) 
				{
					if(jtheta[i]<M_PI/2)
					{
						temp=(jtheta[i]-(joint_limit[0][i]+M_PI/2)/2)/(M_PI/2-joint_limit[0][i]);
					}else
					{
						temp=(jtheta[i]-(joint_limit[1][i]+M_PI/2)/2)/(joint_limit[1][i]-M_PI/2);
					}
				}else
				{
					temp=(jtheta[i]-(joint_limit[0][i]+joint_limit[1][i])/2)/(joint_limit[1][i]-joint_limit[0][i]);
				}
				*/
				temp=(jtheta[i]-(joint_limit[0][i]+joint_limit[1][i])/2)/(joint_limit[1][i]-joint_limit[0][i]);
				//temp=-(1+exp((temp-Kj1)/Kj2))*temp;
				grad_criterion(i,0)=-temp;
			}			
			Jnull.Mul(&grad_criterion,&dnullj);
			//Get max absolute of dnullj
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnullj(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Kj3*(1-exp(-(temp)/Kj4))/(temp+0.001f)/(1+exp(-(tim-tims)/2.0f+2.0f));
			dnullj.Mul(coeff,&dnullj);			
		}; 

		//Modified end-effecter orientation
		if(control_e==1)
		{
			fEndEffectorInfo(jtheta,X7,Y7,Z7);
			fCopy(jtheta1,jtheta,7);
			jtheta1[6]=jtheta1[6]+0.001f;
			fEndEffectorInfo(jtheta1,X71,Y71,Z71);
			temp=(Y71[2]-Y7[2])*1000.0f;
			vj7=temp*(0.5f- 1.0f/(1.0f+exp(-Y7[2]/0.2f) ));

		}
	
		//Avoid Osbtacle
		if(obsEnable==1)
		{
			fPosition(jtheta,Sh,El,Wr,Ha);
			for(int i=0;i<7;i++)
			{		
				fCopy(jtheta1,jtheta,7);
				jtheta1[i]=jtheta1[i]+0.001f;
				fPosition(jtheta1,Sh1,El1,Wr1,Ha1);
				dEl[i][0]=(El1[0]-El[0])*1000.0f;
				dEl[i][1]=(El1[1]-El[1])*1000.0f;
				dEl[i][2]=(El1[2]-El[2])*1000.0f;
				dWr[i][0]=(Wr1[0]-Wr[0])*1000.0f;
				dWr[i][1]=(Wr1[1]-Wr[1])*1000.0f;
				dWr[i][2]=(Wr1[2]-Wr[2])*1000.0f;
			}
			OO[0]=0;OO[1]=0;OO[2]=0;
			//Low pass filter for gradients
			dtim=tim-timp;
			//coeff_filter=dtim/(dtim+K_filter);
			//K_fr=coeff_filter;
			if(dtim<0.001f) dtim=0.001f;
			timp=tim;
			//-----Avoid Elbow
			//-----Gradient of criterion
			for(int i=0;i<7;i++)
			{		
				grad_criterion(i,0)=0;				
			}
			for(int i=0;i<obs_num;i++)
			{
				Ob[0]=obs_array[i][0];Ob[1]=obs_array[i][1];Ob[2]=obs_array[i][2];Ob_s=obs_array[i][3];
				if(obs_f[i]==1)
				{
					criterion=sqrt(fprod(Ob,El,Ob,El));					
					for(int j=0;j<7;j++)
					{		
						El1[0]=dEl[j][0];El1[1]=dEl[j][1];El1[2]=dEl[j][2];
						temp=Ko1/((Koff1+criterion)*(Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(fprod(El,Ob,El1,OO));					
						grad_criterion(j,0)=grad_criterion(j,0)+temp*4/(1+exp(2*criterion/(Kor*Ob_s)-2));
					}	
				}
			}
			//Low pass filter
			//Get max absolute of differenence between new and old grad
			
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(grad_criterion(i,0)-gradp[i][0]);
				if(temp<temp1) temp=temp1;
			}
			temp1=K_fm*(1-exp(-temp/K_fr));
			coeff_filter=dtim/(dtim+temp1);
			
			for(int i=0;i<7;i++)
			{
				gradp[i][0]=coeff_filter*grad_criterion(i,0)+(1-coeff_filter)*gradp[i][0];
				grad_criterion(i,0)=gradp[i][0];
			}
			Jnull.Mul(&grad_criterion,&dnulloE);
			//Get max absolute of dnulloE
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulloE(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ko2*(1.0f-exp(-(temp)/Ko3))/(temp+0.001f);
			dnulloE.Mul(coeff,&dnulloE);
			//-----Avoid Wrist
			//-----Gradient of criterion
			for(int i=0;i<7;i++)
			{		
				grad_criterion(i,0)=0;				
			}			
			for(int i=0;i<obs_num;i++)
			{
				Ob[0]=obs_array[i][0];Ob[1]=obs_array[i][1];Ob[2]=obs_array[i][2];Ob_s=obs_array[i][3];
				if(obs_f[i]==1)
				{
					criterion=sqrt(fprod(Ob,Wr,Ob,Wr));					
					for(int j=0;j<7;j++)
					{		
						Wr1[0]=dWr[j][0];Wr1[1]=dWr[j][1];Wr1[2]=dWr[j][2];
						temp=Ko1/((Koff1+criterion)*(Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(fprod(Wr,Ob,Wr1,OO));					
						grad_criterion(j,0)=grad_criterion(j,0)+temp*4/(1+exp(2*criterion/(Kor*Ob_s)-2));
					}		
				}
				if(obs_f[i]==-3)
				{
					criterion=sqrt((Ob[2]-Wr[2])*(Ob[2]-Wr[2]));
					for(int j=0;j<7;j++)
					{		
						temp=Ko1/((Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(dWr[j][2]);					
						grad_criterion(j,0)=grad_criterion(j,0)+temp*4/(1+exp(2*criterion/(Kor*Ob_s)-2));
					}
				}
			}
			//Low pass filter
			//Get max absolute of differenence between new and old grad
			
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(grad_criterion(i,0)-gradp[i][1]);
				if(temp<temp1) temp=temp1;
			}
			temp1=K_fm*(1-exp(-temp/K_fr));
			coeff_filter=dtim/(dtim+temp1);
			//coeff_filter=1;
			
			for(int i=0;i<7;i++)
			{
				gradp[i][1]=coeff_filter*grad_criterion(i,0)+(1-coeff_filter)*gradp[i][1];
				grad_criterion(i,0)=gradp[i][1];
			}
			Jnull.Mul(&grad_criterion,&dnulloW);
			//Get max absolute of dnullo
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulloW(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ko2*(1.0f-exp(-(temp)/Ko3))/(temp+0.001f);
			dnulloW.Mul(coeff,&dnulloW);
			//-----Avoid SE
			//-----Gradient of criterion
			for(int i=0;i<7;i++)
			{		
				grad_criterion(i,0)=0;				
			}
			for(int i=0;i<obs_num;i++)
			{
				Ob[0]=obs_array[i][0];Ob[1]=obs_array[i][1];Ob[2]=obs_array[i][2];Ob_s=obs_array[i][3];
				if(obs_f[i]==1)
				{								
					temp=fprod(El,Sh,Ob,Sh);
					criterion=sqrt(fprod(Ob,Sh,Ob,Sh)-100*temp*temp/(L3*L3));									
					for(int j=0;j<7;j++)
					{	
						El1[0]=dEl[j][0];El1[1]=dEl[j][1];El1[2]=dEl[j][2];
						temp1=Ko1/((Koff1+criterion)*(Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(-100.0f/(L3*L3))*temp*(fprod(Ob,Sh,El1,OO));	
						grad_criterion(j,0)=grad_criterion(j,0)+temp1*4/(1.0f+exp(2.0f*criterion/(Kor*Ob_s)-2.0f));
					}
				}
			}
			//Low pass filter
			//Get max absolute of differenence between new and old grad
			
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(grad_criterion(i,0)-gradp[i][2]);
				if(temp<temp1) temp=temp1;
			}
			temp1=K_fm*(1-exp(-temp/K_fr));
			coeff_filter=dtim/(dtim+temp1);
			
			for(int i=0;i<7;i++)
			{
				gradp[i][2]=coeff_filter*grad_criterion(i,0)+(1-coeff_filter)*gradp[i][2];
				grad_criterion(i,0)=gradp[i][2];
			}
			Jnull.Mul(&grad_criterion,&dnulloSE);
			//Get max absolute of dnullo
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulloSE(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ko2*(1.0f-exp(-(temp)/Ko3))/(temp+0.001f);
			dnulloSE.Mul(coeff,&dnulloSE);
			//-----Avoid EW
			//-----Gradient of criterion
			for(int i=0;i<7;i++)
			{		
				grad_criterion(i,0)=0;				
			}
			for(int i=1;i<obs_num;i++)
			{
				Ob[0]=obs_array[i][0];Ob[1]=obs_array[i][1];Ob[2]=obs_array[i][2];Ob_s=obs_array[i][3];
				if(obs_f[i]==1)
				{								
					temp=fprod(Wr,El,Ob,El);
					criterion=sqrt(fprod(Ob,El,Ob,El)-100*temp*temp/(L5*L5));									
					for(int j=0;j<7;j++)
					{	
						El1[0]=dEl[j][0];El1[1]=dEl[j][1];El1[2]=dEl[j][2];
						Wr1[0]=dWr[j][0];Wr1[1]=dWr[j][1];Wr1[2]=dWr[j][2];
						temp1=Ko1/((Koff1+criterion)*(Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(-fprod(Ob,El,El1,OO)-100.0f/(L5*L5)*temp*(-fprod(Wr,El,El1,OO)+fprod(Wr1,El1,Ob,El)));
						grad_criterion(j,0)=grad_criterion(j,0)+temp1*4.0f/(1+exp(2.0f*criterion/(Kor*Ob_s)-2.0f));
					}
				}			
			}
			//Low pass filter
			//Get max absolute of differenence between new and old grad
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(grad_criterion(i,0)-gradp[i][3]);
				if(temp<temp1) temp=temp1;
			}
			temp1=K_fm*(1-exp(-temp/K_fr));
			coeff_filter=dtim/(dtim+temp1);

			for(int i=0;i<7;i++)
			{
				gradp[i][3]=coeff_filter*grad_criterion(i,0)+(1-coeff_filter)*gradp[i][3];
				grad_criterion(i,0)=gradp[i][3];
			}
			Jnull.Mul(&grad_criterion,&dnulloEW);
			//Get max absolute of dnulloEW
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulloEW(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ko2*(1.0f-exp(-(temp)/Ko3))/(temp+0.001f);
			dnulloEW.Mul(coeff,&dnulloEW);

			//-----Avoid WH
			//-----Gradient of criterion
			for(int i=0;i<7;i++)
			{		
				grad_criterion(i,0)=0.0f;				
			}
			for(int i=1;i<obs_num;i++)
			{
				Ob[0]=obs_array[i][0];Ob[1]=obs_array[i][1];Ob[2]=obs_array[i][2];Ob_s=obs_array[i][3];
				if(obs_f[i]==1)
				{								
					temp=fprod(Wr,Ha,Ob,Ha);
					criterion=sqrt(fprod(Ob,Ha,Ob,Ha)-100.0f*temp*temp/(L7*L7));				
								
					for(int j=0;j<7;j++)
					{	
						Wr1[0]=dWr[j][0];Wr1[1]=dWr[j][1];Wr1[2]=dWr[j][2];
						temp1=Ko1/((Koff1+criterion)*(Koff2+criterion-Ob_s)*(Koff2+criterion-Ob_s))*(-temp*100.0f/(L7*L7)*fprod(Ob,Ha,Wr1,OO));
						grad_criterion(j,0)=grad_criterion(j,0)+temp1*4.0f/(1+exp(2.0f*criterion/(Kor*Ob_s)-2.0f));
					}
				}
			}
			//Low pass filter
			//Get max absolute of differenence between new and old grad
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(grad_criterion(i,0)-gradp[i][4]);
				if(temp<temp1) temp=temp1;
			}
			temp1=K_fm*(1-exp(-temp/K_fr));
			coeff_filter=dtim/(dtim+temp1);

			for(int i=0;i<7;i++)
			{
				gradp[i][4]=coeff_filter*grad_criterion(i,0)+(1-coeff_filter)*gradp[i][4];
				grad_criterion(i,0)=gradp[i][4];
			}
			Jnull.Mul(&grad_criterion,&dnulloWH);
			//Get max absolute of dnulloWH
			temp=0;
			for(int i=0;i<7;i++)
			{
				temp1=abs(dnulloWH(i,0));
				if(temp<temp1) temp=temp1;
			}
			coeff=Ko2*(1.0f-exp(-(temp)/Ko3))/(temp+0.001f);
			dnulloWH.Mul(coeff,&dnulloWH);
		}		
		//Integration
		for(int i=0;i<7;i++) 
		{			
			dnull[i]=0;
			if(jlimEnable==1)
			{
				dnull[i]=dnull[i]+dnullj(i,0);
			}
			if(singEnable==1)
			{
				dnull[i]=dnull[i]+dnulls(i,0);
			}
			if(obsEnable==1)
			{
				dnull[i]=dnull[i]+dnulloE(i,0)+dnulloW(i,0)+dnulloSE(i,0)+dnulloEW(i,0)+dnulloWH(i,0);	
			}
		}
		//Get max absolute of dnull
		temp=0;
		for(int i=0;i<7;i++)
		{
			temp1=abs(dnull[i]);
			if(temp<temp1) temp=temp1;
		}
		coeff=Kn1*(1.0f-exp(-(temp)/Kn2))/(temp+0.001f);
		for(int i=0;i<7;i++)
		{
			dnull[i]=dnull[i]*coeff;
			djtheta[i]=dneed(i,0)+dnull[i];
		}
		if(control_e==1)
		{
			djtheta[6]=vj7;
		}
		//Check if the calculated velocity is in its range		
		for(int i=0;i<7;i++)
		{
			if(djtheta[i]!=djtheta[i])
			{
				for(int j=0;j<7;j++)	{
					djtheta[j]=0;
				}
				return -20;
			}
		}	
		//Check if the calculated velocity is in its range
		for(int i=0;i<7;i++)
		{
			if(abs(djtheta[i])>velocity_max[i])
			{
				//Velocity of Joint i excesses its limit velocity
				/*
				for(int j=0;j<7;j++)	{
					djtheta[j]=0;
				}
				*/
				return -i;
			}
		}		   
	}
	//Sucessfull
	
	return 1;
}