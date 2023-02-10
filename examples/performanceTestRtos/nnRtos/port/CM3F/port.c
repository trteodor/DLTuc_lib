#include "port.h"

uint32_t nnOs_SystemTime;

void (*IncrementSystemTimeFunPtr)(void);



void nnOsSchedulerLaunch(void);

void nnOsPort_KernelLaunch(uint32_t quanta, uint32_t millis_prescaler){
	SysTick->CTRL =0;
	SysTick->VAL  =0;
	SysTick->LOAD =  (quanta*millis_prescaler)-1;

	SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7
	SYSPRI3 =(SYSPRI3&0xFF00FFFF)|0x00010000; // priority 1 PendSV
	SysTick->CTRL =  0x00000007;

	nnOsSchedulerLaunch();
}

void nnPort_ThreadYield(void){
INTCTRL  =  0x10000000; //Trigger PendSV - Contex Switcher
}

void nnOsRegisterSysIncrementCb(void (*SysIncFunPtr)(void))
{
	IncrementSystemTimeFunPtr = SysIncFunPtr;
}

void SysTickCHandler(void) // TODO: This fun shoudn't exist - I should write it in Assembly leangue
{
	IncrementSystemTimeFunPtr(); //If this callback will bw not registered MemFault will be indicated
	INTCTRL  =  0x10000000; //Trigger PendSV - Contex Switcher
}
