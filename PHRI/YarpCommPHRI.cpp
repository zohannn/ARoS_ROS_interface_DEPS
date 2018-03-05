// YarpCommPHRI.cpp: implementation of the CYarpCommPHRI class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "YarpCommPHRI.h"
#include "common.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYarpCommPHRI::CYarpCommPHRI( const std::string name, const std::string peerName, bool isServer )
: CYarpCommunication( name, peerName, isServer )
{
	//Reset flags
	connected=0;
	obs_num=0;
	refresh=0;	
	tar_num=0;
	for(int i=0;i<85;i++)
	{
		obs_g[i]=300;
	}	
	neck=0;
	v_pan=-30;
	v_tilt=-10;
}

void CYarpCommPHRI::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	// Here I process messages that arrive
	// with data requested by me, or messages that don't require reply
	int j=0;
	float temp;
	flag_all=0;
	flag_dev=1;
	flag_comm=1;
	if(msgIn.uCommand % 2 == 0)
	{
		flag_comm=0;  // Error in communication
		return;
	}
	if(msg.iErrorCode !=0)
	{
		flag_dev=0; //Error in device
		return;
	}		
	switch( msgIn.uCommand )
	{
	case NULL+ACK:
		//msgIn.fData[0-6]
		break;

	case AMTEC_MOVE_VEL+ACK:
		flag_all=1;
		break;
	case AMTEC_GET_POS_ALL+ACK:
		if( msgIn.fData.size() < 7 ) break;
		for( size_t i=0 ; i<msgIn.fData.size() ; i++ )
		{
			jtheta[i] = msgIn.fData[i];
		}
		flag_all=1;
		break;
	case AMTEC_GET_GRIPPER+ACK:
		if( msgIn.fData.size() < 1 ) break;
		gripper=msgIn.fData[0];
		flag_all=1;
		//msgIn.strText
		break;
	case AMTEC_GET_FORCE_TORQUE+ACK:
		if( msgIn.fData.size() < 1 ) break;
		for( size_t i = 0; i<msgIn.fData.size(); i++ )
		{
			fW[i] = msgIn.fData[i];
		}
		flag_all=1;
		break;

	case VISION_BOT_GET_POSITION_OBJECT_TYPE+ACK:

		if( msgIn.fData.size() < 1 ) 	
		{
			break;
		}
		switch (mode)
		{
		case 1://Obstacle
			switch(object_type)
			{
				case OBJECT_MAGENTA:
					if(refresh==0)
					{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							
							obs[j][0]=msgIn.fData[i*3];
							obs[j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10.0f) continue;
							//obs[j][2]=obs[j][2]-65+5;  //For Dumbo
							obs[j][2]=msgIn.fData[i*3+2]-150.0f;   //For Aros
							obs[j][3]=10.0f;//20;
							obs_f[j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=j;
						refresh=1;
					}else{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[obs_num+j][0]=msgIn.fData[i*3];
							obs[obs_num+j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[obs_num+j][3]=10;//20;
							obs_f[obs_num+j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=obs_num+j;
					}
					flag_all=1;		
					break;
				case OBJECT_MAGENTA_SMALL:
					if(refresh==0)
					{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							
							obs[j][0]=msgIn.fData[i*3];
							obs[j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;  //For Dumbo
							obs[j][2]=msgIn.fData[i*3+2]-150;   //For Aros
							obs[j][3]=10;//20;
							obs_f[j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=j;
						refresh=1;
					}else{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[obs_num+j][0]=msgIn.fData[i*3];
							obs[obs_num+j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[obs_num+j][3]=10;//20;
							obs_f[obs_num+j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=obs_num+j;
					}
					flag_all=1;		
					break;
				case OBJECT_MAGENTA_BIG:
					if(refresh==0)
					{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							
							obs[j][0]=msgIn.fData[i*3];
							obs[j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;  //For Dumbo
							obs[j][2]=msgIn.fData[i*3+2]-150;   //For Aros
							obs[j][3]=15;//20;
							obs_f[j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=j;
						refresh=1;
					}else{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[obs_num+j][0]=msgIn.fData[i*3];
							obs[obs_num+j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[obs_num+j][3]=15;//20;
							obs_f[obs_num+j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=obs_num+j;
					}
					flag_all=1;		
					break;
				case OBJECT_GREEN:
					if(refresh==0)
					{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[j][0]=msgIn.fData[i*3];
							obs[j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];
							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[j][3]=15;
							obs_f[j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=j;
						refresh=1;
					}else{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[obs_num+j][0]=msgIn.fData[i*3];
							obs[obs_num+j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65+5;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[obs_num+j][3]=15;
							obs_f[obs_num+j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=obs_num+j;
					}
					flag_all=1;		
					break;
				case OBJECT_RED:
					if(refresh==0)
					{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							
							obs[j][0]=msgIn.fData[i*3];
							obs[j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65-10;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[j][3]=12;
							obs_f[j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=j;
						refresh=1;
					}else{
						j=0;
						for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
						{
							if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
							obs[obs_num+j][0]=msgIn.fData[i*3];
							obs[obs_num+j][1]=msgIn.fData[i*3+1];
							obs[j][2]=msgIn.fData[i*3+2];							
							temp=sqrt(obs[j][0]*obs[j][0]+obs[j][1]*obs[j][1]+obs[j][2]*obs[j][2]);
							if(temp<10) continue;
							//obs[j][2]=obs[j][2]-65-10;
							obs[j][2]=msgIn.fData[i*3+2]-150;
							obs[obs_num+j][3]=12;
							obs_f[obs_num+j]=1;
							j++;
							if(j==20) break;
						}
						obs_num=obs_num+j;
					}
					flag_all=1;		
					break;
				default:
					break;
			}
			break;
		
		case 2:// Target
			tar_num=0;
			switch(object_type)
			{
			case OBJECT_GREEN:	// Target, handle				    
				for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
					{
						if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
						target[tar_num][0]=msgIn.fData[i*3];
						target[tar_num][1]=msgIn.fData[i*3+1];						
						target[tar_num][2]=msgIn.fData[i*3+2];
						temp=sqrt(target[tar_num][0]*target[tar_num][0]+target[tar_num][1]*target[tar_num][1]+target[tar_num][2]*target[tar_num][2]);
						if(temp>10)
						{
							target[tar_num][1]=target[tar_num][1];//-10;			
							target[tar_num][2]=target[tar_num][2]-150;//-150 for aros  -65 for dumbo	
							tar_num++;
							if (tar_num>3) break;
						}
						
					}					
					flag_all=1;		
					break;	
				case OBJECT_RED: // Location				    
					for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
					{
						if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
						target[tar_num][0]=msgIn.fData[i*3];
						target[tar_num][1]=msgIn.fData[i*3+1];						
						target[tar_num][2]=msgIn.fData[i*3+2];
						temp=sqrt(target[tar_num][0]*target[tar_num][0]+target[tar_num][1]*target[tar_num][1]+target[tar_num][2]*target[tar_num][2]);
						if(temp>10)
						{
							target[tar_num][2]=target[tar_num][2]-150;  //-150 for aros  -65 for dumbo	
							tar_num++;
							if (tar_num>3) break;
						}
						
					}						
					flag_all=1;		
					break;
				case OBJECT_MAGENTA: 			    
					for( size_t i = 0; i<msgIn.fData.size() / 3; i++ )
					{
						if(msgIn.fData[i*3]!=msgIn.fData[i*3]) continue;
						target[tar_num][0]=msgIn.fData[i*3];
						target[tar_num][1]=msgIn.fData[i*3+1];						
						target[tar_num][2]=msgIn.fData[i*3+2];
						temp=sqrt(target[tar_num][0]*target[tar_num][0]+target[tar_num][1]*target[tar_num][1]+target[tar_num][2]*target[tar_num][2]);
						if(temp>10)
						{
							target[tar_num][2]=target[tar_num][2]-150;  //-150 for aros  -65 for dumbo	
							tar_num++;
							if (tar_num>3) break;
						}
						
					}						
					flag_all=1;		
					break;
				default:
					break;
			}
			break;
		default:
		break;
		}
	
	case BARRETTHAND_GETPOSITIONS+ACK:
		if( msgIn.fData.size() < 4 ) break;
		for( size_t i = 0; i<msgIn.fData.size(); i++ )
		{
			jpos[i] = msgIn.fData[i];
		}
		flag_all=1;
		break;
	case VOICE_MESSAGE+ACK:
		if( msgIn.uParam.size() < 1 ) break;
		command=msgIn.uParam[0];
		flag_all=1;
		break;
	case MOTORS_GET_VELOCITY+ACK:
		if( msgIn.fData.size() < 2 ) break;
		for( int i=0 ; i<2 ; i++ )
		{
			mvel[i] = msgIn.fData[i];
		}
		flag_all=1;
		break;
	case RANGE_FINDER_GET_LAST_SCAN+ACK:
		if( msgIn.uParam.size() < 85 ) break;
		for( int i=0 ; i<85 ; i++ )
		{
			obs_g[i] = static_cast<float>( msgIn.uParam[i] )/10.0f;// cm
		}
		flag_all=1;
		break;
	default:
		//all other ACK commands don't matter
		flag_all=1;
		break;
	}
}
