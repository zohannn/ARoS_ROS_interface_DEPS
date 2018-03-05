#include "ArmThread.h"
// This class is responsible to perform combined tasks
CArmThread::CArmThread(void)
{
	//Init flags
	started=0;
	stop=0;
	tims=0;
	mode=0;
	//-------Target acquisition--------
	see_tar=0;
	dist0=10;	
	for(int i=0;i<3;i++)
	{
				vtar[i]=0;
	}
	modep=0;
	//---PID parameters for force control
	Kpf=4.0f;
	Tif=10.0f;
	Tdf=0.001f;
	Nf=2.0f;
	Kpm=0.1f;
	Tim=0.003f;
	Tdm=0.001f;
	Nm=2.0f;
	//Flag
	for(int i=0;i<6;i++)
	{		
		dpos0[i]=0.0f;
	}	
	dmob[0]=0.0f;
	dmob[1]=0.0f;
	tim_rot=0.0f;
	tim=0.0;
}
CArmThread::~CArmThread(void)
{
	
}
bool CArmThread::threadInit()
{
	started=true;
	dpos_s=0;	
	return true;
}
void CArmThread::threadRelease()
{
	stop=0;
	started=0;	
}
void CArmThread::run()
{		
	//double tim1;
	long tcount;	
	int state;
	//Draw ellipse variables
	std::ofstream fdjpos,fdjvel,fdforce,fdforceraw,fddpos,fdmvel,fdds,fdves;
	float w=0.2f;//Parameter for the ellipse
	float Ra=200.0f;
	float Rb=150.0f;
	float temp=0.0f;
	float temp1=0.0f;
	//Pursue a dynamic target variables
	float Vtar0,V0[3],dist,Vmin[3],Vmax[3],K_f,alpha_f;
	float Vmin0=40.0f,Vmax0=280.0f,Vp[3];
	//PID for force control
	float err[6],errn[6],errp[6],out_p[6],out_n[6],dt=0.0f;
	// Force reaction
	int tomove=0;
	float dpost[6];
	//Reset Arm	
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<5;j++)
		{
			ArosArm->gradp[i][j]=0;
		}
	}
	
	//Task selection
	switch (mode)
	{
		case 0:// Move with a desired velocity				
			//Open files for recording joint positions,velocities			
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->timp=tims;
			ArosArm->tims=tims;	
			dpos_s=1;//Smooth start enable
			ArosArm->timfp=tims;	
			//Reset Force old value
			YPHRI->Send(AMTEC_GET_POS_ALL);
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			for(int i=0;i<6;i++)
			{
				ArosArm->fWp[0]=0.0f;
			}
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			while(!stop)
			{		
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				//ArosArm->fForceCompensated(jtheta,YPHRI->fW,ArosArm->fW);
				//ArosArm->fForceStartComp();

				for (int i=0;i<6;i++)
					{
						fdforce << ArosArm->fW[i] << " ";
					}
				fdforce << ";";
				if(dpos_s==1)
				{// Smooth start
					for(int i=0;i<6;i++)
					{
						dpos[i]=dpos0[i]/(1.0f+exp(-(tim-ArosArm->tims)+2.0f));
					}
				}else
				{
					for(int i=0;i<6;i++)
					{
						dpos[i]=dpos0[i];
					}
				}				
				//Inverse kinematics
				state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
				//Record data
				for (int i=0;i<6;i++)
				{
					fdforce << ArosArm->fW[i] << " ";
				}
				fdforce << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";

				if(state!=1) 
				{
					break;
				}						
				//Send to motors
				YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
				YPHRI->msg.uParam.resize(0);
				YPHRI->msg.fData.resize(7);
				for(int i=0;i<7;i++)
				{
					YPHRI->msg.fData[i]=djtheta[i];
				}	
				YPHRI->Send(YPHRI->msg);				
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			break;
		case 1:// Draw an ellipse					
			// Open files for recording joint positions,velocities			
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fdforceraw.open ("dataforceraw.txt");
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;			
			ArosArm->timp=tims;
			ArosArm->tims=tims;	
			ArosArm->timfp=tims;	
			ArosArm->singEnable=0;

			//Reset Force old value
			YPHRI->Send(AMTEC_GET_POS_ALL);
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			for(int i=0;i<6;i++)
			{
				ArosArm->fWp[0]=0;
			}
			//ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);	

			ArosArm->fForceEnd(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);

			// PID initialization			
			err[2]=0;	
			errn[2]=0;
			out_n[2]=0;
			
			Kpf=40.0f;
			Tif=6.0f;
			Tdf=0.02f;
			Nf=2.0f;
			//Draw the ellipse	
			while((tim<(tims+31.1416f*2.0f))&&(!stop))
			{		
				tcount=GetTickCount()-ArosArm->start;
				timp=tim;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
				//ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);

				ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW);				
				ArosArm->fForceStartComp(YPHRI->jtheta,YPHRI->fW,tim);

				ArosArm->fForceEnd(YPHRI->jtheta,YPHRI->fW,fW);

				dpos[0]=w*Rb*cos(w*(tim-tims-7.7854f));
				dpos[1]=-w*Ra*sin(w*(tim-tims-7.7854f));
				dpos[2]=0.0f;
				
				//Force compensation in z axis					
				errp[2]=errn[2];
				out_p[2]=out_n[2];
				dt=tim-timp;
				errn[2]=ArosArm->fW[2]-0.0f;
				err[2]=err[2]+errn[2]*dt;
				out_n[2]=Kpf*(1*errn[2]+err[2]/Tif)+Nf*Tdf*(errn[2]-errp[2])/(Nf*dt+Tdf)+Tdf*out_p[2]/(Nf*dt+Tdf);
				dpos[2]=out_n[2];	
				
				//---
				dpos[3]=0.0f;
				dpos[4]=0.0f;
				dpos[5]=0.0f;

				//Inverse kinematics
				//---------------------------				
				if((ArosArm->ypr[1]>-40*M_PI/180))
				{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
				}else
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					};	
				//Record data
				for (int i=0;i<6;i++)
				{					
					fdforce << ArosArm->fW[i] << " ";
				}
				fdforce << ";";
				for (int i=0;i<6;i++)
				{					
					fdforceraw << fW[i] << " ";
				}
				fdforceraw << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";

				if(state!=1) 
				{
					break;
				}		
				//Send to motors
				YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
				YPHRI->msg.uParam.resize(0);
				YPHRI->msg.fData.resize(7);
				for(int i=0;i<7;i++)
				{
					YPHRI->msg.fData[i]=djtheta[i];
				}	
				YPHRI->Send(YPHRI->msg);
				
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fdforceraw.close();
			break;
		case 2: //Pursue a dynamic target	
			search_loc=0;
			search_handle=0;
			search_tar=1;
			HandThread.catched=0;
			HandThread.angle_f=60;
			HandThread.mode=2;//Open the hand
			HandThread.start();
			while(see_tar==0)
			{
				//wait for the target appear
			}
			//look at the arm
			YVIS->v_pan=-50;
			YVIS->v_tilt=-30;
			YVIS->neck=1;			

			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			for(int i=0;i<3;i++)
			{
				Vp[i]=0.0f;
			}
			
			ArosArm->timp=tims;
			ArosArm->tims=tims;
			//Begin the pursue task
			while(!stop)
			{		
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;					
				YPHRI->Send(AMTEC_GET_POS_ALL);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);
				
				ArosArm->fEndEffectorInfo(jtheta,pos);				
				Vtar0=3*sqrt(vtar[0]*vtar[0]+vtar[1]*vtar[1]+vtar[2]*vtar[2]);
				for(int i=0;i<3;i++)
				{
					V0[i]=target[i]-pos[i];
				}
				dist=sqrt(V0[0]*V0[0]+V0[1]*V0[1]+V0[2]*V0[2]);
				if((HandThread.catched==0)&&(HandThread.closed==1)&&(!HandThread.started))
				{//Unsuccess in catching the target
					search_loc=0;
					search_handle=0;
					search_tar=1;
					HandThread.mode=0;
					HandThread.start();	
				}
				if((dist>dist0)||(HandThread.closed==1))//Move over the target
				{
					target[2]=target[2]+10.0f;
				}
				for(int i=0;i<3;i++)
				{
					V0[i]=(target[i]-pos[i])*2.0f;
				}
				dist=sqrt(V0[0]*V0[0]+V0[1]*V0[1]+V0[2]*V0[2]);				
				if(dist<2)
				{					
					//Try to Grasp the target - Start HandThread
					if((!HandThread.started)&&(HandThread.catched==0))
					{							
						//Close the hand
						HandThread.mode=1;
						HandThread.start();						
					}					
				}
				//Filter target acquisition velocity
				for(int i=0;i<3;i++)
				{
					Vmin[i]=Vmin0*V0[i]/(dist+0.001f)*(1.0f-exp(-dist));
					Vmax[i]=Vmax0*V0[i]/(dist+0.001f);
				}
				if(Vmin0+Vtar0>Vmax0)
				{
					for(int i=0;i<3;i++)
					{
						V0[i]=Vmax[i];
					}
				}else
				{
					if(dist>dist0)
					{
						for(int i=0;i<3;i++)
						{
							V0[i]=(Vmax[i]-Vmin[i]-vtar[i])*(1-exp(-(dist-dist0)/2 )) +Vmin[i]+vtar[i];
						}
					}else
					{
						for(int i=0;i<3;i++)
						{
							V0[i]=Vmin[i]+vtar[i];
						}
					}
				}
				//Get max of velocity difference
				temp=0;
				for(int i=0;i<3;i++)
				{
					temp1=abs(V0[i]-Vp[i]);
					if(temp<temp1) temp=temp1;
				}
				K_f=0.4f*(1-exp(-temp/20.0f) )*(1.0f-exp(-dist/2.0f) );					
				alpha_f=(tim-timp+0.0001f)/(tim-timp+K_f+0.0001f);
				for(int i=0;i<3;i++)
				{
					dpos[i]=alpha_f*V0[i]+(1-alpha_f)*Vp[i];
					Vp[i]=dpos[i];
				}				
				//-----------Check if the target catched
				if (HandThread.catched==1)
				{
					//Do something	
					break;
				}else
				{	//Continue to pursue					
					dpos[3]=0;dpos[4]=0;dpos[5]=0;
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);	
					if(state!=1) 
					{						
						break;
					}						
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);					
				}
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			break;
		case 3: //Follow a basket and release the target
			//Look at the location space
			
			//look at the arm
			YVIS->v_pan=-10;
			YVIS->v_tilt=-45;
			YVIS->neck=1;
			search_loc=1;
			search_handle=0;
			search_tar=0;
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->timp=tims;
			ArosArm->tims=tims;
			while((!stop)) 
			{		
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);				
				
				ArosArm->fEndEffectorInfo(jtheta,pos);				
			
				target[0]=73.0f;target[1]=0.0f;target[1]=-50.0f;//Red box
				for(int i=0;i<3;i++)
				{
					V0[i]=target[i]-pos[i];
				}
				V0[2]=0.0f;
				dist=sqrt(V0[0]*V0[0]+V0[1]*V0[1]);				
				if(dist<2)
				{					
					//Release the target - Start HandThread
					if((!HandThread.started)&&(HandThread.catched==1))
					{//Open hand
						HandThread.mode=2;
						HandThread.start();
						HandThread.catched=0;
						break;
					}					
				}
				//Pursue
				for(int i=0;i<3;i++)
				{
					Vmin[i]=Vmin0*V0[i]/(dist+0.001f)*(1.0f-exp(-dist));
					Vmax[i]=Vmax0*V0[i]/(dist+0.001f);
				}
				if(Vmin0>Vmax0)
				{
					for(int i=0;i<3;i++)
					{
						V0[i]=Vmax[i];
					}
				}else
				{
					if(dist>dist0)
					{
						for(int i=0;i<3;i++)
						{
							V0[i]=(Vmax[i]-Vmin[i])*(1.0f-exp(-(dist-dist0)/2.0f ) ) +Vmin[i];
						}
					}else
					{
						for(int i=0;i<3;i++)
						{
							V0[i]=Vmin[i];
						}
					}
				}
				//Get max of velocity difference
				temp=0;
				for(int i=0;i<3;i++)
				{
					temp1=abs(V0[i]-Vp[i]);
					if(temp<temp1) temp=temp1;
				}
				K_f=0.2f*(1-exp(-temp/20.0f) )*(1-exp(-dist/2.0f) );					
				alpha_f=(tim-timp+0.0001f)/(tim-timp+K_f+0.0001f);
				for(int i=0;i<3;i++)
				{
					dpos[i]=alpha_f*V0[i]+(1-alpha_f)*Vp[i];
					Vp[i]=dpos[i];
				}
				dpos[3]=0.0f;dpos[4]=0.0f;dpos[5]=0.0f;
				state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
				if(state!=1) 
				{					
					break;
				}						
				YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
				YPHRI->msg.uParam.resize(0);
				YPHRI->msg.fData.resize(7);
				for(int i=0;i<7;i++)
				{
					YPHRI->msg.fData[i]=djtheta[i];
				}	
				YPHRI->Send(YPHRI->msg);
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			break;			
		case 4:// Force reaction
			// Open files for recording joint positions, velocities			
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fddpos.open ("datadpos.txt");
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->timp=tims;
			ArosArm->tims=tims;	
			ArosArm->timEp=tims;			
			YPHRI->Send(AMTEC_GET_POS_ALL);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			for(int i=0;i<6;i++)
			{
				ArosArm->fWp[i]=0.0f;				
			}
			Kpf=8.0f;
			Kpm=0.2f;
			ArosArm->flag_torquez=0;
			while(!stop)
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
				//ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				// Reaction
				ArosArm->fReaction(dpos,&tomove);
				ArosArm->fVelSmoothing(dpos,tim);
				//Record data
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<6;i++)
					{
						fdforce << ArosArm->fW[i] << " ";
					}
					fdforce << ";";
				
				if(tomove==1)
				{
					//Inverse kinematics
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					//Record data					
					for (int i=0;i<7;i++)
					{
						fdjpos << jtheta[i] << " ";
					}
					fdjpos << ";";
					
					
					for (int i=0;i<7;i++)
					{
						fdjvel << djtheta[i] << " ";
					}
					fdjvel << ";";

					if(state!=1) 
					{
						break;
					}					
					//Send to motors
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					
					
				}else
				{
					YPHRI->Send(AMTEC_SOFT_STOP);
					ArosArm->fReset(tim);
				}
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fddpos.close();
			break;
		case 5:// Interaction
			// Open files for recording joint positions, velocities			
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fddpos.open ("datadpos.txt");
		
			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;			
			flag_armstop=0;
			HandThread.started=0;
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="Hello. Give me something";
			YSPE->Send(YSPE->msg);
			
			while(tim<tims+3)
			{
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;
			}
			if(!HandThread.started)
			{				
				//Close hand
				HandThread.mode=1;				
				HandThread.start();									
			}
			
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;			
			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			ArosArm->jlimEnable=1;
			ArosArm->singEnable=1;
			
			while(!stop)
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				// Compliance
				ArosArm->fReaction(dpos,&tomove);
				ArosArm->fVelSmoothing(dpos,tim);
				
								
				state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data
				for (int i=0;i<6;i++)
				{
					fdforce << ArosArm->fW[i] << " ";
				}
				fdforce << ";";
				for (int i=0;i<6;i++)
				{
					fddpos << dpos[i] << " ";
				}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
							
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							/*
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="You are hurting my arm";
							YSPE->Send(YSPE->msg);
							*/
							
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}
					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					
				}												
			}
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fddpos.close();
			break;
					
		case 6:
			// Take me around			
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fddpos.open ("datadpos.txt");
			fdmvel.open ("datamvel.txt");
			fdds.open ("datads.txt");
			fdves.open ("dataves.txt");		
			//Rotate the neck	
			YVIS->v_pan=-80;
			YVIS->v_tilt=-15;
			YVIS->neck=1;
			//Open hand			
			if(!HandThread.started)
			{				
				//Open hand for hold hand
				HandThread.mode=2;
				HandThread.angle_f=90;
				HandThread.start();									
			}			
			search_handle=0;
			search_tar=0;
			search_loc=0;
			// Say something
			ArosArm->flag_comp=1;
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="Lead the way";
			YSPE->Send(YSPE->msg);

			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			ArosArm->see_target=0;
			flag_armstop=0;

			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;			
			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;
			
			while(!stop)
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);				

				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);				
				ArosArm->fMobile(dmob,tim);
				
				ArosArm->fEndEffector(dmob,dpos,tim,4);
				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;				
				// Say				
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;								
						
					if(ArosArm->flag_obs==1)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. I recommend to go to my left";
						YSPE->Send(YSPE->msg);
					}	
					
					if((ArosArm->flag_obs==-1)||(ArosArm->flag_obs==-2))
					{
						ArosArm->flag_obs=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="We can continue safely now";
						YSPE->Send(YSPE->msg);
					}
					if(ArosArm->flag_obs==2)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. Let's go to my right";
						YSPE->Send(YSPE->msg);
					}
					
					if(ArosArm->flag_hurt==1)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="You pull my arm so hard";
						YSPE->Send(YSPE->msg);
					}	
					if(ArosArm->flag_hurt==-1)
					{
						ArosArm->flag_hurt=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="My arm is better now";
						YSPE->Send(YSPE->msg);
						
					}	
					if(ArosArm->flag_hurt==2)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="You push my arm so hard";
						YSPE->Send(YSPE->msg);
					}	
					if(ArosArm->flag_hurt==-2)
					{
						ArosArm->flag_hurt=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="My arm is better now";
						YSPE->Send(YSPE->msg);
						
					}
					if(ArosArm->flag_hurt==3)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="You move too fast";
						YSPE->Send(YSPE->msg);
					}	
					if(ArosArm->flag_hurt==-3)
					{
						ArosArm->flag_hurt=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="I feel better now";
						YSPE->Send(YSPE->msg);
						
					}
					
				}
				//---------------------------				
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};				
				//mask[4]=1;
				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);					
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";

				fdves << ArosArm->ddL_ves[0] << " " << ArosArm->dL_ves[0] << " " << ArosArm->L_ves[0] <<" ";
				fdves << ArosArm->ddL_ves[1] << " " << ArosArm->dL_ves[1] << " " << ArosArm->L_ves[1] << ";";		
				
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="You are hurting my arm";
							YSPE->Send(YSPE->msg);
							
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}					
					//Send to motors					
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}						
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];								
			}
			YMOB->Send(MOTORS_STOP);
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fddpos.close();
			fdmvel.close();
			fdds.close();
			fdves.close();
			break;
		case 7:
			//Mobile platform
			YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
			YMOB->msg.uParam.resize(0);
			YMOB->msg.fData.resize(2);
			YMOB->msg.fData[0]=dmob[0];
			YMOB->msg.fData[1]=dmob[1];
			YMOB->Send(YMOB->msg);
			while(!stop)
			{					
			}
			YMOB->Send(MOTORS_STOP);	
			break;
		case 8:// Mobile autonomous driving							
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			dmobp[0]=0;
			dmobp[1]=0;
			ArosArm->F_dsp=0;
			ArosArm->M_dsp=0;
			ArosArm->target[0]=2500;
			ArosArm->target[1]=-200;	
			while(!stop)
			{
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;
				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);
				ArosArm->fMobileDriving(dmob);
				//Caculate new target				
				ArosArm->fCalculateTarget(dmob,ArosArm->target,tim-timp);
				if(sqrt(ArosArm->target[0]*ArosArm->target[0]+ArosArm->target[1]*ArosArm->target[1])<100)
				{
					break;
				}
				if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
				{
					//Mobile platform
					YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
					YMOB->msg.uParam.resize(0);
					YMOB->msg.fData.resize(2);
					YMOB->msg.fData[0]=dmob[0];
					YMOB->msg.fData[1]=dmob[1];
					YMOB->Send(YMOB->msg);
				}
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];
				
			}
			YMOB->Send(MOTORS_STOP);	
			break;
		case 9:			
			// Object transportation
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fddpos.open ("datadpos.txt");
			fdmvel.open ("datamvel.txt");
			fdds.open ("datads.txt");
			fdves.open ("dataves.txt");		
			// Take me around			
			YVIS->v_pan=0;
			YVIS->v_tilt=-30;
			YVIS->neck=1;
			// Say something
			stage=0;
			search_handle=1;
			search_tar=0;
			search_loc=0;
			
			ArosArm->flag_comp=1;
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="I can help you to transport objects";
			YSPE->Send(YSPE->msg);
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="Take me to the object that you want to transport";
			YSPE->Send(YSPE->msg);
			//Open hand
			if(!HandThread.started)
			{				
				//Open hand for hold hand
				HandThread.mode=2;
				HandThread.angle_f=90;
				HandThread.start();									
			}
			
			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			ArosArm->see_target=0;
			flag_armstop=0;

			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;			
			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;
			
			flag_neck=0;
			while((!stop)&&(stage==0))
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				//ArosArm->fForceStartComp();

				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);
				//ArosArm->fMobileIntegrating(dpos,dmob,tim);	
				ArosArm->fMobile(dmob,tim);
				ArosArm->fEndEffector(dmob,dpos,tim,0);
				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;
				
				if((tim>tims+5)&&(flag_neck==0))
				{	
					YVIS->v_pan=-75;
					YVIS->v_tilt=-10;
					flag_neck=1;
					YVIS->neck=1;												
				}
				if((ArosArm->Oef[2]<50)&&(flag_neck==1))
				{
					YVIS->v_pan=0;
					YVIS->v_tilt=-30;
					YVIS->neck=1;
					flag_neck=2;
				}
				
				if(ArosArm->get_target==1)
				{		
					stage=1;
					break;//End task
				}
				// Say
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;
								
						
					if(ArosArm->flag_obs==1)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. I recommend to go to my left";
						YSPE->Send(YSPE->msg);
					}	
					
					if((ArosArm->flag_obs==-1)||(ArosArm->flag_obs==-2))
					{
						ArosArm->flag_obs=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="We can continue safely now";
						YSPE->Send(YSPE->msg);
					}
					if(ArosArm->flag_obs==2)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. Let's go to my right";
						YSPE->Send(YSPE->msg);
					}						
				}
				//---------------------------
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};

				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							/*
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="You are hurting my arm";
							YSPE->Send(YSPE->msg);
							*/
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}
						/*
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						*/
						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}
					
					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}	
					
					
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];								
			}
			//---------Grasp object------------
			if(stage==1)
			{			
			
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="I will pick up the object";
			YSPE->Send(YSPE->msg);
			if(!HandThread.started)
			{				
				//Open hand for grasping
				HandThread.mode=0;				
				HandThread.start();									
			}				
			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			flag_armstop=0;

			HandThread.catched=0;
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;

			
			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;	

			search_handle=0;
			search_tar=0;
			search_loc=0;
			ArosArm->flag_grasp=0;
			
			//ArosArm->flag_comp=0;
			flag_neck=0;
			ArosArm->fCopy(ArosArm->targetp,ArosArm->target,4);
			ArosArm->fPanTilt(ArosArm->target,&pan,&tilt);
			
			YVIS->v_pan=pan;
			YVIS->v_tilt=tilt;			
			YVIS->neck=1;
			ArosArm->avoidobs=0;
			}
					
			while((!stop)&&(stage==1))
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);				

				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);
				
				ArosArm->fMobile(dmob,tim);
				ArosArm->fEndEffector(dmob,dpos,tim,1);// By target
				
				if((!HandThread.started)&&(HandThread.catched==0)&&(HandThread.closed==1))
				{ //Open hand again if grasp unsuccessfully
					HandThread.mode=0;
					HandThread.start();
					
				}
				
				if(ArosArm->flag_grasp==1)
				{					
					//Try to Grasp the target - Start HandThread
					if((!HandThread.started)&&(HandThread.catched==0))
					{	//Close the hand
						YMOB->Send(MOTORS_STOP);
						YPHRI->Send(AMTEC_SOFT_STOP);
						HandThread.mode=3;
						HandThread.start();	
						tim=tim;
					}
				}
				if(HandThread.catched==1)
				{// Grasp sucessfully
					stage=2;					
					YSPE->msg.uCommand=AUDIO_MESSAGE;
					YSPE->msg.strText="I grasped the handle";
					YSPE->Send(YSPE->msg);
					break;
				}


				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;				
				
				// Say
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;				
					
				}
				//---------------------------
				if(ArosArm->flag_grasp!=1)
				{
				
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};

				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data		
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="My arm is hurted";
							YSPE->Send(YSPE->msg);
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						
						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}
					
					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}	
					
					
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];
				}
			}
			
			//---------Lift the object------------
			
			if(stage==2)
			{			
			search_handle=0;
			search_tar=0;
			search_loc=0;			
			ArosArm->flag_comp=1;
			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="Let's lift up the object";
			YSPE->Send(YSPE->msg);
						
			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			flag_armstop=0;

			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;
			
			YVIS->v_pan=-70;
			YVIS->v_tilt=-10;			
			YVIS->neck=1;

			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;
			ArosArm->avoidobs=0;
			}			

			while((!stop)&&(stage==2))
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);				

				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);			

				ArosArm->fMobile(dmob,tim);
				ArosArm->flag_comp=1;
				if(tim<tims+3)
				{
					ArosArm->flag_comp=0;
				}				
				ArosArm->fEndEffector(dmob,dpos,tim,3);// By target
				if(tim<tims+3)
				{
					dmob[0]=0;
					dmob[1]=0;
				}
				if(tim>tims+10)
				{			
					stage=3;
					break;
				}
				
				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;				
				
				// Say
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;				
					
				}
				//---------------------------				
				
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};

				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data	
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="My arm is hurted";
							YSPE->Send(YSPE->msg);
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						
						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}				

					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}	
					
					
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];
				
			}
			//Object transportation
			if(stage==3)
			{
				ArosArm->flag_comp=1;
				YSPE->msg.uCommand=AUDIO_MESSAGE;
				YSPE->msg.strText="Let's go";
				YSPE->Send(YSPE->msg);				
				
				ArosArm->flag_torquez=1;
				ArosArm->flag_torquey=0;
				ArosArm->normal=1;
				ArosArm->flag_say=0;
				ArosArm->flag_obs=0;
				flag_notify=0;
				ArosArm->see_target=0;
				flag_armstop=0;

				tcount=GetTickCount()-ArosArm->start;
				tims=static_cast<float>( tcount )/1000.0f;
				tim=tims;
				timp=tims;
				ArosArm->fReset(tim);
				dmobp[0]=0;
				dmobp[1]=0;			
				YPHRI->Send(AMTEC_GET_POS_ALL);			
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				// Update force at the wrist			
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
				ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
				//ArosArm->jlimEnable=0;
				ArosArm->singEnable=0;
				ArosArm->avoidobs=1;
			}
				
			while((!stop)&&(stage==3))
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				//ArosArm->fForceStartComp();

				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);
				//ArosArm->fMobileIntegrating(dpos,dmob,tim);	
				ArosArm->fMobile(dmob,tim);
				ArosArm->fEndEffector(dmob,dpos,tim,2);
				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;	
				
				if(ArosArm->finish==1)
				{		
					
					YSPE->msg.uCommand=AUDIO_MESSAGE;
					YSPE->msg.strText="The task is finished";
					YSPE->Send(YSPE->msg);
					break;//End task
				}
				// Say
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;
					
					
					if(ArosArm->flag_obs==1)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. I recommend to go to my left";
						YSPE->Send(YSPE->msg);
					}	
					
					if((ArosArm->flag_obs==-1)||(ArosArm->flag_obs==-2))
					{
						ArosArm->flag_obs=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="We can continue safely now";
						YSPE->Send(YSPE->msg);
					}
					if(ArosArm->flag_obs==2)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. Let's go to my right";
						YSPE->Send(YSPE->msg);
					}	
					
					
				}
				//---------------------------
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};

				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="You are hurting my arm";
							YSPE->Send(YSPE->msg);
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}
						/*
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						*/
						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0;
						dmobp[1]=0;
						flag_armstop=0;
						continue;
					}
					
					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}	
					
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];								
			}
			YMOB->Send(MOTORS_STOP);
			YPHRI->Send(AMTEC_SOFT_STOP);
			YVIS->v_pan=0;
			YVIS->v_tilt=-30;
			YVIS->neck=1;
			HandThread.YMOD->Send(BARRETTHAND_OPEN);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fddpos.close();
			fdmvel.close();
			fdds.close();
			fdves.close();
			break;	
		case 10:
			// Transformer
			fdjpos.open ("datajpos.txt");
			fdjvel.open ("datajvel.txt");
			fdforce.open ("dataforce.txt");
			fddpos.open ("datadpos.txt");
			fdmvel.open ("datamvel.txt");
			fdds.open ("datads.txt");
			fdves.open ("dataves.txt");		
			//Rotate the neck	
			YVIS->v_pan=-80;
			YVIS->v_tilt=-15;
			YVIS->neck=1;
			//Open hand
			if(!HandThread.started)
			{				
				//Open hand for hold hand
				HandThread.mode=2;
				HandThread.angle_f=90;
				HandThread.start();									
			}
			// Say something
			search_handle=0;
			search_tar=0;
			search_loc=0;
			
			ArosArm->flag_comp=1;			

			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			ArosArm->see_target=0;
			flag_armstop=0;

			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;			
			YPHRI->Send(AMTEC_GET_POS_ALL);			
			ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&ArosArm->jtheta_p[0],&YPHRI->jtheta[0],7);
			ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
			// Update force at the wrist			
			YPHRI->Send(AMTEC_GET_FORCE_TORQUE);
			ArosArm->fForceCompensated(YPHRI->jtheta,YPHRI->fW,ArosArm->fW0);
		
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;

			while(!stop)
			{
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				temp=sqrt(ArosArm->fW[0]*ArosArm->fW[0]+ArosArm->fW[1]*ArosArm->fW[1]+ArosArm->fW[2]*ArosArm->fW[2]);
				if(temp>3) 
				{
					break;
				}
			}
			ArosArm->flag_comp=1;			

			ArosArm->flag_torquez=1;
			ArosArm->flag_torquey=0;
			ArosArm->normal=1;
			ArosArm->flag_say=0;
			ArosArm->flag_obs=0;
			flag_notify=0;
			ArosArm->see_target=0;
			flag_armstop=0;

			ArosArm->start=GetTickCount();
			tcount=GetTickCount()-ArosArm->start;
			tims=static_cast<float>( tcount )/1000.0f;
			tim=tims;
			timp=tims;
			ArosArm->fReset(tim);
			dmobp[0]=0;
			dmobp[1]=0;				
		
			//ArosArm->jlimEnable=0;
			ArosArm->singEnable=0;

			YSPE->msg.uCommand=AUDIO_MESSAGE;
			YSPE->msg.strText="Lead the way";
			YSPE->Send(YSPE->msg);
			
			while(!stop)
			{	
				timp=tim;
				tcount=GetTickCount()-ArosArm->start;
				tim=static_cast<float>( tcount )/1000.0f;				
				YPHRI->Send(AMTEC_GET_POS_ALL);				
				ArosArm->fCopy(&ArosArm->jtheta_n[0],&YPHRI->jtheta[0],7);
				ArosArm->fCopy(&jtheta[0],&YPHRI->jtheta[0],7);	
				//Update force at the wrist
				YPHRI->Send(AMTEC_GET_FORCE_TORQUE);				
				ArosArm->fForceProcessing(YPHRI->jtheta,YPHRI->fW,tim);
				//ArosArm->fForceStartComp();
				if(tim<tims+1)
				{
					for(int i=0;i<6;i++)
					{
						ArosArm->fW[i]=0;
					}
				}
				
				YOBS->Send(RANGE_FINDER_GET_LAST_SCAN);
				ArosArm->fCopy(ArosArm->obs_g,YOBS->obs_g,85);
				//ArosArm->fMobileIntegrating(dpos,dmob,tim);	
				ArosArm->fMobile(dmob,tim);
				
				ArosArm->fEndEffector(dmob,dpos,tim,5);
				dtt=tim-ArosArm->timIntegrate;
				ArosArm->timIntegrate=tim;				
				// Say
				
				if(ArosArm->flag_say==1)
				{
					ArosArm->flag_say=0;								
						
					if(ArosArm->flag_obs==1)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. I recommend to go to my left";
						YSPE->Send(YSPE->msg);
					}	
					
					if((ArosArm->flag_obs==-1)||(ArosArm->flag_obs==-2))
					{
						ArosArm->flag_obs=0;
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="We can continue safely now";
						YSPE->Send(YSPE->msg);
					}
					if(ArosArm->flag_obs==2)
					{
						YSPE->msg.uCommand=AUDIO_MESSAGE;
						YSPE->msg.strText="Obstacles are blocking my way. Let's go to my right";
						YSPE->Send(YSPE->msg);
					}						
				}
				//---------------------------
				
				if((ArosArm->ypr[1]<-40*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
				{
					state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
					ArosArm->DiffKine(jtheta,djtheta,dpos_check);
					if(dpos_check[4]<0)//PITCH decreases more!
					{
						mask[4]=1;
						dpos[4]=0.01f;
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						mask[4]=0;
					}
					
				}else
				{
					if((ArosArm->ypr[1]>30*M_PI/180)&&(abs(ArosArm->jtheta_n[1]-M_PI/2)>15*M_PI/180))
					{
						state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
						ArosArm->DiffKine(jtheta,djtheta,dpos_check);
						if(dpos_check[4]>0)//PITCH increases more!
						{
							mask[4]=1;
							dpos[4]=-0.01f;
							state=ArosArm->InvDiffKine(dpos,jtheta,djtheta,mask,tim);
							mask[4]=0;
						}
						
					}else
					{
						state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);
					}
				};
				
				//mask[4]=1;
				
				//state=ArosArm->InvDiffKine(dpos,ArosArm->jtheta_n,djtheta,mask,tim);				
				
				ArosArm->DiffKine(ArosArm->jtheta_n,djtheta,dpost);
				
				//Record data
				for (int i=0;i<6;i++)
					{
						fddpos << dpos[i] << " ";
					}
				fddpos << ";";
				for (int i=0;i<7;i++)
				{
					fdjpos << jtheta[i] << " ";
				}
				fdjpos << ";";
					
					
				for (int i=0;i<7;i++)
				{
					fdjvel << djtheta[i] << " ";
				}
				fdjvel << ";";
				fdmvel << dmob[0]*1000 << " " << dmob[1] << ";";

				fdves << ArosArm->ddL_ves[0] << " " << ArosArm->dL_ves[0] << " " << ArosArm->L_ves[0] <<" ";
				fdves << ArosArm->ddL_ves[1] << " " << ArosArm->dL_ves[1] << " " << ArosArm->L_ves[1] << ";";		
				
				if(state!=1) 
				{				
					if(flag_armstop==0)
					{
						//Arm can not respond
						YPHRI->Send(AMTEC_SOFT_STOP);
						YMOB->Send(MOTORS_STOP);						
						if(flag_notify==0)
						{
							tim_notify=tim;
							flag_notify=1;
							
							YSPE->msg.uCommand=AUDIO_MESSAGE;
							YSPE->msg.strText="You are hurting my arm";
							YSPE->Send(YSPE->msg);
							
						}
						if(tim>tim_notify+2)
						{						
							flag_notify=0;
						}						
					}
					flag_armstop=1;
					//timc=tim;			
				}else
				{		
					if(flag_armstop==1)
					{
						tcount=GetTickCount()-ArosArm->start;
						tims=static_cast<float>( tcount )/1000.0f;
						tim=tims;
						timp=tims;
						ArosArm->fReset(tim);
						dmobp[0]=0.0f;
						dmobp[1]=0.0f;
						flag_armstop=0;
						continue;
					}
					
					//Send to motors				
					YPHRI->msg.uCommand=AMTEC_MOVE_VEL;
					YPHRI->msg.uParam.resize(0);
					YPHRI->msg.fData.resize(7);
					for(int i=0;i<7;i++)
					{
						YPHRI->msg.fData[i]=djtheta[i];
					}	
					YPHRI->Send(YPHRI->msg);
					//Mobile platform
					if((abs(dmob[0]-dmobp[0])>0.000001)||(abs(dmob[1]-dmobp[1])>0.0001))
					{
						//Mobile platform
						YMOB->msg.uCommand=MOTORS_SET_VELOCITY;
						YMOB->msg.uParam.resize(0);
						YMOB->msg.fData.resize(2);
						YMOB->msg.fData[0]=dmob[0];
						YMOB->msg.fData[1]=dmob[1];
						YMOB->Send(YMOB->msg);
					}	
					
					
				}					
				dmobp[0]=dmob[0];
				dmobp[1]=dmob[1];								
			}
			YMOB->Send(MOTORS_STOP);
			YPHRI->Send(AMTEC_SOFT_STOP);
			fdjpos.close();
			fdjvel.close();
			fdforce.close();
			fddpos.close();
			fdmvel.close();
			fdds.close();
			fdves.close();
			break;
				
		//----------------------
	}
	//Exit thread
		
}