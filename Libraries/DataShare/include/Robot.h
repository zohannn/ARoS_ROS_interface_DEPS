#ifndef _ROBOT_H_
#define _ROBOT_H_

#pragma once

#include <vector>
#include "RobotBase.h"

	struct arm_elements
	{
		std::vector < float >
			aHandRotation,
			aRobotConfig,
			ParkConfig;

		int
			nTypeOfGrip,
			nSelTar;

		arm_elements(void)
		{
			nTypeOfGrip = 0;
			nSelTar = 0;
		}
	};

	class Robot
		: public RobotBase
	{
	public:
		/// <summary>Arm elements.</summary>
		std::vector<struct arm_elements> arm;

	public:
		/// <summary>Constructor.</summary>
		Robot(void);

		/// <summary>Copy constructor.</summary>
		/// <param name="obj">Robot to be copied.</param>
		Robot(const Robot &obj);
	};

#endif // _ROBOT_H_
