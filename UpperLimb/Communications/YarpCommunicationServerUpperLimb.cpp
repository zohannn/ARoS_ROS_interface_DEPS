#include "YarpCommunicationServerUpperLimb.h"
#include <commands.h>



#ifndef DEG_TO_RAD_F
#define DEG_TO_RAD_F 0.017453292519943295769236907684886f
#endif

#ifndef RAD_TO_DEG_F
#define RAD_TO_DEG_F 57.295779513082320876798154814105f
#endif


CYarpCommunicationServerUpperLimb::CYarpCommunicationServerUpperLimb(const std::string name, const std::string arm_config, const std::string hand_config,
										bool en_arm, bool en_hand, bool ini_hand):
	CYarpCommunication(name, "", true ),
	arm_nr_joints(7),
	hand_nr_joints(4),
	nr_joints(arm_nr_joints + hand_nr_joints)
{
	enable_arm = en_arm;
	enable_hand = en_hand;
	init_hand = ini_hand;
	stop_executor = false;
	stop_movement = false;
	executing_movement = false;
	stop_vel_executor = false;
	stop_vel_movement = false;
	executing_vel_movement = false;
	arm_config_file = arm_config;
	hand_config_file = hand_config;

	traj_executor_thrd = boost::thread(boost::bind(&CYarpCommunicationServerUpperLimb::TrajectoryMovementsExecutor, this));

	vel_traj_executor_thrd = boost::thread(boost::bind(&CYarpCommunicationServerUpperLimb::VelTrajectoryMovementsExecutor, this));

	//change thread priority
	//HANDLE threadHandle = traj_executor_thrd.native_handle();
	HANDLE threadHandle = vel_traj_executor_thrd.native_handle();

	BOOL suc = SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
	if (!suc)
	{
		std::cout << "Error: Failed to set priority to trajectory executor" << std::endl;
	}

	hand_machine_state = HandMachineState::Moving;

}


CYarpCommunicationServerUpperLimb::~CYarpCommunicationServerUpperLimb()
{
	stop_executor = true;
	stop_movement = true;
	stop_vel_executor = true;
	stop_vel_movement = true;
	if (traj_executor_thrd.joinable())
		traj_executor_thrd.join();

	if (vel_traj_executor_thrd.joinable())
		vel_traj_executor_thrd.join();
}

void CYarpCommunicationServerUpperLimb::init_logging()
{
	boost::log::add_common_attributes();

	boost::log::add_file_log
	(
		boost::log::keywords::file_name = "upperlimb_%N.log", // file name pattern
		boost::log::keywords::rotation_size = 10 * 1024 * 1024, // rotation file every 10 MB ... 
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,0,0), // ... or at midnight
		boost::log::keywords::format = "[%TimeStamp%]: %Message%", // log record format
		boost::log::keywords::target = "Boost_logs"
	);

	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= boost::log::trivial::info
	);

}

bool CYarpCommunicationServerUpperLimb::Init( void )
{
	if (enable_arm)
	{
		//PStepserver instance
		std::cout << "Initializing PStepServer instance... " << std::flush;
		if (!PStepServer.Initialize())
			return false;
		std::cout << "OK." << std::endl;
	}

	if (enable_arm)
		arm = boost::make_shared<yarp::dev::AmtecLWA7dof>();
	if (enable_hand)
		hand = boost::make_shared<yarp::dev::BarrettHand826X>();

	bool success;
	yarp::os::Property arm_prop, hand_prop;
	if (enable_arm)
	{
		std::cout << "Loading arm config file... " << std::flush;
		//check configuration files for arm and hand
		success = arm_prop.fromConfigFile(arm_config_file);
		if (!success)
		{
			std::cout << "Error: Could not read from " << arm_config_file << std::endl;
			return false;
		}
		std::cout << "OK." << std::endl;

	}
	else
		std::cout << "Arm module is disabled." << std::endl;

	if (enable_hand)
	{
		std::cout << "Loading hand config file... " << std::flush;
		success = hand_prop.fromConfigFile(hand_config_file);
		if (!success)
		{
			std::cout << "Error: Could not read from " << hand_config_file << std::endl;
			return false;
		}
		std::cout << "OK." << std::endl;
	}
	else
		std::cout << "Hand module is disabled." << std::endl;

	//open devices
	if (enable_arm)
	{
		//arm must have sync_motion enabled
		arm_prop.unput("use_sync_motion");
		arm_prop.put("use_sync_motion", yarp::os::Value::makeValue("true"));

		std::cout << "Opening arm device... " << std::flush;
		success = arm->open(arm_prop);
		if (!success)
		{
			std::cout << "Error: Could not open ARM device: " << arm->Get_Error_Str() << std::endl;
			return 5;
		}
		std::cout << "OK." << std::endl;

	}

	if (enable_hand)
	{
		//overwrite config file option of initialization of hand
		hand_prop.unput("initialize");
		hand_prop.put("initialize", yarp::os::Value::makeInt(init_hand? 1 : 0));

		std::cout << "Opening hand device... " << std::flush;
		success = hand->open(hand_prop);
		if (!success)
		{
			std::cout << "Error: Could not open HAND device." << std::endl;
			return 6;
		}
		std::cout << "OK." << std::endl;
		

		//Prepare the specific commands hand processor
		PrepareHandProcessor();
	}

	init_logging();
}

void CYarpCommunicationServerUpperLimb::Fini( void )
{
	stop_executor = true;
	stop_movement = true;

	stop_vel_executor = true;
	stop_vel_movement = true;

	if( enable_arm )
	{
		if(arm->close())
			PStepServer.Terminate();
		else
			std::cerr << "Error closing The ARM Device Driver!" << std::endl;
	}
	if( enable_hand )
	{
		if(!hand->close())
			std::cerr << "Error closing The HAND Device Driver!" << std::endl;
	}

}

boost::shared_ptr<yarp::dev::AmtecLWA7dof> CYarpCommunicationServerUpperLimb::getArmDevice()
{
	return arm;
}

boost::shared_ptr<yarp::dev::BarrettHand826X> CYarpCommunicationServerUpperLimb::getHandDevice()
{
	return hand;
}

void CYarpCommunicationServerUpperLimb::ResetExecutor()
{
	executor_state = ExecutorState::Ready;
}

CYarpCommunicationServerUpperLimb::ExecutorState CYarpCommunicationServerUpperLimb::GetExecutorState()
{
	return executor_state;
}

void CYarpCommunicationServerUpperLimb::SetDevices(boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm_, boost::shared_ptr<yarp::dev::BarrettHand826X> hand_)
{
	arm = arm_;
	hand = hand_;
}

bool CYarpCommunicationServerUpperLimb::start_joints_states_sender(std::string name)
{
	joint_states_out_name = name;

	joint_states_out_worker_end_job = false;
	joint_states_out_channel.resume();
	bool works = joint_states_out_channel.open(joint_states_out_name);
	//BOOST_LOG_SEV(lg, info) << "Comm channel '" << joint_states_out_name <<
	std::cout << "Comm channel '" << joint_states_out_name <<
		(works ? "' open with success" : " failed to open: ") << std::endl;
	if (!works)
		return false;

	joint_states_out_worker = boost::thread(boost::bind(&CYarpCommunicationServerUpperLimb::joint_states_job, this));

	return true;
}

void CYarpCommunicationServerUpperLimb::stop_joints_states_sender()
{
	joint_states_out_worker_end_job = true;
	joint_states_out_channel.interrupt();
	joint_states_out_channel.close();
//	joint_states_queue_cv.notify_all();
	if (joint_states_out_worker.joinable())
		joint_states_out_worker.join();
}

void CYarpCommunicationServerUpperLimb::joint_states_job()
{
	float time_step = 10; // ms
	while (!joint_states_out_worker_end_job)
	{

		//if(!executing_vel_movement){
			bool modules_ok; bool valued = false;
			Joint_States joints_values;
			joints_values.position.resize(nr_joints);
			joints_values.velocity.resize(nr_joints);

			if(!executing_movement && !executing_vel_movement){
				// lock the mutex variables
				boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); 
				if(!hand->rt_mode_on){
					modules_ok = hand->startRTmode();
					boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				}
				double dArray[4]; //std::vector<float> hand_pos(4,1);
				if(hand->rt_mode_on){modules_ok = hand->getRTPositions(dArray);}
				if (!modules_ok)
					throw std::string("Failed to get the hand current positions!");

				// positions
				modules_ok = arm->getPosAll(joints_values.position.data());
				if (!modules_ok)
						throw std::string("Failed to get the arm current positions!");


				//modules_ok = hand->startRTmode(); 
				//std::vector<double> hand_v(4,-7*DEG_TO_RAD_F); std::vector<double> hand_v_0(4,0);
				//double hand_v_r[4];
							

				/*
				float vel_0; arm->getMaxVelLimit(0,&vel_0);
				float vel_1; arm->getMaxVelLimit(1,&vel_1);
				float vel_2; arm->getMaxVelLimit(2,&vel_2);
				float vel_3; arm->getMaxVelLimit(3,&vel_3);
				float vel_4; arm->getMaxVelLimit(4,&vel_4);
				float vel_5; arm->getMaxVelLimit(5,&vel_5);
				float vel_6; arm->getMaxVelLimit(6,&vel_6);

				float def_vel_0; arm->getMaxDefVelLimit(0,&def_vel_0);
				float def_vel_1; arm->getMaxDefVelLimit(1,&def_vel_1);
				float def_vel_2; arm->getMaxDefVelLimit(2,&def_vel_2);
				float def_vel_3; arm->getMaxDefVelLimit(3,&def_vel_3);
				float def_vel_4; arm->getMaxDefVelLimit(4,&def_vel_4);
				float def_vel_5; arm->getMaxDefVelLimit(5,&def_vel_5);
				float def_vel_6; arm->getMaxDefVelLimit(6,&def_vel_6);

				float acc_0; arm->getMaxAccLimit(0,&acc_0);
				float acc_1; arm->getMaxAccLimit(1,&acc_1);
				float acc_2; arm->getMaxAccLimit(2,&acc_2);
				float acc_3; arm->getMaxAccLimit(3,&acc_3);
				float acc_4; arm->getMaxAccLimit(4,&acc_4);
				float acc_5; arm->getMaxAccLimit(5,&acc_5);
				float acc_6; arm->getMaxAccLimit(6,&acc_6);

				float def_acc_0; arm->getMaxDefAccLimit(0,&def_acc_0);
				float def_acc_1; arm->getMaxDefAccLimit(1,&def_acc_1);
				float def_acc_2; arm->getMaxDefAccLimit(2,&def_acc_2);
				float def_acc_3; arm->getMaxDefAccLimit(3,&def_acc_3);
				float def_acc_4; arm->getMaxDefAccLimit(4,&def_acc_4);
				float def_acc_5; arm->getMaxDefAccLimit(5,&def_acc_5);
				float def_acc_6; arm->getMaxDefAccLimit(6,&def_acc_6);
				*/

				//hand->setRTVelocities(hand_v);			
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
				//hand->getRTVelocities(hand_v_r);
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
				//hand->setRTVelocities(hand_v_0);

				//double dArray_1[4];
				//if(hand->rt_mode_on){modules_ok = hand->getRTPositions(dArray_1);}

				//modules_ok = hand->stopRTmode();
			


				////move spread to end of array
				//std::rotate(dArray, dArray + 1, dArray + 4);
				//for (int i = 0; i < 4; i++)
						//hand_pos.at(i) = static_cast<float>(dArray[i]) * DEG_TO_RAD_F;

				std::vector<float> hand_pos(dArray,dArray+4);
				std::transform(hand_pos.begin(),hand_pos.end(),hand_pos.begin(),std::bind1st(std::multiplies<float>(),DEG_TO_RAD_F));					
				std::copy(hand_pos.begin(), hand_pos.end(), joints_values.position.begin() + arm_nr_joints);

				// velocities
				modules_ok = arm->getVelAll(joints_values.velocity.data());
				if (!modules_ok)
						throw std::string("Failed to get the arm current velocities!");

				double dArrayVel[4]; //std::vector<float> hand_vel(4,1);
				//modules_ok = hand->startRTmode();
				if(hand->rt_mode_on){modules_ok = hand->getRTVelocities(dArrayVel);}

				//modules_ok = hand->stopRTmode();

				if (!modules_ok)
					throw std::string("Failed to get the hand current velocities!");
				////move spread to end of array
				//std::rotate(dArrayVel, dArrayVel + 1, dArrayVel + 4);
				//for (int i = 0; i < 4; i++)
					//hand_vel.at(i) = static_cast<float>(dArrayVel[i]) * DEG_TO_RAD_F;
				std::vector<float> hand_vel(dArrayVel,dArrayVel+4);
				std::transform(hand_vel.begin(),hand_vel.end(),hand_vel.begin(),std::bind1st(std::multiplies<float>(),DEG_TO_RAD_F));		
				std::copy(hand_vel.begin(), hand_vel.end(), joints_values.velocity.begin() + arm_nr_joints);

				// handle the asynchronous writings of the serial port 
				hand->ioService.run();
				//hand->stopRTmode();

				valued = true;


			}else{
				boost::unique_lock<boost::mutex> lck(joint_states_queue_mtx);
				//joint_states_queue_cv.wait(lck, [&]() { return joint_states_out_worker_end_job || !joint_states_queue.empty(); });
				//if (joint_states_out_worker_end_job)
					//continue;

				if(!joint_states_queue.empty() && !joint_states_out_worker_end_job)
				{
					//dequeue a info
					joints_values = std::move(joint_states_queue.front());
					joint_states_queue.pop();
					valued = true;
				}else{
					valued = false;
				}
			}
			
			if (joint_states_out_channel.getOutputCount() != 0 && valued)
			{
				Joint_States &nf = joint_states_out_channel.prepare();
				nf = boost::move(joints_values);
				//this info might be dropped (in write) in case there is an ongoing write operation
				joint_states_out_channel.write();
			}
		//} // if NOT executing vel movement
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_step));
		
	}// while loop
	
	boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); 
	hand->stopRTmode();
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	//BOOST_LOG_SEV(lg, boost::log::trivial::info) << "Ended joint_states thread.";
	//std::cout << "joint_states thread has finished." << std::endl;
}



void CYarpCommunicationServerUpperLimb::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	int nAux;
	//flag indicating if command was processed by one of the specific commands processor (arm or hand)
	bool processed = false;

	try
	{
		switch (msgIn.uCommand)
		{
		case UpperLimb::ADD_TRAJECTORY:
			AddTrajectory(msgIn.uParam, msgIn.fData);
			break;
		case UpperLimb::ADD_VEL_TRAJECTORY:
			AddVelTrajectory(msgIn.uParam, msgIn.fData);
			break;
		case UpperLimb::STOP_CLEAR_TRAJECTORY:
			StopClearTrajectory();
			break;
		case UpperLimb::WAIT_TRAJECTORY_END:
			if (msgIn.uParam.size() > 0)
			{
				// Perform wait based in time
				nAux = WaitTrajectoryEnd(msgIn.uParam.front());
			}else{
				// Perform timeless wait
				nAux = WaitTrajectoryEnd(-1);
			}
			msgOut.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>(nAux)
				);
			break;
		case UpperLimb::WAIT_VEL_TRAJECTORY_END:
			if (msgIn.uParam.size() > 0)
			{
				// Perform wait based in time
				nAux = WaitVelTrajectoryEnd(msgIn.uParam.front());
			}else{
				// Perform timeless wait
				nAux = WaitVelTrajectoryEnd(-1);
			}
			msgOut.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>(nAux)
				);
			break;
		case UpperLimb::WAIT_TRAJECTORY_END_ASYNC:
			boost::async(boost::launch::async,[&]()
			{
				int ret;
				//wait for trajectory end
				ret = WaitTrajectoryEnd(-1);

				CMessage msg;
				// Assign one value to uParam, the return value of WaitMotionEnd
				msg.uParam.push_back(static_cast<unsigned int>(ret));
				msg.uCommand = UpperLimb::TRAJECTORY_ASYNC_FINISHED;
				bool sent_fin = Send(msg, false);
				if (!sent_fin)
					std::cout << "Failed to send an ASYNC_FINISHED message: " << get_ErrorString();
			});
			break;
		case UpperLimb::WAIT_VEL_TRAJECTORY_END_ASYNC:
			boost::async(boost::launch::async,[&]()
			{
				int ret;
				//wait for trajectory end
				ret = WaitVelTrajectoryEnd(-1);

				CMessage msg;
				// Assign one value to uParam, the return value of WaitMotionEnd
				msg.uParam.push_back(static_cast<unsigned int>(ret));
				msg.uCommand = UpperLimb::VEL_TRAJECTORY_ASYNC_FINISHED;
				bool sent_fin = Send(msg, false);
				if (!sent_fin)
					std::cout << "Failed to send an ASYNC_FINISHED message: " << get_ErrorString();
			});
			break;
		case UpperLimb::CONFIGURE_LIMBS:
			if (msgIn.uParam.size() != 2)
				throw std::string("Invalid configuration!");

			soft_enabled_arm = msgIn.uParam[0] == 0 ? false : true;
			soft_enabled_hand = msgIn.uParam[1] == 0 ? false : true;
			break;
		case UpperLimb::GET_POSITIONS:
		{
			msgOut.fData.resize(nr_joints);
			//boost::unique_lock<boost::mutex> lck_arm(mutex_amtec);
			bool modules_ok = arm->getPosAll(msgOut.fData.data());
			if (!modules_ok)
				throw std::string("Failed to get the arm current positions!");

			boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); hand->startRTmode();
			double dArray[4];
			modules_ok = hand->getRTPositions(dArray);
			if (!modules_ok)
				throw std::string("Failed to get the hand current positions!");

			//hand->stopRTmode();
			//move spread to end of array
			//std::rotate(dArray, dArray + 1, dArray + 4);
			for (int i = 0; i <= 3; i++)
				//msgOut.fData[i + arm_nr_joints] = static_cast<float>(dArray[i]) * 3.14159265f /180.0f;
				msgOut.fData[i + arm_nr_joints] = static_cast<float>(dArray[i]) * DEG_TO_RAD_F;
		}
			break;
		case UpperLimb::GET_VELOCITIES:
		{
			msgOut.fData.resize(nr_joints);
			//boost::unique_lock<boost::mutex> lck_arm(mutex_amtec);
			bool modules_ok = arm->getVelAll(msgOut.fData.data());
			if (!modules_ok)
				throw std::string("Failed to get the arm current velocities!");

			boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); hand->startRTmode();
			double dArray[4];
			modules_ok = hand->getRTVelocities(dArray);

			//move spread to end of array
			//std::rotate(dArray, dArray + 1, dArray + 4);

			//hand->stopRTmode();

			for (int i = 0; i <= 3; i++)
				//msgOut.fData[i + arm_nr_joints] = static_cast<float>(dArray[i]) * 3.14159265f /180.0f;
				msgOut.fData[i + arm_nr_joints] = static_cast<float>(dArray[i]) * DEG_TO_RAD_F;

			if (!modules_ok)
				throw std::string("Failed to get the hand current velocities!");
		}
			break;

		case UpperLimb::SET_POSITIONS:
			if (msgIn.fData.size() < nr_joints + 1)
			{
				msgOut.iErrorCode = -1;
				msgOut.strText = "Corrupted data";
			}
			else
			{			
				MovePos( msgIn.fData );
			}		
			break;

		case UpperLimb::SET_VELOCITIES:

			if( msgIn.fData.size() < nr_joints )
			{
				std::stringstream joint_text; joint_text << nr_joints;
				msgOut.iErrorCode = -1;
				msgOut.strText = "Move velocity: Array size to small, it must have " + joint_text.str() + "elements.";
			}
			else
			{
				boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); 
				if(!hand->rt_mode_on){hand->startRTmode();}
				MoveVel( msgIn.fData );
			}
			break;

		case UpperLimb::GRASP_OBJECT:
			{
				boost::unique_lock<boost::mutex> lck(mutex_barrett);
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
				hand->stopRTmode();
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
				hand_machine_state = HandMachineState::Grasping;
				if (!CloseTorque())
					throw Report_Error_at_Command(msgIn.uCommand);
			}
			break;

		case UpperLimb::RELEASE_OBJECT:
			{
				boost::unique_lock<boost::mutex> lck(mutex_barrett); 
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
				hand->stopRTmode();
				//boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
				hand_machine_state = HandMachineState::Releasing;
				if (!Open())
					throw Report_Error_at_Command(msgIn.uCommand);
			}
			break;
			//--------------------------------------------------------------------------
		default:
			//it is a specific command...
			processed = ArmProcessor(msgIn, msgOut);
			if (processed)
				break;
			processed = HandProcessor(msgIn, msgOut);
			if (!processed)
			{
				msgOut.strText = "unknown command";
				msgOut.iErrorCode = 1;
			}
			break;
		}
	}
	catch (std::string &ex)
	{
		msgOut.iErrorCode = 2;
		msgOut.strText = ex;
	}
}

void CYarpCommunicationServerUpperLimb::AddTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData)
{
	//Expected protocol:
	//uParam[0] -> Nr trajectory steps
	//uParam[1] -> flag indicating if arm limb should run
	//uParam[2] -> flag indicating if hand limb should run
	//fData[0->NrJoints-1] -> joints values for first step
	//fData[NrJoints] -> time value for first step
	//fData[NrJoints+1->2*NrJoints] -> joints values for second step
	// and so forth...

	//prepare the trajectory
	if (uParam.size() != 3)
		throw std::string("The uParam vector size must be 3");

	size_t nr_steps = static_cast<size_t>(uParam[0]);

	bool run_arm = uParam[1] == 0 ? false : true;
	bool run_hand = uParam[2] == 0 ? false : true;

	size_t fData_expected_size = (nr_joints + 1)*nr_steps;
	if (fData.size() != fData_expected_size)
		throw std::string("Metadata and data does not match: fData size is different from the uParam info.");

	TrajectoryMovement trajectory;

	//set the devices flags
	std::get<1>(trajectory) = run_arm;
	std::get<2>(trajectory) = run_hand;

	//store each step
	for (size_t idx = 0; idx < nr_steps; idx++)
	{
		size_t start_idx = idx*(nr_joints + 1);
		TrajectoryStep step;

		//copy the joints values
		std::copy(fData.begin() + start_idx, fData.begin() + start_idx + nr_joints, std::back_inserter(std::get<0>(step)));

		//copy movement time
		std::get<1>(step) = *(fData.begin() + start_idx + nr_joints);
		std::get<0>(trajectory).emplace_back(std::move(step));
	}
	//enqueue the trajectory for executor
	{
		boost::unique_lock<boost::mutex> lck(queue_mtx);
		trajectory_queue.push(boost::move(trajectory));
	}
	//signal that a new trajectory is available for process
	queue_not_empty_cv.notify_one();
}

void CYarpCommunicationServerUpperLimb::AddVelTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData)
{
	//Expected protocol:
	//uParam[0] -> Nr trajectory steps
	//uParam[1] -> flag indicating if arm limb should run
	//uParam[2] -> flag indicating if hand limb should run
	//fData[0->NrJoints-1] -> joints values for first step
	//fData[NrJoints] -> time value for first step
	//fData[NrJoints+1->2*NrJoints] -> joints values for second step
	// and so forth...

	//prepare the trajectory
	if (uParam.size() != 3)
		throw std::string("The uParam vector size must be 3");

	size_t nr_steps = static_cast<size_t>(uParam[0]);

	bool run_arm = uParam[1] == 0 ? false : true;
	bool run_hand = uParam[2] == 0 ? false : true;

	size_t fData_expected_size = (nr_joints + 1)*nr_steps;
	if (fData.size() != fData_expected_size)
		throw std::string("Metadata and data does not match: fData size is different from the uParam info.");

	VelTrajectoryMovement vel_traj;

	//set the devices flags
	std::get<1>(vel_traj) = run_arm;
	std::get<2>(vel_traj) = run_hand;

	//store each step
	for (size_t idx = 0; idx < nr_steps; ++idx)
	{

		size_t start_idx = idx*(nr_joints + 1);
		VelTrajectoryStep vel_step;

		//copy the joints values
		std::copy(fData.begin() + start_idx, fData.begin() + start_idx + nr_joints, std::back_inserter(std::get<0>(vel_step)));

		//copy movement time
		std::get<1>(vel_step) = *(fData.begin() + start_idx + nr_joints);
		std::get<0>(vel_traj).emplace_back(std::move(vel_step));

	}

	//enqueue the trajectory for the velocity executor
	{
		boost::unique_lock<boost::mutex> lck(vel_queue_mtx);
		vel_trajectory_queue.push_back(boost::move(vel_traj));
	}
	//signal that a new trajectory is available for process
	vel_queue_not_empty_cv.notify_one();

}

void CYarpCommunicationServerUpperLimb::StopClearTrajectory()
{
	boost::unique_lock<boost::mutex> lck(queue_mtx);
	boost::unique_lock<boost::mutex> vel_lck(vel_queue_mtx);

	if (!executing_movement && trajectory_queue.empty() && vel_trajectory_queue.empty())
	{
		std::cout << "Received a request to stop trajectory, but no trajectory was under execution and queue is empty" << std::endl;
		return;
	}
	stop_movement = true;
	stop_vel_movement = true;
}

void CYarpCommunicationServerUpperLimb::MoveVel( std::vector<float>& data )
{
	//size_t vSize = fData.size();
	std::vector<double> arm_joints(arm_nr_joints);
	std::copy(data.begin(), data.begin() + arm_nr_joints, arm_joints.begin());
	std::vector<double> hand_joints(hand_nr_joints);
	std::copy(data.begin() + arm_nr_joints, data.begin() + arm_nr_joints + hand_nr_joints, hand_joints.begin());

	//boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); 
	//if(!hand->rt_mode_on){hand->startRTmode();}
	//double *arm_joints = new double[vSize];

	//for( size_t i = 0; i < arm_nr_joints; i++ )
		//arm_joints[i] = fData.at( i );

	//boost::unique_lock<boost::mutex> lck_arm(mutex_amtec);

	//std::vector<float> curr_vel(arm_nr_joints);
	//bool arm_cmd_ok = arm->getVelAll(&curr_vel[0]);
	//float sum = 0;
	//for(int i=0;i<arm_nr_joints;++i)
		//sum += pow(data.at(i)-curr_vel.at(i),2);
	//float thr = 0.5*DEG_TO_RAD_F;
	//if(sqrt(sum)>thr)
	//{
	//bool arm_cmd_ok = arm->velocityMove(&arm_joints[0]);
	//bool hand_cmd_ok = hand->setRTVelocities(hand_joints);
	if (!arm->velocityMove(&arm_joints[0]) || !hand->setRTVelocities(hand_joints))
	//if (!arm->velocityMove(&arm_joints[0]))
	//if(!hand->setRTVelocities(hand_joints))
		throw std::string("Failed to execute the arm/hand motion in velocity mode!");
	//}
	//delete[] joints;
	//joints = nullptr;
	//boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); hand->startRTmode();
	//bool hand_cmd_ok = hand->setRTVelocities(hand_joints);
	//if (!hand_cmd_ok)
		//throw std::string("Failed to execute the hand motion in velocity mode!");
}


void CYarpCommunicationServerUpperLimb::MovePos( std::vector<float> data )
{
	//size_t vSize = fData.size();
	std::vector<float> arm_joints(arm_nr_joints);
	std::copy(data.begin(), data.begin() + arm_nr_joints, arm_joints.begin());
	std::vector<double> hand_joints(hand_nr_joints);
	std::copy(data.begin() + arm_nr_joints, data.begin() + arm_nr_joints + hand_nr_joints, hand_joints.begin());
	unsigned short time_step = data.at(nr_joints);
	//Send joints values to arm
	//boost::unique_lock<boost::mutex> lck_arm(mutex_amtec);
	bool arm_cmd_ok = arm->positionMoveSync(arm_joints.data(), time_step, false);
	if (!arm_cmd_ok)
		throw std::string("Failed to execute the synchronized arm motion!");

	// Send joints values to hand
	//convert to degrees
	std::transform(hand_joints.begin(), hand_joints.end() , hand_joints.begin(), std::bind(std::multiplies<double>(), std::placeholders::_1, RAD_TO_DEG_F));
	//move spread to begin of array
	//std::rotate(hand_joints.begin(), hand_joints.end() - 1, hand_joints.end());

	boost::unique_lock<boost::mutex> lck_hand(mutex_barrett);
	bool hand_cmd_ok;
	if (abs(hand_joints[1] - hand_joints[2]) < 0.01
		&& abs(hand_joints[1] - hand_joints[3]) < 0.01)
		hand_cmd_ok = hand->positionMoveFingers(hand_joints[1]);

	else if (abs(hand_joints[1] - hand_joints[2]) < 0.01)
		hand_cmd_ok = hand->positionMoveTwoFingers((double*)(hand_joints.data()));
	else
		hand_cmd_ok = hand->positionMoveFingers((double*)(hand_joints.data()));

	if (!hand_cmd_ok)
	{
		throw std::string("Failed to move Fingers.");
	}
	//move spread
	hand_cmd_ok = hand->positionMove(0, hand_joints[0]);

	if (!hand_cmd_ok)
		throw std::string("Failed to move Spread.");
}

int CYarpCommunicationServerUpperLimb::WaitTrajectoryEnd(int timeout_ms)
{
	//ERROR CODES:
	enum
	{
		FINISHED = 0,
		EXECUTING = 1,
		TIMEOUT = 2,
		FAILED = 3,
	};
	
	//acquire the movements queue lock
	boost::unique_lock<boost::mutex> lck(executing_movement_mtx);

	//wait for executor to signal that the movement has finished

	//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
	//result is the same as predicate: !executing_movement;
	if (timeout_ms == 0)
	{ //do not block waiting.....
		return executing_movement ? EXECUTING : executing_movement_error ? FAILED : FINISHED;
	}
	else if (timeout_ms < 0)
	{	//wait injdefinitely
		executing_movement_cv.wait(lck, [&](){ return !executing_movement; });
		return executing_movement_error ? FAILED : FINISHED;
	}
	else
	{
		//block execution until a signal is received or timeout occurs
		bool result = executing_movement_cv.wait_for(lck, boost::chrono::milliseconds(timeout_ms),
			[&](){ return !executing_movement; });
		if (!result)	//timeout
			return TIMEOUT;
		return executing_movement_error ? FAILED : FINISHED;
	}
	return 0;
}

int CYarpCommunicationServerUpperLimb::WaitVelTrajectoryEnd(int timeout_ms)
{
	//ERROR CODES:
	enum
	{
		FINISHED = 0,
		EXECUTING = 1,
		TIMEOUT = 2,
		FAILED = 3,
	};
	
	//acquire the movements queue lock
	boost::unique_lock<boost::mutex> lck(executing_vel_movement_mtx);

	//wait for executor to signal that the movement has finished

	//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
	//result is the same as predicate: !executing_movement;
	if (timeout_ms == 0)
	{ //do not block waiting.....
		return executing_vel_movement ? EXECUTING : executing_vel_movement_error ? FAILED : FINISHED;
	}
	else if (timeout_ms < 0)
	{	//wait injdefinitely
		executing_vel_movement_cv.wait(lck, [&](){ return !executing_vel_movement; });
		return executing_vel_movement_error ? FAILED : FINISHED;
	}
	else
	{
		//block execution until a signal is received or timeout occurs
		bool result = executing_vel_movement_cv.wait_for(lck, boost::chrono::milliseconds(timeout_ms),
			[&](){ return !executing_vel_movement; });
		if (!result)	//timeout
			return TIMEOUT;
		return executing_vel_movement_error ? FAILED : FINISHED;
	}
	return 0;
}

void CYarpCommunicationServerUpperLimb::TrajectoryMovementsExecutor()
{
	//The executor has two possible states:
	//
	//  Ready -> waiting for trajectories os executing one.
	//
	//  Error -> failed to execute a step or some joint failed
	//
	// It starts from cold in Ready mode.
	//	On error, Reset must be invoked, otherwise no more trajectories are processed
	//	If an error occurs while executing a movement, the remaining of the trajectory is discared.

	//		Start						End
	//		------->*****************------->
	//				*	Ready		*
	//		------->*				*--------
	//		|		*****************		|
	//		|								|  Error on execution
	//Reset |		*****************	    |
	//		|		*	Error		*       |
	//		|------	*				*<-------
	//				*****************--------->End

	using boost::chrono::milliseconds;
	using boost::chrono::high_resolution_clock;

	int test_Joint = 0;

	high_resolution_clock::time_point start, wake_up_time, last_hand_update;

	while (!stop_executor)
	{
		try
		{
			switch (executor_state)
			{
			case ExecutorState::Ready:
				break;
			case ExecutorState::Error:
			{
				//sleep for some time waiting for either error is cleared or stop is requested
				boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
				continue;
			}
				break;
			default:
				break;
			}
			TrajectoryMovement mov;
			{
				//acquire the movements queue lock
				boost::unique_lock<boost::mutex> lck(queue_mtx);

				if (stop_movement)
				{
					//flush the queue
					while (!trajectory_queue.empty())
						trajectory_queue.pop();

					std::cout << "Upperlimb trajectory executor: Current trajectory was stopped and queue flushed!" << std::endl;

					//reset flag
					stop_movement = false;
				}

				//wait for someome signal that we have a new movement to perform

				//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
				//result is the same as predicate: !trajectory_queue.empty();
				bool result = queue_not_empty_cv.wait_for(lck, milliseconds(500), [&](){ return !trajectory_queue.empty(); });

				if (!result)	//timeout
					continue;
				//signal that we are performing a movement
				executing_movement_error = false;
				executing_movement = true;

				//dequeue a movement
				mov = boost::move(trajectory_queue.front());
				trajectory_queue.pop();
			}
			boost::shared_future<void> hand_handle;
			//should arm and hand mov?
			bool run_arm = std::get<1>(mov);
			bool run_hand = std::get<2>(mov);

			boost::unique_lock<boost::mutex> lck_hand(mutex_barrett); 
			hand->stopRTmode();

			//send values to hand only each 5 steps, otherwise the hand does not move. 
			size_t step_counter = 0;
			size_t nr_steps = std::get<0>(mov).size();
			last_hand_update = high_resolution_clock::now() - boost::chrono::milliseconds(1000);


			//process each trajectory step
			for each(auto &step in std::get<0>(mov))
			{
				if (stop_movement)	//stop executing if stop_movement goes to true
					break;

				const std::vector<float> &joints = std::get<0>(step);
				unsigned time = static_cast<unsigned>(std::get<1>(step)*1000.0f);

				//compute the time point when should the next step be processed
				start = high_resolution_clock::now();
				float time_thresshold = 25;
				unsigned int uptowake = time <= time_thresshold ? time : time - time_thresshold;
				wake_up_time = start + boost::chrono::milliseconds(uptowake);

				//Send joints values to arm
				if (run_arm && soft_enabled_arm && arm)
				{
					//boost::unique_lock<boost::mutex> lck(mutex_amtec);
					bool arm_cmd_ok = arm->positionMoveSync(joints.data(), time, false);
					if (!arm_cmd_ok)
						throw std::string("Failed to execute the synchronized arm motion!");
				}

				//Send joints values to the hand
				//joints += arm_nr_joints;
				if (run_hand && soft_enabled_hand && hand && hand_machine_state != HandMachineState::Grasping)
				{
					//copy hand joints values because vector might get out of scope before
					//hand async lambda gets a chance to run.
					std::vector<double> hand_joints(hand_nr_joints);
					std::copy(joints.begin() + arm_nr_joints, joints.end(), hand_joints.begin());

					hand_machine_state = HandMachineState::Moving;

					boost::function<void(std::vector<double>, boost::shared_ptr<yarp::dev::BarrettHand826X>, boost::shared_future<void>)> move_hand =
						[](std::vector<double> hand_joints, boost::shared_ptr<yarp::dev::BarrettHand826X> hand, boost::shared_future<void> previous_hand_handle)
					{
						if(previous_hand_handle.valid())
							previous_hand_handle.wait();

						//convert to degrees
						std::transform(hand_joints.begin(), hand_joints.end() , hand_joints.begin(), std::bind(std::multiplies<double>(), std::placeholders::_1, 180.0/ 3.14159265));
						//move spread to begin of array
						//std::rotate(hand_joints.begin(), hand_joints.end() - 1, hand_joints.end());

						bool hand_cmd_ok;
						if (abs(hand_joints[1] - hand_joints[2]) < 0.01
							&& abs(hand_joints[1] - hand_joints[3]) < 0.01)
							hand_cmd_ok = hand->positionMoveFingers(hand_joints[1]);

						else if (abs(hand_joints[1] - hand_joints[2]) < 0.01)
							hand_cmd_ok = hand->positionMoveTwoFingers(const_cast<double*>(hand_joints.data()));
						else
							hand_cmd_ok = hand->positionMoveFingers(hand_joints.data());

						if (!hand_cmd_ok)
						{
							throw std::string("Failed to move Fingers.");
						}
						//move spread
						hand_cmd_ok = hand->positionMove(0, hand_joints[0]);

						if (!hand_cmd_ok)
							throw std::string("Failed to move Spread.");
					};

					//check wether previous hand movement has error
					if (hand_handle.valid() && hand_handle.has_exception())
						hand_handle.get();//this will let the exception, if thrown in async method, to pass and be catch in below catch statement

					if(start > last_hand_update + boost::chrono::milliseconds(1000) || step_counter == nr_steps - 1)
					{
						last_hand_update = start;
						boost::shared_future<void> previous_hand_handle = hand_handle;
						hand_handle = boost::async(boost::launch::async, boost::bind(move_hand, std::move(hand_joints), hand, previous_hand_handle)).share();
					}
					step_counter++;
				}

				
				//test 2 joints each step. We cannot check them all every step due to the time it would take...
				if (run_arm && soft_enabled_arm && arm)
				{
					//until next step, get joints positions and state. If stop requested, break execution
					while (wake_up_time > boost::chrono::high_resolution_clock::now() && !stop_movement)
					{
						Joint_States joints_values;
						joints_values.position.resize(nr_joints);
						joints_values.velocity.resize(nr_joints);

						//boost::unique_lock<boost::mutex> lck(mutex_barrett);
						//float pos[7];
						bool modules_ok;
						// arm positions
						modules_ok = arm->getPosAll(joints_values.position.data());
						if (!modules_ok)
								throw std::string("Failed to get the arm current positions!");

						//hand positions
						std::copy(joints.begin() + arm_nr_joints, joints.end(), joints_values.position.begin() + arm_nr_joints);
						//double dArray[4]; std::vector<float> hand_pos(4,1);
						//modules_ok = hand->getPositions(dArray);
						//if (!modules_ok)
								//throw std::string("Failed to get the hand current positions!");
						////move spread to end of array
						//std::rotate(dArray, dArray + 1, dArray + 4);
						//for (int i = 0; i < 4; i++)
								//hand_pos.at(i) = static_cast<float>(dArray[i]) * DEG_TO_RAD_F;
						//std::copy(hand_pos.begin(), hand_pos.end(), joints_values.position.begin() + arm_nr_joints);

						// arm velocities
						modules_ok = arm->getVelAll(joints_values.velocity.data());
						if (!modules_ok)
								throw std::string("Failed to get the arm current velocities!");

						// hand velocities
						// it is 0 in this case because I cannot read from the hand
						//double dArrayVel[4]; 
						std::vector<float> hand_vel(4,0);
						//modules_ok = hand->getRefSpeeds(dArrayVel);
						//if (!modules_ok)
							//throw std::string("Failed to get the hand current velocities!");
						////move spread to end of array
						//std::rotate(dArrayVel, dArrayVel + 1, dArrayVel + 4);
						//for (int i = 0; i < 4; i++)
							//hand_vel.at(i) = static_cast<float>(dArrayVel[i]) * DEG_TO_RAD_F;
						std::copy(hand_vel.begin(), hand_vel.end(), joints_values.velocity.begin() + arm_nr_joints);

						//modules_ok = arm->getPosAll(joints_values.position.data());
						//if (!modules_ok)
							//throw std::string("Failed to execute the synchronized arm motion!");

						//std::copy(joints.begin() + arm_nr_joints, joints.end(), joints_values.position.begin() + arm_nr_joints);

						
						//if (hand_machine_state == HandMachineState::Grasping)
							//joints_values.end_effector_state = Joint_States::EndEffector::Grasping;
						//else if (hand_machine_state == HandMachineState::Releasing)
							//joints_values.end_effector_state = Joint_States::EndEffector::Releasing;
						//else
							//joints_values.end_effector_state = Joint_States::EndEffector::Moving;
						
						
						//push the joint states
						{
							boost::unique_lock<boost::mutex> lck(joint_states_queue_mtx);
							joint_states_queue.push(boost::move(joints_values));
						}

						//notify that we have pushed data into queue
						joint_states_queue_cv.notify_one();
					}
				}
				else
				{
					
				
					//sleep until the time point where we should process the next step
					boost::this_thread::sleep_until(wake_up_time);
				}
			}
			if(hand_handle.valid())
				hand_handle.get();	//this will let the exception, if thrown in async method, to pass and be catch in below catch statement
			
			//signal that this trajectory was finished
			executing_movement_error = false;
			executing_movement = false;
			executing_movement_cv.notify_all();
		}
		catch (std::string &ex)
		{
			std::cout << "Error: " << ex << std::endl;
			last_error_str = ex;
			if (arm && soft_enabled_arm)
			{
				//boost::unique_lock<boost::mutex> lck(mutex_amtec);
				arm->SoftStop();
				if (!arm->are_Modules_OK())
					std::cout << "Arm specific error: " << arm->Get_Error_Str() << std::endl;
			}
			if (hand && soft_enabled_hand)
			{
				for (int i = 0; i < hand->getAxes(); i++)
				{
					boost::unique_lock<boost::mutex> lck(mutex_barrett);
					int status;
					std::string error;
					std::string name;
					hand->getAxisName(i, name);
					if (hand->getStatus(i, &status, error) && status != 0)
						std::cout << "Hand specific error (axe " << name << "): " << error << std::endl;
				}
			}
			//switch to error state
			executor_state = ExecutorState::Error;
			executing_movement_error = true;
			executing_movement = false;
			executing_movement_cv.notify_all();
		}
	}
}

void CYarpCommunicationServerUpperLimb::VelTrajectoryMovementsExecutor()
{
	//The executor has two possible states:
	//
	//  Ready -> waiting for trajectories os executing one.
	//
	//  Error -> failed to execute a step or some joint failed
	//
	// It starts from cold in Ready mode.
	//	On error, Reset must be invoked, otherwise no more trajectories are processed
	//	If an error occurs while executing a movement, the remaining of the trajectory is discared.

	//		Start						End
	//		------->*****************------->
	//				*	Ready		*
	//		------->*				*--------
	//		|		*****************		|
	//		|								|  Error on execution
	//Reset |		*****************	    |
	//		|		*	Error		*       |
	//		|------	*				*<-------
	//				*****************--------->End


	while (!stop_vel_executor)
	{
		try
		{
			switch (executor_state)
			{
			case ExecutorState::Ready:
				break;
			case ExecutorState::Error:
			{
				//sleep for some time waiting for either error is cleared or stop is requested
				boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
				continue;
			}
				break;
			default:
				break;
			}

			//acquire the movements queue lock
			boost::unique_lock<boost::mutex> lck(vel_queue_mtx);
			if (stop_vel_movement)
			{
				//flush the queue
				while (!vel_trajectory_queue.empty())
					vel_trajectory_queue.pop_front();

				std::cout << "Upperlimb trajectory executor: Current trajectory was stopped and queue flushed!" << std::endl;
				//reset flag
				stop_vel_movement = false;
			}
			//wait for someome signal that we have a new movement to perform

			//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
			//result is the same as predicate: !vel_trajectory_queue.empty();
			bool result = vel_queue_not_empty_cv.wait_for(lck, boost::chrono::milliseconds(500), [&](){ return !vel_trajectory_queue.empty(); });

			if (!result)	//timeout
				continue;

			//signal that we are performing a movement ---------------------------------------------------------------

			// get the mean acceleration of the trajectory
			std::vector<std::vector<float>> j_acc_mean_traj;
			std::vector<VelTrajectoryStep> j_vel_traj = std::get<0>(vel_trajectory_queue.front());
			for(int i=0;i<j_vel_traj.size()-1;++i)
			{
				//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "trajectory = " << i;
				//TRACE("trajectory = %d\n",i);
				std::vector<float> j_vel_a = std::get<0>(j_vel_traj.at(i)); 
				boost::chrono::duration<float, boost::ratio<1>> timestep(std::get<1>(j_vel_traj.at(i))); 
				std::vector<float> j_vel_b = std::get<0>(j_vel_traj.at(i+1));

				std::vector<float> j_acc_mean;
				for(int ii=0;ii<j_vel_a.size();++ii)
					j_acc_mean.push_back((j_vel_b.at(ii)-j_vel_a.at(ii))/timestep.count());

				j_acc_mean_traj.push_back(j_acc_mean);

			}
			VelTrajectoryMovement vel_mov;			

			executing_vel_movement_error = false;
			executing_vel_movement = true;
			//dequeue a movement
			vel_mov = boost::move(vel_trajectory_queue.front());
			vel_trajectory_queue.pop_front();
			//should arm and hand mov?
			bool run_arm = std::get<1>(vel_mov);
			bool run_hand = std::get<2>(vel_mov);
			size_t nr_steps = std::get<0>(vel_mov).size();

			//process each trajectory step
			if(run_hand){boost::unique_lock<boost::mutex> lck_hand(mutex_barrett);} 
			if(!hand->rt_mode_on){hand->startRTmode();}
			int acc=0;

			//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "NEW TRAJECTORY";

			boost::chrono::duration<float, boost::ratio<1>> ta,tb,tx;			
			std::vector<float> j_acc_mean;
			std::vector<float> j_vel_x;
			bool modules_ok;
			double dArray[4]; //std::vector<float> hand_pos(4,1);
			double dArrayVel[4]; //std::vector<float> hand_vel(4,0);
			Joint_States joints_values;

			for each(auto &step in std::get<0>(vel_mov))
			{
				boost::chrono::system_clock::time_point t_start_interval = boost::chrono::system_clock::now();
				
				if (stop_vel_movement || (acc==j_acc_mean_traj.size()))	{break;}				
				std::vector<float> j_vel_a = std::get<0>(step);
				boost::chrono::duration<float, boost::ratio<1>> timestep(std::get<1>(step));
				j_acc_mean = j_acc_mean_traj.at(acc); acc++;
				j_vel_x.resize(j_acc_mean.size());

				//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "new interval";
				//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "timestep [sec] = "<< timestep.count();


				//boost::chrono::system_clock::time_point t_start_interval = boost::chrono::system_clock::now(); 				
				tx = boost::chrono::system_clock::now() - t_start_interval;
				//tx = ta; 
				//tb = ta + timestep;

				//boost::chrono::high_resolution_clock::time_point t_start_interval = boost::chrono::high_resolution_clock::now(); 
				//tb = timestep;
				//ta = boost::chrono::system_clock::now() - t_start_interval;
				//tx = ta; 
				//tb = ta + timestep;
				

				
				//while((tx.count() <= tb.count()) && !stop_vel_movement)
				while((tx.count() <= timestep.count()) && !stop_vel_movement)
				{
					
					//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "ta [sec] = "<< ta.count();
					//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "tb [sec] = "<< tb.count();
					//BOOST_LOG_SEV(lg,boost::log::trivial::info) << "tx [sec] = "<< tx.count();

					// set the interpolated velocities
					for(int ii=0;ii<j_acc_mean.size();++ii)
						j_vel_x.at(ii) = (j_vel_a.at(ii)+(j_acc_mean.at(ii)*tx.count()));

					MoveVel(j_vel_x);

					
					// read the positions and the velocities
					joints_values.position.resize(nr_joints);	joints_values.velocity.resize(nr_joints);
					// arm positions
					//modules_ok = 
					arm->getPosAll(joints_values.position.data());	//if (!modules_ok){break;}
					//hand positions
					//modules_ok =
					//hand->getRTPositions(dArray);	//if (!modules_ok){break;}
					//std::vector<float> hand_pos(dArray,dArray+4);
					std::vector<float> hand_pos(4,0);
					std::transform(hand_pos.begin(),hand_pos.end(),hand_pos.begin(),std::bind1st(std::multiplies<float>(),DEG_TO_RAD_F));
					std::copy(hand_pos.begin(), hand_pos.end(), joints_values.position.begin() + arm_nr_joints);

					// arm velocities
					arm->getVelAll(joints_values.velocity.data());	//if (!modules_ok){break;}
					// hand velocities
					//hand->getRTVelocities(dArrayVel);	//if (!modules_ok){break;}
					//std::vector<float> hand_vel(dArrayVel,dArrayVel+4);
					std::vector<float> hand_vel(4,0);
					std::transform(hand_vel.begin(),hand_vel.end(),hand_vel.begin(),std::bind1st(std::multiplies<float>(),DEG_TO_RAD_F));
					std::copy(hand_vel.begin(), hand_vel.end(), joints_values.velocity.begin() + arm_nr_joints);

					//push the joint states
					boost::unique_lock<boost::mutex> lck(joint_states_queue_mtx);
					joint_states_queue.push(boost::move(joints_values));
					//notify that we have pushed data into queue
					joint_states_queue_cv.notify_one();
					
					

					tx = boost::chrono::system_clock::now() - t_start_interval;
					//tx = boost::chrono::high_resolution_clock::now() - t_start_interval;	
				
				} // while loop interval
			}// for loop each step

			// stop the motion
			std::vector<float>j_vel_0(nr_joints,0.0);	MoveVel(j_vel_0);														
														
			//signal that this trajectory was finished
			executing_vel_movement_error = false;
			executing_vel_movement = false;
			executing_vel_movement_cv.notify_all();
			/*
			BOOST_LOG_SEV(lg,boost::log::trivial::info) << "trajectory executed";			
			std::vector<float> j_pos(arm_nr_joints);	arm->getPosAll(j_pos.data());					
			BOOST_LOG_SEV(lg,boost::log::trivial::info) << "j_pos [deg] = "<< j_pos.at(0)*RAD_TO_DEG_F
																	<< " " << j_pos.at(1)*RAD_TO_DEG_F - 90
																	<< " " << j_pos.at(2)*RAD_TO_DEG_F
																	<< " " << j_pos.at(3)*RAD_TO_DEG_F - 90
																	<< " " << j_pos.at(4)*RAD_TO_DEG_F
																	<< " " << j_pos.at(5)*RAD_TO_DEG_F
																	<< " " << j_pos.at(6)*RAD_TO_DEG_F
																	;
																	*/
																	
																	

		}
		catch (std::string &ex)
		{
			std::cout << "Error: " << ex << std::endl;
			last_vel_error_str = ex;

			arm->SoftStop();
			if (!arm->are_Modules_OK())
				std::cout << "Arm specific error: " << arm->Get_Error_Str() << std::endl;

			for (int i = 0; i < hand->getAxes(); i++)
			{
				boost::unique_lock<boost::mutex> lck(mutex_barrett); hand->stopRTmode();
				int status;
				std::string error;
				std::string name;
				hand->getAxisName(i, name);
				if (hand->getStatus(i, &status, error) && status != 0)
					std::cout << "Hand specific error (axe " << name << "): " << error << std::endl;
			}

			//switch to error state
			executor_state = ExecutorState::Error;
			executing_vel_movement_error = true;
			executing_vel_movement = false;
			executing_vel_movement_cv.notify_all();
		}
	} // while vel_executor
	

}
