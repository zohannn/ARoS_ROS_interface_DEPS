#ifndef _ARM_MOVEMENTS_H_
#define _ARM_MOVEMENTS_H_

#pragma once

#include <string>
#include "YarpCommunicationServerAmtecLWArm.h"

#ifndef DEG_TO_RAD_F
#define DEG_TO_RAD_F 0.017453292519943295769236907684886f
#endif

#ifndef RAD_TO_DEG_F
#define RAD_TO_DEG_F 57.295779513082320876798154814105f
#endif

bool check_error( CYarpCommunicationServerAmtecLWArm * arm, int iJoint );

void ArmMaintenance( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file );

void ArmGoHome( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file );

void RestPosition( CYarpCommunicationServerAmtecLWArm * arm_ptr, std::string config_file );

#endif // _ARM_MOVEMENTS_H_
