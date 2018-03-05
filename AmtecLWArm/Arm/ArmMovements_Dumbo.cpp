#include "ArmMovements.h"
#include "ArmMovements_Dumbo.h"
#include <string>
#include <iostream>

void PerformArmMaintenance_Dumbo( CYarpCommunicationServerAmtecLWArm * arm )
{
	std::string error_str;
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

	arm->HomeJoint( 5 );
	if( !check_error( arm, 5 ) )
		return;

	arm->HomeJoint( 0 );
	if( !check_error( arm, 0 ) )
		return;
	
	arm->MoveJointPositionRamp( 0, 0.0f );
	if( !check_error( arm, 0 ) )
		return;

	arm->HomeAll();
	if( !check_error( arm, -1 ) )
		return;

	// Put the arm straight
	std::vector< float > vPos;
	vPos.assign( 8, 0.0f );
	vPos[3] = 90.0f*DEG_TO_RAD_F;
	vPos.back() = 10000.0f; // Time to complete movement in ms
	arm->MoveStepAllSameTime( vPos ); // Move all joints to 'zero'
	if( !check_error(arm, -1) )
		return;

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
	if( !check_error( arm, -1 ) )
		return;

	//-------------------------------------------------
	// Open and close the gripper if present
	if( nJoints>7 )
	{
		for( int i=0 ; i<nRepeat ; i++ )
		{
			arm->OpenGripper();
			if( !check_error(arm, nJoints) )
				return;

			arm->CloseGripper( 0.010f,5000 );
			if( !check_error(arm, nJoints) )
				return;
		}

		arm->CloseGripper( 0.0345f,5000 ); // Position for gripper half open
		if( !check_error(arm, nJoints) )
			return;
	}
	//-------------------------------------------------

	for( int iJoint=nJoints-1 ; iJoint>=0 ; iJoint-- )
	{
		arm->GetJointLimits( iJoint, fLimit );
		if( !check_error(arm, iJoint) )
			return;
		
		fLimit[1] *= 0.99f;
	
		// Some joints need special restrictions for safety purposes.
		switch( iJoint )
		{
		case 3:
			// Make the target position 40% of the limit (for safety reasons)
			fLimit[0] = 0.0f;
			fLimit[2] = 90.0f*DEG_TO_RAD_F;
			break;
		
		case 1:
			fLimit[0] = 0.0f;
			fLimit[1] = 90.0f*DEG_TO_RAD_F;
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
			if( !check_error( arm, iJoint ) )
				return;

			arm->MoveJointPositionRamp( iJoint, fLimit[1] );
			if( !check_error( arm, iJoint ) )
				return;

		}

		arm->MoveJointPositionRamp( iJoint, fLimit[2] );
		if( !check_error( arm, iJoint ) )
		return;
	}

	//---------------------------------------------------------------------
	// Move arm to rest position
	arm->MoveJointPositionRamp( 3, 0.0f );
	if( !check_error( arm, 3 ) )
		return;
	arm->MoveJointPositionRamp( 2, 90.0f*DEG_TO_RAD_F );
	if( !check_error( arm, 2 ) )
		return;

	arm->MoveJointPositionRamp( 1, 0.0f );
	if( !check_error( arm, 1 ) )
		return;

	arm->MoveJointPositionRamp( 0, -90.0f*DEG_TO_RAD_F );
	if( !check_error( arm, 0 ) )
		return;
	
	arm->MoveJointPositionRamp( 5, 110.0f*DEG_TO_RAD_F );
	if( !check_error( arm, 0 ) )
		return;

	// All done!
}
