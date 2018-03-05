#include "WrapperAmtecArm.h"
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost\chrono.hpp>

#include "Windows.h" //because of HANDLE

CWrapperAmtecArm::CWrapperAmtecArm( void )
: PStepServer()
, ant( -1 )
{
	//start the thread responsible for processing the movements
	stop_executor = false;
	stop_movement = false;
	traj_executor_thrd = boost::thread(boost::bind(&CWrapperAmtecArm::TrajectoryMovementsExecutor, this));

	//change thread priority
	HANDLE threadHandle = traj_executor_thrd.native_handle();

	BOOL suc = SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
	if (!suc)
	{
		std::cout<<"Error: Failed to set priority to trajectory executor"<<std::endl;
	}
}

CWrapperAmtecArm::~CWrapperAmtecArm( void )
{
	stop_executor = true;
	stop_movement = true;
	if (traj_executor_thrd.joinable())
		traj_executor_thrd.join();
}

void CWrapperAmtecArm::HomeJoint( int joint )
{
	success_ = Amtec_Arm.HomeJoint( joint );
}

void CWrapperAmtecArm::Home( std::vector<unsigned int> &home_order )
{
	success_ = Amtec_Arm.Home( home_order );
}

int CWrapperAmtecArm::HomeAll( void )
{
	success_ = Amtec_Arm.HomeAll();
	return success_;
}

void CWrapperAmtecArm::HomeGripper( void )
{
	success_ = Amtec_Arm.HomeGripper();
}

bool CWrapperAmtecArm::Success( void )
{
	return success_;
}

std::string CWrapperAmtecArm::Get_Device_Error( void )
{
	return Amtec_Arm.Get_Error_Str();
}

bool CWrapperAmtecArm::isArmStatusOK( void )
{
	success_ = Amtec_Arm.isArmStatusOK();
	return success_;
}

void CWrapperAmtecArm::freeJoint( int nJoint )
{
	success_ = Amtec_Arm.freeJoint( nJoint );
}

void CWrapperAmtecArm::ResetJoint( int joint )
{
	success_ = Amtec_Arm.Reset( joint );
}

void CWrapperAmtecArm::ResetAll( void )
{
	success_ = Amtec_Arm.ResetAll();
}

void CWrapperAmtecArm::Restart( void )
{
	std::cout << "Restarting module: ";
	success_ = Amtec_Arm.Restart_Module();

	std::cout << ( success_ ? "ok" : " : error" ) << std::endl;
}

void CWrapperAmtecArm::StopJoint( int joint )
{
	success_ = Amtec_Arm.stop( joint );
}

void CWrapperAmtecArm::StopAll( void )
{
	success_ = Amtec_Arm.stop();
}

void CWrapperAmtecArm::SoftStop( void )
{
	success_ = Amtec_Arm.SoftStop();
}

void CWrapperAmtecArm::CloseGripper( float value, unsigned int time )
{
	success_ = Amtec_Arm.moveGripperPos( value, static_cast<unsigned short>( time ) );
}

void CWrapperAmtecArm::OpenGripper( void )
{
	success_ = Amtec_Arm.moveGripperPos( 0.060f );
}

float CWrapperAmtecArm::GetGripperPos( void )
{
	float pos = 0.0f;
	success_ = Amtec_Arm.getGripper( &pos );
	return pos;
}

int CWrapperAmtecArm::GetNumberJoints( void )
{
	int n = 0;
	success_ = Amtec_Arm.getAxes( &n );
	return n;
}

float CWrapperAmtecArm::GetPos( int joint )
{
	float fAux = 0.0f;
	success_ = Amtec_Arm.getPos( joint, &fAux );
	return fAux;
}

void CWrapperAmtecArm::GetPosAll( std::vector<float> &data )
{
	int number = 0;

	success_ = Amtec_Arm.getAxes( &number );

	if( !success_ )
		return;

	data.assign( number, 0.0f );

	float *fPos = new float[number];

	success_ = Amtec_Arm.getPosAll( fPos );

	if( !success_ )
		return;

	for( size_t i = 0; i < data.size(); i++ )
		data.at( i ) = fPos[i];

	delete[] fPos;
	fPos = nullptr;
}

float CWrapperAmtecArm::GetPosDeg( int joint )
{
	float fAux = 0.0f;
	success_ = Amtec_Arm.getPosDeg( joint, &fAux );
	return fAux;
}

std::vector<float> CWrapperAmtecArm::GetPosDegAll( void )
{
	std::vector<float> fData;

	int number = 0;

	Amtec_Arm.getAxes( &number );

	fData.assign( number, 0.0f );

	float *fPosDeg = new float[number];

	success_ = Amtec_Arm.getPosAllDeg( fPosDeg );

	for( size_t i = 0; i<fData.size(); i++ )
		fData.at( i ) = fPosDeg[i];

	delete[] fPosDeg;
	fPosDeg = nullptr;

	return fData;
}

void CWrapperAmtecArm::GetJointLimits( int joint, float * fLimits )
{
	success_ = Amtec_Arm.getMinLimit( joint, &fLimits[0] );
	success_ &= Amtec_Arm.getMaxLimit( joint, &fLimits[1] );
}

std::vector<float> CWrapperAmtecArm::Get_Force_Torque( void )
{
	std::vector<float> fData;
	float data[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	fData.assign( 6, 0.0f );

	int error_no = Amtec_Arm.get_force_torque( data );

	if( error_no < 0 )
	{
		std::cerr <<
			"ERROR: " << Amtec_Arm.Get_Error_Function_String( error_no ) << std::endl;

		//msgOut.strText = Amtec_Arm.Get_Error_Function_String( error_no );
	}
	else
	{
		for( unsigned int i = 0; i < 6; i++ )
			fData.at( i ) = data[i];
	}

	//msgOut.iErrorCode = error_no;

	return fData;
}

int CWrapperAmtecArm::WaitMotionEnd( unsigned int timeout_ms )
{
	int state = 0;
	success_ = Amtec_Arm.waitMotionEnd( timeout_ms, &state );
	return state;
}

int CWrapperAmtecArm::WaitMotionEnd( std::vector<float> refs, float stop_margin )
{
	size_t vSize = refs.size();

	float *fRefs = new float[vSize];

	for( size_t i = 0; i < vSize; i++ )
		fRefs[i] = refs.at( i );

	success_ = Amtec_Arm.waitMotionEnd( fRefs, stop_margin );

	delete[] fRefs;
	fRefs = nullptr;

	return success_;
}

void CWrapperAmtecArm::MoveJointPosition( int joint, float value )
{
	success_ = Amtec_Arm.positionMove( joint, static_cast<double>( value ) );
}

void CWrapperAmtecArm::MoveJointPositionRamp( int joint, float pos )
{
	success_ = Amtec_Arm.positionMoveRamp( joint, pos );
}

void CWrapperAmtecArm::MoveJointPositionRamp( int joint, float pos, float vel, float acc )
{
	success_ = Amtec_Arm.positionMoveRamp( joint, pos, vel, acc );
}

void CWrapperAmtecArm::MoveStep( int joint, float value, unsigned short time, bool wait_end )
{
	success_ = Amtec_Arm.positionMove( joint, value, time, wait_end );
}

void CWrapperAmtecArm::MoveStepAll( std::vector<float> fData )
{
	unsigned short usTimes[7] = { 0, 0, 0, 0, 0, 0, 0 };
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i<14; i += 2 )
	{
		fJoints[i] = fData.at( i );
		usTimes[i] = static_cast<unsigned short>( fData.at( i+1 ) );
	}

	success_ = Amtec_Arm.positionMove( fJoints, usTimes );
}

void CWrapperAmtecArm::MoveStepSameTime( std::vector<float> *fData, std::vector<unsigned int> *joints2move )
{
	bool bJoints2Move[7] = { false, false, false, false, false, false, false };
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i<7; i++ )
	{
		fJoints[i] = ( *fData )[i];
		bJoints2Move[i] = ( *joints2move )[i] == 0 ? false : true;
	}

	success_ = Amtec_Arm.positionMove( fJoints, bJoints2Move, static_cast<unsigned short>( fData->back() ), false );
}

void CWrapperAmtecArm::MoveStepSameTimeWait( std::vector<float> fData )
{
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i<7; i++ )
	{
		fJoints[i] = fData.at( i );
	}

	success_ = Amtec_Arm.positionMove( fJoints, static_cast<unsigned short>( fData.back() ), true );
}

void CWrapperAmtecArm::MoveStepAllSameTime( std::vector<float> fData )
{
	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i<7; i++ )
	{
		fJoints[i] = fData.at( i );
	}

	success_ = Amtec_Arm.positionMove( fJoints, static_cast<unsigned short>( fData.back() ) );
}

void CWrapperAmtecArm::MoveTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData)
{
	//Expected protocol:
	//uParam[0] -> nr joints values
	//uParam[1] -> Nr trajectory steps
	//fData[0->NrJoints-1] -> joints values for first step
	//fData[NrJoints] -> time value for first step
	//fData[NrJoints+1->2*NrJoints] -> joints values for second step
	// and so forth...

	//prepare the trajectory
	if (uParam.size() != 2)
		throw std::string("The uParam vector size must be 2");
	
	size_t nr_joints = static_cast<size_t>(uParam[0]);
	size_t nr_steps = static_cast<size_t>(uParam[1]);

	size_t fData_expected_size = (nr_joints + 1)*nr_steps;
	if (fData.size() != fData_expected_size)
		throw std::string("Metadata and data does not match: fData size is different from the uParam info.");

	TrajectoryMovement trajectory;

	for (size_t idx = 0; idx < nr_steps; idx++)
	{
		size_t start_idx = idx*(nr_joints + 1);
		TrajectoryStep step;

		//copy the joints values
		std::copy(fData.begin() + start_idx, fData.begin() + start_idx + nr_joints, std::back_inserter(std::get<0>(step)));

		//copy movement time
		std::get<1>(step) = static_cast<unsigned>(*(fData.begin() + start_idx + nr_joints));
		trajectory.emplace_back(std::move(step));
	}

	boost::unique_lock<boost::mutex> lck(queue_mtx);
	trajectory_queue.push(boost::move(trajectory));
	queue_not_empty_cv.notify_one();
}

int CWrapperAmtecArm::WaitTrajectoryEnd(int timeout_ms)
{	
//acquire the movements queue lock
	boost::unique_lock<boost::mutex> lck(executing_movement_mtx);

	//wait for executor to signal that the movement has finished

	//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
	//result is the same as predicate: !movements_queue.empty();
	if (timeout_ms == 0)
	{ //do not block waiting.....
		return executing_movement ? 1 : 0;
	}
	else if( timeout_ms < 0)
	{	//wait infenetly
		executing_movement_cv.wait(lck,[&](){ return !executing_movement; });
		return 0;
	}
	else
	{
		bool result = executing_movement_cv.wait_for(lck, boost::chrono::milliseconds(timeout_ms), 
			[&](){ return !executing_movement; });
		if (!result)	//timeout
			return 1;
	}
	return 0;
}

void CWrapperAmtecArm::StopClearTrajectory()
{
	boost::unique_lock<boost::mutex> lck(queue_mtx);

	if(!executing_movement && trajectory_queue.empty())
		return;

	stop_movement = true;
}

void CWrapperAmtecArm::TrajectoryMovementsExecutor()
{
	while (!stop_executor)
	{
		try
		{
			TrajectoryMovement mov;
			{
				//acquire the movements queue lock
				boost::unique_lock<boost::mutex> lck(queue_mtx);

				if(stop_movement)
				{
					//flush the queue
					while(!trajectory_queue.empty())
						trajectory_queue.pop();

					//reset flag
					stop_movement = false;
				}

				//wait for someome signal that we have a new movement to perform

				//the function only blocks if pred returns false, and notifications can only unblock the thread when it becomes true
				//result is the same as predicate: !movements_queue.empty();
				bool result = queue_not_empty_cv.wait_for(lck, boost::chrono::milliseconds(500), [&](){ return !trajectory_queue.empty(); });

				if (!result)	//timeout
					continue;
				//signal that we are performing a movement
				executing_movement = true;

				//dequeue a movement
				mov = boost::move(trajectory_queue.front());
				trajectory_queue.pop();
			}

			for each (auto &step in mov)
			{
				if (stop_movement)	//stop executing if stop_movement goes to true
					break;

				const float *joints = std::get<0>(step).data();
				unsigned time = std::get<1>(step);
				bool arm_cmd_ok = Amtec_Arm.positionMoveSync(joints, time, false);
				if (!arm_cmd_ok)
					throw std::string("Failed to execute the synchronized arm motion!");

				boost::this_thread::sleep_for(boost::chrono::milliseconds(time-20));

				/*int state;
				arm_cmd_ok = Amtec_Arm.waitMotionEnd(joints, time + 200, &state);
				if (!arm_cmd_ok)
					throw std::string("Failed to wait for the synchronized arm motion!");

				if (state)
					throw std::string("Timeout waiting for the synchronized arm motion to finish!");*/
			}
		
			executing_movement = false;
			executing_movement_cv.notify_all();
		}
		catch (std::string &ex)
		{
			std::cout << "Error: " << ex << ". Arm specific error: " << Amtec_Arm.Get_Error_Str() << std::endl;
			executing_movement = false;
			executing_movement_cv.notify_all();
		}
	}
}

void CWrapperAmtecArm::MoveVel( int j, double sp )
{
	success_ = Amtec_Arm.velocityMove( j, sp );
}

void CWrapperAmtecArm::MoveVel( std::vector<float> fData )
{
	size_t vSize = fData.size();

	double *joints = new double[vSize];

	for( size_t i = 0; i < vSize; i++ )
		joints[i] = fData.at( i );

	success_ = Amtec_Arm.velocityMove( joints );

	delete[] joints;
	joints = nullptr;
}

void CWrapperAmtecArm::MoveAllVel( std::vector<float> fData )
{
	int
		i = 0,
		nJoints = 0;

	float fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	success_ = Amtec_Arm.getAxes( &nJoints );

	for( i = 0; i<nJoints; i++ )
	{
		fJoints[i] = fData.at( i );
	}

	success_ &= Amtec_Arm.positionMove( fJoints, fData.back() );
}

void CWrapperAmtecArm::MoveAllVelTraj( std::vector<float> fData, std::vector<unsigned int> uParam )
{
	int
		i = 0,
		j = 0,
		n = 0,
		nJoints = 0;

	unsigned short *TrajectoryTime = nullptr;

	float
		fAux = 0.0f,
		**TrajectoryPoints = nullptr;

	std::vector< float >
		vf_Times,
		vfAux;

	std::vector< std::vector< float > > vTrajPoints;

	success_ = Amtec_Arm.getAxes( &nJoints );

	std::vector< float >::iterator it_max;

	if( uParam[1] != ant )
	{
		ant = uParam[1];

		vfAux.assign( 8, 0.0f );

		vTrajPoints.push_back( vfAux );

		vfAux.clear();

		for( i = 0; i<nJoints + 1; i++ )
		{
			vTrajPoints.back()[i] = fData[i];
		}

		if( uParam[0] == 1 )
		{
			n = vTrajPoints.size();
			TrajectoryPoints = new float*[n];
			TrajectoryTime = new unsigned short[n];

			for( i = 1; i<n; i++ )
			{
				TrajectoryPoints[i] = new float[7];

				for( j = 0; j<7; j++ )
				{
					TrajectoryPoints[i][j] = vTrajPoints[i][j];
					fAux = fabs( TrajectoryPoints[i][j] - vTrajPoints[i - 1][j] ) / vTrajPoints[i][7];
					vf_Times.push_back( fAux );
				}

				it_max = std::max_element( vf_Times.begin(), vf_Times.end() );

				vf_Times.clear();

				TrajectoryTime[i] = static_cast<unsigned short>( *it_max * 1000.0f );
				if( TrajectoryTime[i] < 200 )
				{
					TrajectoryTime[i] = 200;
				}
			}

			for( i = 1; i<n; i++ )
			{
				success_ = Amtec_Arm.positionMove( TrajectoryPoints[i], TrajectoryTime[i] );
			}

			vTrajPoints.clear();

			for( i = 1; i < n; i++ )
			{
				delete[] TrajectoryPoints[i];
				TrajectoryPoints[i] = nullptr;
			}
			
			delete[] TrajectoryPoints;
			TrajectoryPoints = nullptr;

			delete[] TrajectoryTime;
			TrajectoryTime = nullptr;
		}
	}
}

void CWrapperAmtecArm::Inv_kinematics( std::vector<float> fData )
{
	int i = 0;

	float
		delta_angle = 0.0f,
		delta_angle1 = 0.0f,
		velocity = 5.0f, // graus/seg
		fJoints[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	delta_angle = 0.0f;

	std::vector<float> Joints;

	Joints.assign( 7, 0.0f );

	Joints = GetPosDegAll();

	for( i = 0; i<7; i++ )
	{
		fJoints[i] = fData[i] * 3.1416f / 180.0f;

		delta_angle1 = fabs( fData[i] - Joints[i] );

		if( delta_angle1>delta_angle )
			delta_angle = delta_angle1;
	}

	delta_angle = delta_angle*1000.0f / velocity;

	success_ = Amtec_Arm.positionMove( fJoints, static_cast<unsigned short>( delta_angle ) );
}
