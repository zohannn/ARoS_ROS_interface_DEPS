/* This file contains the ARM specific commands processor and helper methods */

#include <sstream>
#include "YarpCommunicationServerUpperLimb.h"


bool CYarpCommunicationServerUpperLimb::ArmCheckCommand(unsigned int cmd)
{
	switch (cmd)
	{
	case AMTEC_TERMINATE:
	case AMTEC_HOME:
	case AMTEC_HOME_JOINT:
	case AMTEC_RESET_JOINT:
	case AMTEC_RESET_ALL:
	case AMTEC_STOP_JOINT:
	case AMTEC_STOP_ALL:
	case AMTEC_MOVE_STEP:
	case AMTEC_MOVE_STEP_ALL:
	case AMTEC_MOVE_ALL_VEL:
	case AMTEC_MOVE_ALL_VEL_TRAJ:
	case AMTEC_GET_POS:
	case AMTEC_GET_POS_ALL:
	case AMTEC_MOVE_STEP_ALL_SAME_TIME:
	case AMTEC_OPENGRIPPER:
	case AMTEC_CLOSEGRIPPER:
	case AMTEC_GET_FORCE_TORQUE:
	case AMTEC_RESTART_MODULE:
	case AMTEC_MOVE_VEL_JOINT:
	case AMTEC_MOVE_VEL:
	case AMTEC_HOME_ALL:
	case AMTEC_GET_POS_DEG:
	case AMTEC_GET_POS_DEG_ALL:
	case AMTEC_MOVE_STEP_WAIT:
	case AMTEC_FREE_JOINT:
	case AMTEC_GET_GRIPPER:
	case AMTEC_HOME_GRIPPER:
	case AMTEC_INV_KIN:
	case AMTEC_SOFT_STOP:
	case AMTEC_MOVE_STEP_SAME_TIME_WAIT:
	case AMTEC_WAIT_MOTION_END:
	case AMTEC_MOVE_STEP_SAME_TIME_SEL:
	case AMTEC_MOVE_STEP_TRAJECTORY:
	case AMTEC_WAIT_TRAJECTORY_END:
	case AMTEC_STOP_CLEAR_TRAJECTORY:
	case AMTEC_WAIT_TRAJECTORY_END_ASYNC:
	case AMTEC_TRAJECTORY_ASYNC_FINISHED:
		if (!arm)
			throw std::string("Error: Command is not acceptable because Arm is hardware disabled!");
		break;
	default:
		return false;
	}
	return true;
}

bool CYarpCommunicationServerUpperLimb::ArmProcessor(CMessage &in_req, CMessage &out_reply,  void *private_data )
{
	int
		nJoints = 0,
		returned_code = 0,
		nAux = 0;

	float fAux = 0.0f;

	std::stringstream joint_text;

	if (!ArmCheckCommand(in_req.uCommand))
		return false;

	//--------------------------------------------------------------------------
	switch (in_req.uCommand)
	{
	case AMTEC_GET_POS:
		if (in_req.uParam.size() < 1)
		{
			out_reply.iErrorCode = -1;
			out_reply.strText = "Undefined joint number!";
		}
		else
		{
			out_reply.fData.resize(1);
			out_reply.fData[0] = GetPos((int)in_req.uParam[0]);
		}
		break;

		//--------------------------------------------------------------------------
	case AMTEC_GET_POS_ALL:
		GetPosAll(out_reply.fData);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_HOME_JOINT:
		if (in_req.uParam.size() < 1)
		{
			out_reply.iErrorCode = -1;
			out_reply.strText = "Undefined joint number!";
		}
		else
		{
			HomeJoint((int)in_req.uParam[0]);
		}

		break;

		//--------------------------------------------------------------------------
	case AMTEC_HOME:
		arm->getAxes(&nJoints);
		if (in_req.uParam.size() < (unsigned)nJoints)
		{
			joint_text << nJoints;
			out_reply.iErrorCode = -1;
			out_reply.strText = "Home: Array size to small, it must have " + joint_text.str() + "elements.";
		}
		else
		{
			Home(in_req.uParam);
		}
		break;

		//--------------------------------------------------------------------------
	case AMTEC_HOME_ALL:
		HomeAll();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_RESET_JOINT:
		ResetJoint((int)in_req.uParam[0]);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_RESET_ALL:
		ResetAll();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_STOP_JOINT:
		StopJoint((int)in_req.uParam[0]);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_STOP_ALL:
		StopAll();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP:
		if (in_req.fData.size() < 3)
		{
			out_reply.iErrorCode = -1;
			out_reply.strText = "Corrupted data";

		}
		else
		{
			MoveStep((int)in_req.fData[0], in_req.fData[1], (unsigned short)in_req.fData[2]);
		}

		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_WAIT:
		if (in_req.fData.size() < 3)
		{
			out_reply.iErrorCode = -1;
			out_reply.strText = "Corrupted data";

		}
		else
		{
			MoveStep((int)in_req.fData[0], in_req.fData[1], (unsigned short)in_req.fData[2], true);
		}

		break;

		//--------------------------------------------------------------------------
	case AMTEC_FREE_JOINT:
		if (in_req.uParam.size() < 1)
		{
			out_reply.iErrorCode = -1;
			out_reply.strText = "Undefined joint number!";
		}
		else
		{
			freeJoint((int)in_req.uParam[0]);
		}
		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_ALL:
		MoveStepAll(in_req.fData);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_ALL_SAME_TIME:
		MoveStepAllSameTime(in_req.fData);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_GET_POS_DEG:
		out_reply.fData.resize(1);
		out_reply.fData[0] = GetPosDeg((int)in_req.uParam[0]);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_GET_POS_DEG_ALL:
		out_reply.fData = GetPosDegAll();
		break;

	case AMTEC_GET_FORCE_TORQUE:
		out_reply.fData = Get_Force_Torque();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_HOME_GRIPPER:
		HomeGripper();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_RESTART_MODULE:
		Restart();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_SOFT_STOP:
		SoftStop();
		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_SAME_TIME_WAIT:
		MoveStepSameTimeWait(in_req.fData);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_WAIT_MOTION_END:
		if (in_req.fData.size() > 0)
		{
			// Perform wait based in angle margin
			fAux = in_req.fData.back();
			in_req.fData.pop_back();
			WaitMotionEnd(in_req.fData, fAux);
		}

		else if (in_req.uParam.size() > 0)
		{
			// Perform wait based in time
			nAux = WaitMotionEnd(in_req.uParam.front());

			out_reply.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>(nAux)
				);
		}

		else
		{
			// Error in command
			out_reply.iErrorCode = -1;
			out_reply.strText += "uParam[0]: wait time (optional).";
			out_reply.strText += "\nfData: target angles for the arms (optional).";
		}

		break;
	default:
			//we did not processed the command
		return false;
	}
	return true;
}

void CYarpCommunicationServerUpperLimb::HomeJoint(int joint)
{
	if (!arm->HomeJoint(joint))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::Home(std::vector<unsigned int> &home_order)
{
	if (!arm->Home(home_order))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::HomeAll(void)
{
	if (!arm->HomeAll())
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::HomeGripper(void)
{
	if (!arm->HomeGripper())
		throw arm->Get_Error_Str();
}

std::string CYarpCommunicationServerUpperLimb::Get_Device_Error(void)
{
	return arm->Get_Error_Str();
}

bool CYarpCommunicationServerUpperLimb::isArmStatusOK(void)
{
	return arm->isArmStatusOK();
}

void CYarpCommunicationServerUpperLimb::freeJoint(int nJoint)
{
	if (!arm->freeJoint(nJoint))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::ResetJoint(int joint)
{
	if (!arm->Reset(joint))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::ResetAll(void)
{
	if (!arm->ResetAll())
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::Restart()
{
	std::cout << "Restarting module: ";

	std::cout << (arm->Restart_Module() ? "ok" : " : error") << std::endl;
}

void CYarpCommunicationServerUpperLimb::StopJoint(int joint)
{
	if (!arm->stop(joint))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::StopAll(void)
{
	if (!arm->stop())
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::SoftStop(void)
{
	if (!arm->SoftStop())
		throw arm->Get_Error_Str();
}


int CYarpCommunicationServerUpperLimb::GetNumberJoints(void)
{
	int n = 0;
	if (!arm->getAxes(&n))
		throw arm->Get_Error_Str();
	return n;
}

float CYarpCommunicationServerUpperLimb::GetPos(int joint)
{
	float fAux = 0.0f;
	if (!arm->getPos(joint, &fAux))
		throw arm->Get_Error_Str();
	return fAux;
}

void CYarpCommunicationServerUpperLimb::GetPosAll(std::vector<float> &data)
{
	int number = 0;

	if (!arm->getAxes(&number))
		throw arm->Get_Error_Str();


	data.resize(number);

	if (!arm->getPosAll(data.data()))
		throw arm->Get_Error_Str();
}

float CYarpCommunicationServerUpperLimb::GetPosDeg(int joint)
{
	float fAux = 0.0f;
	if (!arm->getPosDeg(joint, &fAux))
		throw arm->Get_Error_Str();
	return fAux;
}

std::vector<float> CYarpCommunicationServerUpperLimb::GetPosDegAll(void)
{
	std::vector<float> fData;

	int number = 0;

	if (!arm->getAxes(&number))
		throw arm->Get_Error_Str();

	fData.resize(number);

	if (!arm->getPosAllDeg(fData.data()))
		throw arm->Get_Error_Str();

	return fData;
}

std::vector<float> CYarpCommunicationServerUpperLimb::Get_Force_Torque(void)
{
	std::vector<float> fData;
	float data[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	fData.assign(6, 0.0f);

	int error_code_ = arm->get_force_torque(data);

	if( error_code_ < 0 ) 
		throw arm->Get_Error_Str();
	else
	{
		for (unsigned int i = 0; i < 6; i++)
			fData.at(i) = data[i];
	}

	return fData;
}


void CYarpCommunicationServerUpperLimb::GetJointLimits(int joint, float * fLimits)
{
	if (!arm->getMinLimit(joint, &fLimits[0]))
		throw arm->Get_Error_Str();
	if (!arm->getMaxLimit(joint, &fLimits[1]))
		throw arm->Get_Error_Str();
}

int CYarpCommunicationServerUpperLimb::WaitMotionEnd(unsigned int timeout_ms)
{
	int state = 0;
	if (!arm->waitMotionEnd(timeout_ms, &state))
		throw arm->Get_Error_Str();
	return state;
}

void CYarpCommunicationServerUpperLimb::WaitMotionEnd(std::vector<float> &refs, float stop_margin)
{
	if (!arm->waitMotionEnd(refs.data(), stop_margin))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveJointPosition(int joint, float value)
{
	if (!arm->positionMove(joint, static_cast<double>(value)))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveJointPositionRamp(int joint, float pos)
{
	if (!arm->positionMoveRamp(joint, pos))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveJointPositionRamp(int joint, float pos, float vel, float acc)
{
	if (!arm->positionMoveRamp(joint, pos, vel, acc))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveStep(int joint, float value, unsigned short time, bool wait_end)
{
	if (!arm->positionMove(joint, value, time, wait_end))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveStepAll(std::vector<float> &fData)
{
	unsigned short usTimes[7] = { 0, 0, 0, 0, 0, 0, 0 };
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i<14; i += 2)
	{
		fJoints[i] = fData.at(i);
		usTimes[i] = static_cast<unsigned short>(fData.at(i + 1));
	}

	if (!arm->positionMove(fJoints, usTimes))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveStepSameTime(std::vector<float> *fData, std::vector<unsigned int> *joints2move)
{
	bool bJoints2Move[7] = { false, false, false, false, false, false, false };
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i<7; i++)
	{
		fJoints[i] = (*fData)[i];
		bJoints2Move[i] = (*joints2move)[i] == 0 ? false : true;
	}

	if (!arm->positionMove(fJoints, bJoints2Move, static_cast<unsigned short>(fData->back()), false))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveStepSameTimeWait(std::vector<float> &fData)
{
	if (!arm->positionMove(fData.data(), static_cast<unsigned short>(fData.back()), true))
		throw arm->Get_Error_Str();
}

void CYarpCommunicationServerUpperLimb::MoveStepAllSameTime(std::vector<float> &fData)
{
	if (!arm->positionMove(fData.data(), static_cast<unsigned short>(fData.back())))
		throw arm->Get_Error_Str();
}


