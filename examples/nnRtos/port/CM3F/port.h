#ifndef __NN_PORTHEADER_H__
#define __NN_PORTHEADER_H__


#include "stm32f1xx.h"
/*Here Include Your Device header (from CMSIS Library) */

//Cortex Registers defines..
#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))

void nnOsRegisterSysIncrementCb(void (*SysIncFunPtr)(void));
void nnOsPort_KernelLaunch(uint32_t quanta, uint32_t millis_prescaler);
void nnPort_ThreadYield(void);


//Export Global 
extern uint32_t nnOs_SystemTime;

#endif