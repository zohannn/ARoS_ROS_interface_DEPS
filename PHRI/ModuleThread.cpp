#include "ModuleThread.h"

CModuleThread::CModuleThread(void)
{
	//Reset flags
	started=0;
	stop=0;
	catched=0;
	closed=0;
	mode=0;
	angle_f=30;
}

CModuleThread::~CModuleThread(void)
{
}
bool CModuleThread::threadInit()
{
	started=true;		
	return (YMOD->connected==1);
}
void CModuleThread::threadRelease()
{
	//Reset flags
	stop=0;
	started=false;	
}
void CModuleThread::run()
{
	switch (mod_type)
	{
		case 1:// Hand

			switch(mode)
			{
			case 0://Open hand								
				YMOD->Send(BARRETTHAND_OPEN);
				closed=0;
				catched=0;
				break;
			case 1://Close hand-1
				YMOD->Send(BARRETTHAND_TORQUECLOSE);
				YMOD->Send(BARRETTHAND_GETPOSITIONS);
				if((YMOD->jpos[1]>110)&&(YMOD->jpos[2]>110)&&(YMOD->jpos[3]>110))
				{
					catched=0;
				}else
				{
					catched=1;
				}	
				closed=1;
				break;
			case 2://Open hand
				YMOD->msg.uCommand=BARRETTHAND_SETOPENTARGET;
				YMOD->msg.uParam.resize(0);
				YMOD->msg.fData.resize(4);
				YMOD->msg.fData[0]=0.6857f;
				YMOD->msg.fData[1]=60.0f;
				YMOD->msg.fData[2]=60.0f;
				YMOD->msg.fData[3]=60.0f;
				YMOD->Send(YMOD->msg);
				YMOD->msg.uCommand=BARRETTHAND_MOVEFINGERS;
				YMOD->msg.uParam.resize(0);
				YMOD->msg.fData.resize(1);
				YMOD->msg.fData[0]=angle_f;//40; 60; 90;			
				YMOD->Send(YMOD->msg);
				closed=0;
				catched=0;
				break;	
			case 3://Close hand for grasping the handle
				YMOD->Send(BARRETTHAND_TORQUECLOSE);
				YMOD->Send(BARRETTHAND_GETPOSITIONS);
				if((YMOD->jpos[1]>125)&&(YMOD->jpos[2]>125)&&(YMOD->jpos[3]>125))
				{
					catched=0;
				}else
				{
					YMOD->msg.uCommand=BARRETTHAND_MOVEFINGERS;
					YMOD->msg.uParam.resize(0);
					YMOD->msg.fData.resize(1);
					YMOD->msg.fData[0]=115.0f;//40; 60; 90;			
					YMOD->Send(YMOD->msg);
					catched=1;
				}	
				closed=1;
				break;
			}			
			break;

		default:
			break;
	}
}
