#ifndef _ARM_MOVEMENTS_AROS_H_
#define _ARM_MOVEMENTS_AROS_H_

#pragma once

#include "YarpCommunicationServerAmtecLWArm.h"

void PerformArmMaintenance_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm );

void PerformGoHome_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm );

void PerformRestPosition_ARoS_Right( CYarpCommunicationServerAmtecLWArm * arm );

void PerformArmMaintenance_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm );

void PerformGoHome_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm );

void PerformRestPosition_ARoS_Left( CYarpCommunicationServerAmtecLWArm * arm );

#endif // _ARM_MOVEMENTS_AROS_H_
