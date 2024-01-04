#ifndef __NN_TASK_H__
#define __NN_TASK_H__

#include "stdint.h"
#include "nn_rtosTypes.h"
#include "config.h"
#include "port_macros.h"
#include "port.h"


//Init Functions
void nnOsKernelInit(void);
void nnOsKernelLaunch(uint32_t quanta);


//System Functions
nn_CallRes_t nnOsAddThread(void(*taskFunName)(void),uint32_t priority);
void nnOsThreadSleep(uint32_t sleep_time);
void nnOsThreadYield(void);
uint32_t nnOs_GetSystemTime(void);

//Semaphores
void nnOsSemaphoreInit(int32_t *semaphore,int32_t value);
void nnOsSignalSet(int *semaphore);
void nnOsSignalWait(int32_t *semaphore);
void nnOsSignalCooperativeWait(int32_t *semaphore);

//Inter thread communication
void nnOsMailboxInit(void);
void nnOsMailboxSend(uint32_t data);
uint32_t nnOsMailboxRecv(void);
void nnOsFifoInit(void);
int8_t nnOsFifoPut(uint32_t data);
uint32_t nnOsFifoGet(void);

#endif 