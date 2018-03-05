#include "ArmMovements.h"
#include "ArmMovements_ARoS.h"
#include <iostream>

void PerformArmMaintenance_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm from rest position to home position safely

	arm->HomeJoint( 1 );
	
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );

	arm->HomeJoint( 0 );

	arm->HomeAll();

	std::vector< float > vPos;
	vPos.assign( 8, 0.0f );
	vPos[3] = 90.0f*DEG_TO_RAD_F;
	vPos.back() = 10000.0f; // Time to complete movement in ms
	arm->MoveStepAllSameTime( vPos ); // Put the arm straight

	//---------------------------------------------------------------------
	// The exercise!

	float 
		fLimit[3] = 
		{ 
			0.0f, // Min
			0.0f, // Max
			0.0f  // Final joint position
		}; 

	const int nRepeat = 2; // Number of times to repeat movement between limits
	
	int nJoints = arm->GetNumberJoints();

	for( int iJoint=nJoints ; iJoint>1 ; iJoint-- )
	{
		arm->GetJointLimits( iJoint, fLimit );
		
		fLimit[1] *= 0.99f;
	
		// Some joints need special restrictions for safety purposes.
		switch( iJoint )
		{
		case 3:
			// Make the target position 40% of the limit (for safety reasons)
			fLimit[0] *= 0.4f;
			fLimit[2] = 90.0f*DEG_TO_RAD_F;
			break;

		default:
			// Make the target position 99% of the limit
			fLimit[0] *= 0.99f;
			fLimit[2] = 0.0f;
			break;
		}

		for( int i=0 ; i<nRepeat ; i++ )
		{
			arm->MoveJointPositionRamp( iJoint, fLimit[0] );
			arm->MoveJointPositionRamp( iJoint, fLimit[1] );
		}

		arm->MoveJointPositionRamp( iJoint, fLimit[2] );
	}

	//---------------------------------------------------------------------
	// Move arm to rest position
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 0, -90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 1, 0.0f );

	// All done!
}

void PerformGoHome_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm from rest position to home position safely

	arm->HomeJoint( 1 );
	
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );

	arm->HomeJoint( 0 );

	arm->HomeAll();

	std::vector< float > vPos;
	vPos.assign( 8, 0.0f );
	//vPos[3] = 90.0f*DEG_TO_RAD_F;
	vPos.back() = 10000.0f; // Time to complete movement in ms
	arm->MoveStepAllSameTime( vPos ); // Put the arm straight
}

void PerformRestPosition_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm to rest position
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 3, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 0, -90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 1, 0.0f );

	// All done!
}

void PerformArmMaintenance_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm from rest position to home position safely

	arm->HomeJoint( 1 );
	
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );

	arm->HomeJoint( 0 );

	arm->HomeAll();

	//*********************************************************************************************
	// Alterado devido ao problema do braço esquerdo por vezes detetar a posição de home da junta 3 errada
	// A forma de corrigir o problema é fazer a junta 3 ir a home e depois para a posição -15
	// Depois voltamos a fazer home, e aí, em princípio, já vai para a posição correta de home
	// by Luis Louro 19/04/2013
	/*int Answer=0;

	std::cout <<
		"Is the arm in the home position?" << std::endl << 
			"[1]-Yes" << std::endl << 
			"[0]-No " << std::endl <<
			"Option: ";

	std::cin >> Answer;

	if(Answer==0)
	{
		arm->MoveJointPositionRamp( 3, -24.0f*DEG_TO_RAD_F );

		arm->HomeJoint( 3 );

		std::cout << "Now the arm is in the home position?" << std::endl << 
			"[1]-Yes" << std::endl << 
			"[0]-No " << std::endl <<
			"Option: ";

		std::cin >> Answer;

		if(Answer==0)
		{
			std::cerr << std::endl <<
				"Solve the problem manually. Impossible to do the Arm Maintenance" << std::endl;

			return;
		}
	}*/

	std::vector< float > vPos;
	vPos.assign( 8, 0.0f );
	vPos[3] = 90.0f*DEG_TO_RAD_F;
	vPos.back() = 10000.0f; // Time to complete movement in ms
	arm->MoveStepAllSameTime( vPos ); // Put the arm straight

	//---------------------------------------------------------------------
	// The exercise!

	float 
		fLimit[3] = 
		{ 
			0.0f, // Min
			0.0f, // Max
			0.0f  // Final joint position
		}; 

	const int nRepeat = 2; // Number of times to repeat movement between limits
	
	int nJoints = arm->GetNumberJoints();

	for( int iJoint=nJoints ; iJoint>1 ; iJoint-- )
	{
		arm->GetJointLimits( iJoint, fLimit );
		
		fLimit[1] *= 0.99f;
	
		// Some joints need special restrictions for safety purposes.
		switch( iJoint )
		{
		case 3:
			// Make the target position 40% of the limit (for safety reasons)
			fLimit[0] *= 0.4f;
			fLimit[2] = 90.0f*DEG_TO_RAD_F;
			break;

		default:
			// Make the target position 99% of the limit
			fLimit[0] *= 0.99f;
			fLimit[2] = 0.0f;
			break;
		}

		for( int i=0 ; i<nRepeat ; i++ )
		{
			arm->MoveJointPositionRamp( iJoint, fLimit[0] );
			arm->MoveJointPositionRamp( iJoint, fLimit[1] );
		}

		arm->MoveJointPositionRamp( iJoint, fLimit[2] );
	}

	//---------------------------------------------------------------------
	// Move arm to rest position
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 0, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 1, 0.0f );

	// All done!
}

void PerformGoHome_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm from rest position to home position safely

	arm->HomeJoint( 1 );
	
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );

	arm->HomeJoint( 0 );

	arm->HomeAll();

	std::vector< float > vPos;
	vPos.assign( 8, 0.0f );
	//vPos[3] = 90.0f*DEG_TO_RAD_F;
	vPos.back() = 10000.0f; // Time to complete movement in ms
	arm->MoveStepAllSameTime( vPos ); // Put the arm straight

	//*********************************************************************************************
	// Alterado devido ao problema do braço esquerdo por vezes detetar a posição de home da junta 3 errada
	// A forma de corrigir o problema é fazer a junta 3 ir a home e depois para a posição -15
	// Depois voltamos a fazer home, e aí, em princípio, já vai para a posição correta de home
	// by Luis Louro 19/04/2013
	/*int Answer=0;

	std::cout <<
		"Is the arm in the home position?" << std::endl <<
		"[1]-Yes" << std::endl <<
		"[0]-No " << std::endl <<
		"Option: ";

	std::cin >> Answer;

	if(Answer==0)
	{
		arm->MoveJointPositionRamp( 3, -24.0f*DEG_TO_RAD_F );

		arm->HomeJoint( 3 );
	}*/
}

void PerformRestPosition_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm )
{
	//---------------------------------------------------------------------
	// Perform safety check. If an error is present abort maintenance.

	if( !arm->isArmStatusOK() )
	{
		std::cerr << std::endl << std::endl <<
			"***** One of the arm modules has an error! *****" << 
			std::endl << std::endl;

		return;
	}

	//---------------------------------------------------------------------
	// Move arm to rest position
	arm->MoveJointPositionRamp( 1, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 3, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 0, 90.0f*DEG_TO_RAD_F );
	arm->MoveJointPositionRamp( 1, 0.0f );

	// All done!
}
