#include "ArmMovements.h"
#include "ArmMovements_ARoS.h"
#include "ArmMovements_Dumbo.h"
#include "ArmMovements_Jumbo.h"

bool check_error( CYarpCommunicationServerAmtecLWArm * arm, int iJoint )
{
	std::string error_str;
	if( !arm->Success() )
	{
		error_str = arm->Get_Device_Error();
		std::cerr << error_str << std::endl;
		if( iJoint != -1 )
		{
			bool sic = arm->Amtec_Arm.Update_State_Error( iJoint );
			if( !sic )
			{
				std::cerr << "state error:" << std::endl;
				std::cerr << arm->Get_Device_Error() << std::endl;
			}
			else
				std::cerr << "No state error" << std::endl;

			sic = arm->Amtec_Arm.Update_State_Info( iJoint );

			if( arm->Amtec_Arm.vec_str_error.size() )
			{
				for( unsigned i = 0; i < arm->Amtec_Arm.vec_str_error.size(); i++ )
					error_str += arm->Amtec_Arm.vec_str_error[i] + "\n";
				std::cerr << "state info:" << std::endl;
				std::cerr << error_str << std::endl;
			}
			else
				std::cerr << "No state info" << std::endl;
		}

		return false;
	}
	return true;

}

void ArmMaintenance( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file )
{
	yarp::os::Property prop;
	
	bool bFilePresent = prop.fromConfigFile( config_file.c_str() );
	
	if( !bFilePresent ) return;

	std::string robot = prop.find( "robot" ).asString();
	
	int iarm = prop.find( "arm" ).asInt();

	if( robot.compare( "aros" ) == 0 )
	{
		if(iarm==0)
			PerformArmMaintenance_ARoS_Right( arm_ptr );
		else
			PerformArmMaintenance_ARoS_Left( arm_ptr );
	}
	
	else if( robot.compare( "dumbo" ) == 0 )
	{
		PerformArmMaintenance_Dumbo( arm_ptr );
	}

	else if( robot.compare( "jumbo" ) == 0 )
	{
		PerformArmMaintenance_Jumbo( arm_ptr );
	}
}

void ArmGoHome( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file )
{
	yarp::os::Property prop;
	
	bool bFilePresent = prop.fromConfigFile( config_file.c_str() );
	
	if( !bFilePresent ) return;

	std::string robot = prop.find( "robot" ).asString();

	int iarm = prop.find( "arm" ).asInt();

	if( robot.compare( "aros" ) == 0 )
	{
		if(iarm==0)
			PerformGoHome_ARoS_Right( arm_ptr );
		else
			PerformGoHome_ARoS_Left( arm_ptr );
	}
	
	/*else if( robot.compare( "dumbo" ) == 0 )
	{
		PerformArmMaintenance_Dumbo( arm_ptr );
	}*/

	else if( robot.compare( "jumbo" ) == 0 )
	{
		PerformGoHome_Jumbo( arm_ptr );
	}
}

void RestPosition( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file )
{
	yarp::os::Property prop;
	
	bool bFilePresent = prop.fromConfigFile( config_file.c_str() );
	
	if( !bFilePresent ) return;

	std::string robot = prop.find( "robot" ).asString();

	int iarm = prop.find( "arm" ).asInt();
	
	if( robot.compare( "aros" ) == 0 )
	{
		if(iarm==0)
			PerformRestPosition_ARoS_Right( arm_ptr );
		else
			PerformRestPosition_ARoS_Left( arm_ptr );
	}
	/*else if( robot.compare( "dumbo" ) == 0 )
	{
		PerformArmMaintenance_Dumbo( arm_ptr );
	}*/

	else if( robot.compare( "jumbo" ) == 0 )
	{
		PerformRestPosition_Jumbo( arm_ptr );
	}
}