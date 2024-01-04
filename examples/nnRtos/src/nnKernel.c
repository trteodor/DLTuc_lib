#include "stm32f1xx.h"
#include "stdint.h"
#include "nnRTOS.h"
#include "port.h"

uint32_t MILLIS_PRESCALER;

struct tcb{
	
    int32_t *stackPt;
	struct tcb *nextPt;
	uint32_t sleepTime;
	uint32_t blocked;
	uint32_t priority;
	
};

typedef struct tcb tcbType;

tcbType tcbs[MAX_NUM_OF_THREADS+1]; //+1 -- IDLE Task
tcbType *currentPt;
uint8_t AddedTasksCounter = 0;

int32_t TCB_STACK[MAX_NUM_OF_THREADS+1][STACKSIZE];



nn_CallRes_t nnOsAddThread(void(*taskFunName)(void),uint32_t priority);
void nnOsSysTickInc(void);





__attribute__((weak))void nnOsIdleTask(void) /*If you want use it - please overwrite */
{
	while(1)
	{
		//Nothing ToDo:
		//__WFI();
	}
}

static void nnOsThreadStackInit(int i)
{
   tcbs[i].stackPt =  &TCB_STACK[i][STACKSIZE -16];//StackPointer
	 TCB_STACK[i][STACKSIZE -1] =  0x01000000; /*The T (24)bit in EPSR must be one always instruction pop may overwrite it 
  												This is reset value - 0x01000000*/ 
}

nn_CallRes_t nnOsAddThread(void(*taskFunName)(void),uint32_t priority)
{
    nn_CallRes_t retVal = Call_ok;

	nnENTRY_CRITCAL();
    if(AddedTasksCounter < MAX_NUM_OF_THREADS)     // AddedTasksCounter -- static global
    {
        for(int i=0; i<AddedTasksCounter; i++)
        {
            tcbs[i].nextPt = &tcbs[i + 1];
        }
        tcbs[AddedTasksCounter].nextPt = &tcbs[0];

        tcbs[AddedTasksCounter].priority = priority;
        
        nnOsThreadStackInit(AddedTasksCounter);

        TCB_STACK[AddedTasksCounter][STACKSIZE-2] = (int32_t)(taskFunName); /*Init PC*/

        tcbs[AddedTasksCounter].blocked  = 0;
        tcbs[AddedTasksCounter].sleepTime = 0;

        AddedTasksCounter ++ ;

        retVal = Call_ok;
    }
    else
    {
        retVal = Call_Error; //ERROR!!!
    }

	nnLEAVE_CRITCAL();
return retVal;
}

void nnOsKernelInit(void)
{
	nnOsRegisterSysIncrementCb(&nnOsSysTickInc);
	nnOsAddThread(&nnOsIdleTask,253);
	/*After kernel start frist Called Task will be Idle - until frist switch */
    currentPt = &tcbs[0]; //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	MILLIS_PRESCALER =  (BUS_FREQ/1000);
}


void nnOsKernelLaunch(uint32_t quanta){
	nnOsPort_KernelLaunch(quanta,MILLIS_PRESCALER);
}

void nnOsThreadYield(void){
	nnPort_ThreadYield();
}

void nnOsSysTickInc(void)
{
	nnOs_SystemTime++;

	//I'am not sure where put it
	for(int i =0;i<=AddedTasksCounter;i++){
		if(tcbs[i].sleepTime >0){
		tcbs[i].sleepTime--;
		}
	}
}

uint32_t nnOs_GetSystemTime(void)
{
	return nnOs_SystemTime;
}

//Function called from Port Interrupt - for example from PendSVIrq or SysTick
void nnOsPriorityScheduler(void) //0 - most important task(highest priority ) -> 255 lowest priority
{
	tcbType *_currentPt = currentPt;
	tcbType *nextThreadToRun = _currentPt;
	uint8_t highestPriorityFound = 255;
	
	do{
		_currentPt =_currentPt->nextPt;
		if((_currentPt->priority < highestPriorityFound)&&
				(_currentPt->blocked ==0)&&
			(_currentPt->sleepTime ==0)){
				
				nextThreadToRun =_currentPt;
				highestPriorityFound = _currentPt->priority; 
				}
	}while(_currentPt != currentPt);
 
	currentPt  =  nextThreadToRun;
}

void nnOsThreadSleep(uint32_t sleep_time){
	nnENTRY_CRITCAL();
	currentPt->sleepTime =  sleep_time;
	nnOsThreadYield();
	nnLEAVE_CRITCAL();
}
