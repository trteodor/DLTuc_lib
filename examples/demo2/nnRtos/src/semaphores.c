#include "nnRTOS.h"



void nnOsSemaphoreInit(int32_t *semaphore,int32_t value){

	 *semaphore = value;
	
}


void nnOsSignalSet(int *semaphore)
{
	 nnENTRY_CRITCAL();
	  *semaphore +=1;
	 nnLEAVE_CRITCAL();
}

void nnOsSignalWait(int32_t *semaphore)
{
	nnENTRY_CRITCAL();
	
	while(*semaphore <=0)
	{
		nnENTRY_CRITCAL();
		nnLEAVE_CRITCAL();
	}

	 *semaphore -=1;
	nnLEAVE_CRITCAL();
}

void nnOsSignalCooperativeWait(int32_t *semaphore)
{
	nnENTRY_CRITCAL();
	
	while(*semaphore<=0){
		nnENTRY_CRITCAL();
		nnOsThreadYield();
		nnLEAVE_CRITCAL();
	}
	nnENTRY_CRITCAL();
	*semaphore -=1;
	 nnLEAVE_CRITCAL();
}