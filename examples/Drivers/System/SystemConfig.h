#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__

#include "stm32f1xx.h"

#define PRIGROUP_16G_0S ((const uint32_t) 0x03)
#define PRIGROUP_8G_2S ((const uint32_t) 0x04)
#define PRIGROUP_4G_4S ((const uint32_t) 0x05)
#define PRIGROUP_2G_8S ((const uint32_t) 0x06)
#define PRIGROUP_0G_16S ((const uint32_t) 0x07)



typedef enum
{
	SleepMode,
	StopMode,
	Standby,
}LowPowerMode_t;


void LowLevelSystemInit(LowPowerMode_t LowPowerMode);
void ConfigSysTick1ms(void);
uint32_t GetSysTime(void);
void RegisterSysTickCallBack(void(*SysTickCallBack)(uint32_t TickCount));
void DelayMs(uint32_t Ticks);

#endif 