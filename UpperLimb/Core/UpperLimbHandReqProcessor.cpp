/* This file contains the HAND specific commands processor and helper methods */

#include "UpperLimbReqProcessor.h"
#include <sstream>

void UpperLimbReqProcessor::PrepareHandProcessor()
{
	//start processor only if hand was hardware enabled
	if (hand)
		hand_processor = boost::thread(boost::bind(&UpperLimbReqProcessor::HandExecutor, this));
}

void UpperLimbReqProcessor::HandExecutor()
{
	double HandConfig[4];
	do
	{
		if (Finger1.size()>0 && Finger2.size()>0 && Finger3.size()>0)
		{
			//comm_manager.mutex_barrett.wait();
			HandConfig[1] = (int)Finger1[0];
			HandConfig[2] = (int)Finger2[0];
			HandConfig[3] = (int)Finger3[0];

			if (HandConfig[1] == HandConfig[2] && HandConfig[1] == HandConfig[3])
			{
				hand->positionMoveFingers(HandConfig[1]);
			}
			else if (HandConfig[1] == HandConfig[2])
			{
				hand->positionMoveTwoFingers(HandConfig);
			}
			else
			{
				hand->positionMoveFingers(HandConfig);
			}
			//comm_manager.mutex_barrett.post();

			Finger1.erase(Finger1.begin());
			Finger2.erase(Finger2.begin());
			Finger3.erase(Finger3.begin());


		}
		else
		{
			if (Finger1.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[1] = Finger1[0];

				hand->positionMove(1, HandConfig[1]);
				//comm_manager.mutex_barrett.post();

				Finger1.erase(Finger1.begin());
			}
			if (Finger2.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[2] = Finger2[0];

				hand->positionMove(2, HandConfig[2]);
				//comm_manager.mutex_barrett.post();

				Finger2.erase(Finger2.begin());

			}
			if (Finger3.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[3] = Finger3[0];

				hand->positionMove(3, HandConfig[3]);

				//comm_manager.mutex_barrett.post();
				Finger3.erase(Finger3.begin());
			}
		}

		if (Spread.size()>0)
		{
			//comm_manager.mutex_barrett.wait();
			HandConfig[0] = Spread[0];

			hand->positionMove(0, HandConfig[0]);
			//comm_manager.mutex_barrett.post();

			Spread.erase(Spread.begin());
		}
		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
	} while (!stop_executor);
}

bool UpperLimbReqProcessor::HandCheckCommand(unsigned int cmd)
{
	switch (cmd)
	{
	case BARRETTHAND_TERMINATE:
	case BARRETTHAND_INITIALIZE:
	case BARRETTHAND_INITJOINT:
	case BARRETTHAND_OPENJOINT:
	case BARRETTHAND_OPEN:
	case BARRETTHAND_CLOSEJOINT:
	case BARRETTHAND_CLOSE:
	case BARRETTHAND_TORQUEOPEN_JOINT:
	case BARRETTHAND_TORQUEOPEN:
	case BARRETTHAND_TORQUECLOSE_JOINT:
	case BARRETTHAND_TORQUECLOSE:
	case BARRETTHAND_MOVEJOINT:
	case BARRETTHAND_MOVEALL:
	case BARRETTHAND_SETOPENTARGET:
	case BARRETTHAND_SETCLOSETARGET:
	case BARRETTHAND_GETOPENTARGET:
	case BARRETTHAND_GETCLOSETARGET:
	case BARRETTHAND_SETVELOCITY:
	case BARRETTHAND_GETVELOCITY:
	case BARRETTHAND_SETACCELERATION:
	case BARRETTHAND_GETACCELERATION:
	case BARRETTHAND_INCREMENTALMOVEJOINT:
	case BARRETTHAND_INCREMENTALMOVEALL:
	case BARRETTHAND_SETOPENTARGETJOINT:
	case BARRETTHAND_SETCLOSETARGETJOINT:
	case BARRETTHAND_GETOPENTARGETJOINT:
	case BARRETTHAND_GETCLOSETARGETJOINT:
	case BARRETTHAND_MOVEFINGERS:
	case BARRETTHAND_MOVESPREAD:
	case BARRETTHAND_MOVETWOFINGERS:
	case BARRETTHAND_SETOPENVELOCITY:
	case BARRETTHAND_SETCLOSEVELOCITY:
	case BARRETTHAND_GETSTRAINGAUGE:
	case BARRETTHAND_GETPOSITIONS:
	case BARRETTHAND_MOVETWOFINGERS_WAIT:
	case BARRETTHAND_MOVEALLFINGERS_WAIT:
	case BARRETTHAND_MOVEFINGERS_WAIT:
	case BARRETTHAND_MOVESPREAD_WAIT:
	case BARRETTHAND_MOVEJOINT_WAIT:
	case BARRETTHAND_MOVEALLFINGERS:
	case BARRETTHAND_SETMAXSTRAINGAUGE:
	case BARRETTHAND_GETMAXSTRAINGAUGE:
	case BARRETTHAND_WRITE_COMMAND_RAW:
		if (!hand)
			throw std::string("Error: Command is not acceptable because Hand is hardware disabled!");
		break;
	default:
		return false;
	}
	return true;
}

bool UpperLimbReqProcessor::HandProcessor(CMessage &in_req, CMessage &out_reply, YarpConnector &con)
{
	bool bCommandSuccess = true;

	//check if it is a hand command, and throw if hand is hardware disabled
	if (!HandCheckCommand(in_req.uCommand))
		return false;

	//--------------------------------------------------------------------------
	switch (in_req.uCommand)
	{
	case BARRETTHAND_INITIALIZE:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = Initialize();
	}
	break;

	case BARRETTHAND_INITJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = InitJoint(in_req.fData);
	}
	break;

	case BARRETTHAND_OPENJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = OpenJoint(in_req.fData);
	}
	break;

	case BARRETTHAND_OPEN:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		hand_machine_state = HandMachineState::Releasing;
		bCommandSuccess = Open();
	}
	break;

	case BARRETTHAND_CLOSEJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = CloseJoint(in_req.fData);
	}
	break;

	case BARRETTHAND_CLOSE:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		hand_machine_state = HandMachineState::Grasping;
		bCommandSuccess = Close();
	}
	break;

	case BARRETTHAND_TORQUEOPEN_JOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = OpenTorqueJoint(in_req.fData);
	}
	break;

	case BARRETTHAND_TORQUEOPEN:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		hand_machine_state = HandMachineState::Releasing;
		bCommandSuccess = OpenTorque();
	}
	break;

	case BARRETTHAND_TORQUECLOSE_JOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = CloseTorqueJoint(in_req.fData);
	}
	break;

	case BARRETTHAND_TORQUECLOSE:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		hand_machine_state = HandMachineState::Grasping;
		bCommandSuccess = CloseTorque();
	}
	break;

	case BARRETTHAND_SETOPENTARGET:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetOpenTarget(in_req.fData);
	}
	break;

	case BARRETTHAND_SETCLOSETARGET:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetCloseTarget(in_req.fData);
	}
	break;

	case BARRETTHAND_GETOPENTARGET:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		out_reply.fData.resize(4);
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetOpenTarget(out_reply.fData);
		}
		break;

	case BARRETTHAND_GETCLOSETARGET:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		out_reply.fData.resize(4);
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetCloseTarget(out_reply.fData);
		}
		break;

	case BARRETTHAND_SETOPENTARGETJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetOpenTargetJoint((int)in_req.fData[0], (double)in_req.fData[1]);
	}
	break;

	case BARRETTHAND_SETCLOSETARGETJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetCloseTargetJoint((int)in_req.fData[0], (double)in_req.fData[1]);
	}
	break;

	case BARRETTHAND_SETMAXSTRAINGAUGE:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetMaxStrain(in_req.uParam);
	}
	break;

	case BARRETTHAND_GETOPENTARGETJOINT:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			out_reply.fData.assign(1, GetOpenTargetJoint((int)in_req.fData[0]));
		}
		break;

	case BARRETTHAND_GETCLOSETARGETJOINT:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			out_reply.fData.assign(1, GetCloseTargetJoint((int)in_req.fData[0]));
		}
		break;

	case BARRETTHAND_SETVELOCITY:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetVelocity(in_req.fData);
	}
	break;

	case BARRETTHAND_GETVELOCITY:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		out_reply.fData.assign(4, 0.0f);
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetVelocity(out_reply.fData);
		}
		break;

	case BARRETTHAND_GETPOSITIONS:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			out_reply.fData.assign(4, 0.0f);
			bCommandSuccess = GetPositions(out_reply.fData);
		}
		break;

	case BARRETTHAND_SETOPENVELOCITY:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetOpenVelocity(in_req.fData);
	}
	break;

	case BARRETTHAND_SETCLOSEVELOCITY:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetCloseVelocity(in_req.fData);
	}
	break;

	case BARRETTHAND_SETACCELERATION:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = SetAcceleration(in_req.fData);
	}
	break;

	case BARRETTHAND_GETACCELERATION:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		out_reply.fData.assign(4, 0.0f);
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetAcceleration(out_reply.fData);
		}
		break;

	case BARRETTHAND_GETSTRAINGAUGE:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetStrainGauge(out_reply.uParam);
		}
		break;

	case BARRETTHAND_GETMAXSTRAINGAUGE:
		while (!isHandAvailable())
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
		}
		{
			boost::unique_lock<boost::mutex> lck(mutex_barrett);
			bCommandSuccess = GetMaxStrain(out_reply.uParam);
		}
		break;

	case BARRETTHAND_INCREMENTALMOVEJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = IncrementalMoveJoint((int)in_req.fData[0], (double)in_req.fData[1]);
	}
	break;

	case BARRETTHAND_INCREMENTALMOVEALL:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		bCommandSuccess = IncrementalMove(in_req.fData);
	}
	break;

	case BARRETTHAND_MOVEJOINT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		switch ((int)in_req.fData[0])
		{
		case 0:
			Spread.push_back(in_req.fData[1]);
			break;
		case 1:
			Finger1.push_back(in_req.fData[1]);
			break;
		case 2:
			Finger2.push_back(in_req.fData[1]);
			break;
		case 3:
			Finger3.push_back(in_req.fData[1]);
			break;
		}
	}
	break;

	case BARRETTHAND_MOVESPREAD:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		Spread.push_back(in_req.fData[0]);
	}
	break;

	case BARRETTHAND_MOVEALL:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		Spread.push_back(in_req.fData[0]);
		Finger1.push_back(in_req.fData[1]);
		Finger2.push_back(in_req.fData[2]);
		Finger3.push_back(in_req.fData[3]);
	}
	break;

	case BARRETTHAND_MOVEALLFINGERS:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		Finger1.push_back(in_req.fData[1]);
		Finger2.push_back(in_req.fData[2]);
		Finger3.push_back(in_req.fData[3]);
	}
	break;

	case BARRETTHAND_MOVEFINGERS:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		Finger1.push_back(in_req.fData[0]);
		Finger2.push_back(in_req.fData[0]);
		Finger3.push_back(in_req.fData[0]);
	}
	break;

	case BARRETTHAND_MOVETWOFINGERS:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		Finger1.push_back(in_req.fData[0]);
		Finger2.push_back(in_req.fData[0]);
		Finger3.push_back(in_req.fData[1]);
	}
	break;

	case BARRETTHAND_MOVETWOFINGERS_WAIT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		HandConfig[1] = in_req.fData[0];
		HandConfig[2] = in_req.fData[0];
		HandConfig[3] = in_req.fData[1];
		bCommandSuccess = hand->positionMoveTwoFingers(HandConfig);
	}
	break;

	case BARRETTHAND_MOVEALLFINGERS_WAIT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		HandConfig[1] = in_req.fData[0];
		HandConfig[2] = in_req.fData[0];
		HandConfig[3] = in_req.fData[0];
		bCommandSuccess = hand->positionMoveFingers(HandConfig[1]);
	}
	break;

	case BARRETTHAND_MOVEFINGERS_WAIT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		HandConfig[1] = in_req.fData[0];
		HandConfig[2] = in_req.fData[1];
		HandConfig[3] = in_req.fData[2];
		bCommandSuccess = hand->positionMoveFingers(HandConfig);
	}
	break;

	case BARRETTHAND_MOVEJOINT_WAIT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		HandConfig[(int)in_req.fData[0]] = in_req.fData[1];
		bCommandSuccess = hand->positionMove((int)in_req.fData[0], in_req.fData[1]);
	}
	break;

	case BARRETTHAND_MOVESPREAD_WAIT:
	{
		boost::unique_lock<boost::mutex> lck(mutex_barrett);
		HandConfig[0] = in_req.fData[0];
		bCommandSuccess = hand->positionMove(0, HandConfig[0]);
	}
	break;

	case BARRETTHAND_WRITE_COMMAND_RAW:
		bCommandSuccess = hand->Write_Command_Read_Reply_Clean(in_req.strText, out_reply.strText);
		break;

	default:
		//we did not processed the command
		return false;
	}

	if (!bCommandSuccess)
		throw Report_Error_at_Command(in_req.uCommand);

	return true;
}


bool UpperLimbReqProcessor::Initialize(void)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->Initialize();
	
	return flag;
}

bool UpperLimbReqProcessor::InitJoint(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int)fData[i];
	} while (ivec[i] != -1);

	bool flag = hand->Initialize(ivec);

	return flag;
}

bool UpperLimbReqProcessor::isHandAvailable(void)
{
	size_t
		size_s = Spread.size(),
		size_f1 = Finger1.size(),
		size_f2 = Finger2.size(),
		size_f3 = Finger3.size();

	return ((size_s < 1) && (size_f1 < 1) && (size_f2 < 1) && (size_f3 < 1));
}

bool UpperLimbReqProcessor::OpenJoint(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int)fData[i];
	} while (ivec[i] != -1);

	bool flag = hand->OpenHand(ivec);

	return flag;
}

bool UpperLimbReqProcessor::Open(void)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->OpenHand();
	
	return flag;
}


bool UpperLimbReqProcessor::CloseJoint(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int)fData[i];
	} while (ivec[i] != -1);

	bool flag = hand->CloseHand(ivec);

	return flag;
}

bool UpperLimbReqProcessor::Close(void)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->CloseHand();
	
	return flag;
}

bool UpperLimbReqProcessor::OpenTorqueJoint(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int)fData[i];
	} while (ivec[i] != -1);

	bool flag = hand->TorqueOpen(ivec);

	return flag;
}

bool UpperLimbReqProcessor::OpenTorque(void)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->TorqueOpen();
	
	return flag;
}

bool UpperLimbReqProcessor::CloseTorqueJoint(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int)fData[i];
	} while (ivec[i] != -1);

	bool flag = hand->TorqueClose(ivec);

	return flag;
}

bool UpperLimbReqProcessor::CloseTorque(void)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->TorqueClose();
	
	return flag;
}

bool UpperLimbReqProcessor::SetOpenTarget(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i<4);

	bool flag = hand->setOpenTarget(vec);

	return flag;
}

bool UpperLimbReqProcessor::SetCloseTarget(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i<4);

	bool flag = hand->setCloseTarget(vec);

	return flag;
}

bool UpperLimbReqProcessor::GetPositions(std::vector<float> &fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	double dArray[4];
	bool flag = hand->getPositions(dArray);

	for (int i = 0; i <= 3; i++)
		fData[i] = (float)dArray[i];

	return flag;
}

bool UpperLimbReqProcessor::GetOpenTarget(std::vector<float> &fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	double dArray[4];

	bool flag = hand->getOpenTarget(dArray);

	for (int i = 0; i <= 3; i++)
		fData[i] = (float)dArray[i];

	return flag;
}

bool UpperLimbReqProcessor::GetCloseTarget(std::vector<float> &fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	double dArray[4];

	bool flag = hand->getCloseTarget(dArray);

	for (int i = 0; i <= 3; i++)
		fData[i] = (float)dArray[i];

	return flag;
}

bool UpperLimbReqProcessor::SetOpenTargetJoint(int joint, double value)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->setOpenTarget(joint, value);
	
	return flag;
}

bool UpperLimbReqProcessor::SetCloseTargetJoint(int joint, double value)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->setCloseTarget(joint, value);
	
	return flag;
}

float UpperLimbReqProcessor::GetOpenTargetJoint(int joint)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	double dAux = 0.0;
	bool flag = hand->getOpenTarget(joint, &dAux);
	
	return (float)dAux;
}

float UpperLimbReqProcessor::GetCloseTargetJoint(int joint)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	double dAux = 0.0;
	bool flag = hand->getCloseTarget(joint, &dAux);
	
	return (float)dAux;
}

bool UpperLimbReqProcessor::SetVelocity(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i < 4);

	bool flag = hand->setRefSpeeds(vec);

	return flag;
}

bool UpperLimbReqProcessor::GetVelocity(std::vector<float> &fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	double dArray[4];

	bool flag = hand->getRefSpeeds(dArray);

	for (int i = 0; i <= 3; i++)
		fData[i] = (float)dArray[i];

	return flag;
}

bool UpperLimbReqProcessor::SetOpenVelocity(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i<4);

	bool flag = hand->setRefSpeedsOpen(vec);

	return flag;
}

bool UpperLimbReqProcessor::SetCloseVelocity(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i<4);

	bool flag = hand->setRefSpeedsClose(vec);

	return flag;
}

bool UpperLimbReqProcessor::SetAcceleration(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i < 4);

	bool flag = hand->setRefAccelerations(vec);

	return flag;
}

bool UpperLimbReqProcessor::GetAcceleration(std::vector<float> &fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	double dArrayAux[4];

	bool flag = hand->getRefAccelerations(dArrayAux);

	for (int i = 0; i <= 3; i++)
		fData[i] = (float)dArrayAux[i];

	return flag;
}

bool UpperLimbReqProcessor::SetMaxStrain(std::vector<unsigned int> iData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		ivec[i] = (int)iData[i];
		i++;
	} while (i < 4);

	bool flag = hand->setMaxStrain(ivec);

	return flag;
}

bool UpperLimbReqProcessor::GetMaxStrain(std::vector<unsigned int> &iData)
{
	const int nFingers = 4;
	int strain[nFingers] = { 0, 0, 0, 0 };

	iData.assign(nFingers, 0);

	boost::unique_lock<boost::mutex> lck(mutex_Command);

	bool bSuccess = hand->getMaxStrain(strain);

	if (bSuccess)
	{
		for (int i = 0; i<nFingers; i++)
		{
			iData.at(i) = static_cast<unsigned int>(strain[i]);
		}
	}

	return bSuccess;
}

bool UpperLimbReqProcessor::GetStrainGauge(std::vector<unsigned int> &uParam)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	const int nFingers = 4;

	double strain[nFingers] = { 0.0, 0.0, 0.0, 0.0 };

	bool bSuccess = hand->getStrainGauges(strain);

	if (bSuccess)
	{
		uParam.assign(nFingers, 0);

		for (int i = 0; i<nFingers; i++)
			uParam.at(i) = static_cast<unsigned int>(strain[i]);
	}

	return bSuccess;
}

void UpperLimbReqProcessor::SetStrainGauge(bool ispicked1)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	isPicked = ispicked1;
	
}

bool UpperLimbReqProcessor::IncrementalMoveJoint(int joint, double value)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	bool flag = hand->relativeMove(joint, value);
	
	return flag;
}

bool UpperLimbReqProcessor::IncrementalMove(std::vector<float> fData)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	int i = 0;
	do
	{
		vec[i] = (double)fData[i];
		i++;
	} while (i<4);

	bool flag = hand->relativeMove(vec);

	return flag;
}

void UpperLimbReqProcessor::GetHandConfig(double dHandConfig[4])
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);

	for (int i = 0; i<4; i++)
		dHandConfig[i] = HandConfig[i];
}

void UpperLimbReqProcessor::GetSpreadConfig(double &dHandConfig)
{
	boost::unique_lock<boost::mutex> lck(mutex_Command);
	dHandConfig = HandConfig[0];
}


std::string UpperLimbReqProcessor::Report_Error_at_Command(unsigned int uCommand)
{
	std::string comm_str = "!!! Error in command '" + CommandToString(uCommand) + "' !!!";

	std::cerr << std::endl << comm_str << std::endl;

	return comm_str;
}

std::string UpperLimbReqProcessor::CommandToString(unsigned int uCommand)
{
	std::string command_str = "";

	switch (uCommand)
	{
	case BARRETTHAND_TERMINATE:
		command_str = "BARRETTHAND_TERMINATE";
		break;

	case BARRETTHAND_INITIALIZE:
		command_str = "BARRETTHAND_INITIALIZE";
		break;

	case BARRETTHAND_INITJOINT:
		command_str = "BARRETTHAND_INITJOINT";
		break;

	case BARRETTHAND_OPENJOINT:
		command_str = "BARRETTHAND_OPENJOINT";
		break;

	case BARRETTHAND_OPEN:
		command_str = "BARRETTHAND_OPEN";
		break;

	case BARRETTHAND_CLOSEJOINT:
		command_str = "BARRETTHAND_CLOSEJOINT";
		break;

	case BARRETTHAND_CLOSE:
		command_str = "BARRETTHAND_CLOSE";
		break;

	case BARRETTHAND_TORQUEOPEN_JOINT:
		command_str = "BARRETTHAND_TORQUEOPEN_JOINT";
		break;

	case BARRETTHAND_TORQUEOPEN:
		command_str = "BARRETTHAND_TORQUEOPEN";
		break;

	case BARRETTHAND_TORQUECLOSE_JOINT:
		command_str = "BARRETTHAND_TORQUECLOSE_JOINT";
		break;

	case BARRETTHAND_TORQUECLOSE:
		command_str = "BARRETTHAND_TORQUECLOSE";
		break;

	case BARRETTHAND_MOVEJOINT:
		command_str = "BARRETTHAND_MOVEJOINT";
		break;

	case BARRETTHAND_MOVEALL:
		command_str = "BARRETTHAND_MOVEALL";
		break;

	case BARRETTHAND_SETOPENTARGET:
		command_str = "BARRETTHAND_SETOPENTARGET";
		break;

	case BARRETTHAND_SETCLOSETARGET:
		command_str = "BARRETTHAND_SETCLOSETARGET";
		break;

	case BARRETTHAND_GETOPENTARGET:
		command_str = "BARRETTHAND_GETOPENTARGET";
		break;

	case BARRETTHAND_GETCLOSETARGET:
		command_str = "BARRETTHAND_GETCLOSETARGET";
		break;

	case BARRETTHAND_SETVELOCITY:
		command_str = "BARRETTHAND_SETVELOCITY";
		break;

	case BARRETTHAND_GETVELOCITY:
		command_str = "BARRETTHAND_GETVELOCITY";
		break;

	case BARRETTHAND_SETACCELERATION:
		command_str = "BARRETTHAND_SETACCELERATION";
		break;

	case BARRETTHAND_GETACCELERATION:
		command_str = "BARRETTHAND_GETACCELERATION";
		break;

	case BARRETTHAND_INCREMENTALMOVEJOINT:
		command_str = "BARRETTHAND_INCREMENTALMOVEJOINT";
		break;

	case BARRETTHAND_INCREMENTALMOVEALL:
		command_str = "BARRETTHAND_INCREMENTALMOVEALL";
		break;

	case BARRETTHAND_SETOPENTARGETJOINT:
		command_str = "BARRETTHAND_SETOPENTARGETJOINT";
		break;

	case BARRETTHAND_SETCLOSETARGETJOINT:
		command_str = "BARRETTHAND_SETCLOSETARGETJOINT";
		break;

	case BARRETTHAND_GETOPENTARGETJOINT:
		command_str = "BARRETTHAND_GETOPENTARGETJOINT";
		break;

	case BARRETTHAND_GETCLOSETARGETJOINT:
		command_str = "BARRETTHAND_GETCLOSETARGETJOINT";
		break;

	case BARRETTHAND_MOVEFINGERS:
		command_str = "BARRETTHAND_MOVEFINGERS";
		break;

	case BARRETTHAND_MOVESPREAD:
		command_str = "BARRETTHAND_MOVESPREAD";
		break;

	case BARRETTHAND_MOVETWOFINGERS:
		command_str = "BARRETTHAND_MOVETWOFINGERS";
		break;

	case BARRETTHAND_SETOPENVELOCITY:
		command_str = "BARRETTHAND_SETOPENVELOCITY";
		break;

	case BARRETTHAND_SETCLOSEVELOCITY:
		command_str = "BARRETTHAND_SETCLOSEVELOCITY";
		break;

	case BARRETTHAND_GETSTRAINGAUGE:
		command_str = "BARRETTHAND_GETSTRAINGAUGE";
		break;

	case BARRETTHAND_GETPOSITIONS:
		command_str = "BARRETTHAND_GETPOSITIONS";
		break;

	default:
		command_str = "Unknown";
		break;
	}

	return command_str;
}
