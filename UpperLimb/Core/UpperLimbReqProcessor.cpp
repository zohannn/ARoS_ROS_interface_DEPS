#include "UpperLimbReqProcessor.h"
#include "commands.h"

UpperLimbReqProcessor::UpperLimbReqProcessor():
	arm_nr_joints(7),
	hand_nr_joints(4),
	nr_joints(arm_nr_joints + hand_nr_joints)
{
	//start the thread responsible for processing the movements
	executing_movement = false;
	executing_movement_error = false;
	stop_executor = false;
	stop_movement = false;
	soft_enabled_hand = true;
	soft_enabled_arm = true;
	traj_executor_thrd = boost::thread(boost::bind(&UpperLimbReqProcessor::TrajectoryMovementsExecutor, this));

	//change thread priority
	HANDLE threadHandle = traj_executor_thrd.native_handle();

	BOOL suc = SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
	if (!suc)
	{
		std::cout << "Error: Failed to set priority to trajectory executor" << std::endl;
	}

	hand_machine_state = HandMachineState::Moving;
}

UpperLimbReqProcessor::~UpperLimbReqProcessor()
{
	stop_executor = true;
	stop_movement = true;
	if (traj_executor_thrd.joinable())
		traj_executor_thrd.join();
}

void UpperLimbReqProcessor::SetDevices(boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm_, boost::shared_ptr<yarp::dev::BarrettHand826X> hand_)
{
	arm = arm_;
	hand = hand_;

	//Prepare the specific commands hand processor
	PrepareHandProcessor();
}

void UpperLimbReqProcessor::ProcessReq(CMessage &in_req, CMessage &out_reply, YarpConnector &con)
{
	int nAux;
	//flag indicating if command was processed by one of the specific commands processor (arm or hand)
	bool processed = false;

	//asynchronously waits for trajectory end and sends a command to all connected clients
	boost::function<void(YarpConnector &)> async_mov_finished = [&](YarpConnector &con)
	{
		int ret;
		//wait for trajectory end
		ret = WaitTrajectoryEnd(-1);

		CMessage msg;
		// Assign one value to uParam, the return value of WaitMotionEnd
		msg.uParam.push_back(static_cast<unsigned int>(ret));
		msg.uCommand = UpperLimb::TRAJECTORY_ASYNC_FINISHED;
		msg.strText = last_error_str;
		bool sent_fin = con.Send(msg, false);
		if (!sent_fin)
			std::cout << "Failed to send an ASYNC_FINISHED message: " << con.get_ErrorString();
	};

	try
	{
		switch (in_req.uCommand)
		{
		case UpperLimb::ADD_TRAJECTORY:
			AddTrajectory(in_req.uParam, in_req.fData);
			break;
		case UpperLimb::STOP_CLEAR_TRAJECTORY:
			StopClearTrajectory();
			break;
		case UpperLimb::WAIT_TRAJECTORY_END:
			if (in_req.uParam.size() > 0)
				// Perform wait based in time
				nAux = WaitTrajectoryEnd(in_req.uParam.front());
			else
				// Perform wait based in time
				nAux = WaitTrajectoryEnd(-1);

			out_reply.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>(nAux)
				);
			break;
		case UpperLimb::WAIT_TRAJECTORY_END_ASYNC:
			boost::async(boost::launch::async, boost::bind(async_mov_finished, boost::ref(con)));
			break;

		case UpperLimb::CONFIGURE_LIMBS:
			if (in_req.uParam.size() != 2)
				throw std::string("Invalid configuration!");

			soft_enabled_arm = in_req.uParam[0] == 0 ? false : true;
			soft_enabled_hand = in_req.uParam[1] == 0 ? false : true;
			break;
		case UpperLimb::GET_POSTURE:
		{
			out_reply.fData.resize(nr_joints);
			bool modules_ok = arm->getPosAll(out_reply.fData.data());
			if (!modules_ok)
				throw std::string("Failed to get arm current posture!");

			double dArray[4];
			modules_ok = hand->getPositions(dArray);

			//move spread to end of array
			std::rotate(dArray, dArray + 1, dArray + 4);

			for (int i = 0; i <= 3; i++)
				out_reply.fData[i + arm_nr_joints] = static_cast<float>(dArray[i]) * 3.14159265f /180.0f;

			if (!modules_ok)
				throw std::string("Failed to get hand current posture!");
		}
			break;

		case UpperLimb::GRASP_OBJECT:
			hand_machine_state = HandMachineState::Grasping;
			if (!CloseTorque())
				throw Report_Error_at_Command(in_req.uCommand);
			break;

		case UpperLimb::RELEASE_OBJECT:
			hand_machine_state = HandMachineState::Releasing;
			if (!Open())
				throw Report_Error_at_Command(in_req.uCommand);
			break;
			//--------------------------------------------------------------------------
		default:
			//it is a specific command...
			processed = ArmProcessor(in_req, out_reply, con);
			if (processed)
				break;
			processed = HandProcessor(in_req, out_reply, con);
			if (!processed)
			{
				out_reply.strText = "unknown command";
				out_reply.iErrorCode = 1;
			}
			break;
		}
	}
	catch (std::string &ex)
	{
		out_reply.iErrorCode = 2;
		out_reply.strText = ex;
	}
}

void UpperLimbReqProcessor::AddTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData)
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

int UpperLimbReqProcessor::WaitTrajectoryEnd(int timeout_ms)
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
	{	//wait infenetly
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

void UpperLimbReqProcessor::StopClearTrajectory()
{
	boost::unique_lock<boost::mutex> lck(queue_mtx);

	if (!executing_movement && trajectory_queue.empty())
	{
		std::cout << "Received a request to stop trajectory, but no trajectory was under execution and queue is empty" << std::endl;
		return;
	}

	stop_movement = true;
}


bool UpperLimbReqProcessor::start_joints_states_sender(std::string name)
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

	joint_states_out_worker = boost::thread(boost::bind(&UpperLimbReqProcessor::joint_states_job, this));

	return true;
}

void UpperLimbReqProcessor::stop_joints_states_sender()
{
	joint_states_out_worker_end_job = true;
	joint_states_out_channel.interrupt();
	joint_states_out_channel.close();
	joint_states_queue_cv.notify_all();
	if (joint_states_out_worker.joinable())
		joint_states_out_worker.join();
}

void UpperLimbReqProcessor::joint_states_job()
{
	while (!joint_states_out_worker_end_job)
	{
		Joint_States joints;
		{
			boost::unique_lock<boost::mutex> lck(joint_states_queue_mtx);
			joint_states_queue_cv.wait(lck, [&]() { return joint_states_out_worker_end_job || !joint_states_queue.empty(); });
			if (joint_states_out_worker_end_job)
				continue;

			//dequeue a info
			joints = std::move(joint_states_queue.front());
			joint_states_queue.pop();
		}

		{
			//std::unique_lock<std::mutex> yc_lock(yc_mtx);
			if (joint_states_out_channel.getOutputCount() != 0)
			{
				Joint_States &nf = joint_states_out_channel.prepare();
				nf = boost::move(joints);
				//this info might be dropped (in write) in case there is an ongoing write operation
				joint_states_out_channel.write();
			}
		}
	}


	//BOOST_LOG_SEV(lg, info) << "Ended joint_states thread.";
	std::cout << "Ended joint_states thread." << std::endl;
}

void UpperLimbReqProcessor::ResetExecutor()
{
	executor_state = ExecutorState::Ready;
}

UpperLimbReqProcessor::ExecutorState UpperLimbReqProcessor::GetExecutorState()
{
	return executor_state;
}

void UpperLimbReqProcessor::TrajectoryMovementsExecutor()
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

			boost::unique_lock<boost::mutex> lck(mutex_barrett);

			//send values to hand only each 5 steps
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
				unsigned int uptowake = time <= 25 ? time : time - 25;
				wake_up_time = start + boost::chrono::milliseconds(uptowake);

				//Send joints values to arm
				if (run_arm && soft_enabled_arm && arm)
				{
					bool arm_cmd_ok = arm->positionMoveSync(joints.data(), time, false);
					if (!arm_cmd_ok)
						throw std::string("Failed to execute the synchronized arm motion!");
				}

				//Send joints values to hand
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
						std::rotate(hand_joints.begin(), hand_joints.end() - 1, hand_joints.end());

						bool ok;
						if (abs(hand_joints[1] - hand_joints[2]) < 0.01
							&& abs(hand_joints[1] - hand_joints[3]) < 0.01)
							ok = hand->positionMoveFingers(hand_joints[1]);

						else if (abs(hand_joints[1] - hand_joints[2]) < 0.01)
							ok = hand->positionMoveTwoFingers(const_cast<double*>(hand_joints.data()));
						else
							ok = hand->positionMoveFingers(hand_joints.data());

						if (!ok)
						{
							throw std::string("Failed to move Fingers.");
						}
						//move spread
						ok = hand->positionMove(0, hand_joints[0]);

						if (!ok)
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

						//float pos[7];
						bool modules_ok = true;
						for (int i = 0; i < 7; i++)
						{
							modules_ok &= arm->is_Module_OK(i);
							if (!modules_ok)
								throw std::string("Failed to execute the synchronized arm motion!");
						}

						modules_ok = arm->getPosAll(joints_values.position.data());
						if (!modules_ok)
							throw std::string("Failed to execute the synchronized arm motion!");

						std::copy(joints.begin() + arm_nr_joints, joints.end(), joints_values.position.begin() + arm_nr_joints);

						if (hand_machine_state == HandMachineState::Grasping)
							joints_values.end_effector_state = Joint_States::EndEffector::Grasping;
						else if (hand_machine_state == HandMachineState::Releasing)
							joints_values.end_effector_state = Joint_States::EndEffector::Releasing;
						else
							joints_values.end_effector_state = Joint_States::EndEffector::Moving;
						
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
				arm->SoftStop();
				if (!arm->are_Modules_OK())
					std::cout << "Arm specific error: " << arm->Get_Error_Str() << std::endl;
			}
			if (hand && soft_enabled_hand)
			{
				for (int i = 0; i < hand->getAxes(); i++)
				{
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