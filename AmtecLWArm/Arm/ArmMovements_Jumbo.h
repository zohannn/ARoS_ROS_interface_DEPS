#ifndef _ARM_MOVEMENTS_JUMBO_H_
#define _ARM_MOVEMENTS_JUMBO_H_

#pragma once

#include "YarpCommunicationServerAmtecLWArm.h"

void PerformArmMaintenance_Jumbo( CYarpCommunicationServerAmtecLWArm * arm );

void PerformGoHome_Jumbo( CYarpCommunicationServerAmtecLWArm * arm );

void PerformRestPosition_Jumbo( CYarpCommunicationServerAmtecLWArm * arm );

#endif //_ARM_MOVEMENTS_JUMBO_H_
