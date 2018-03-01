#include "Robot.h"

	Robot::Robot(void)
		: RobotBase()
	{
	}

	Robot::Robot(const Robot &obj)
		: RobotBase(obj)
	{
		arm.resize(obj.arm.size());

		size_t arm_size = arm.size();
		for (size_t i = 0; i < arm_size; i++)
		{
			arm.at(i).nTypeOfGrip = obj.arm.at(i).nTypeOfGrip;
			arm.at(i).nSelTar = obj.arm.at(i).nSelTar;

			arm.at(i).aHandRotation.assign(obj.arm.at(i).aHandRotation.begin(), obj.arm.at(i).aHandRotation.end());

			arm.at(i).aRobotConfig.assign(obj.arm.at(i).aRobotConfig.begin(), obj.arm.at(i).aRobotConfig.end());

			arm.at(i).ParkConfig.assign(obj.arm.at(i).ParkConfig.begin(), obj.arm.at(i).ParkConfig.end());
		}
	}