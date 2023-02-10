#include "nnRTOS.h"

#define FIFO_SIZE			15


//TODO: write useable functions for mailboxes ... e.g. forwarding the address of a queue and many more this implementation is very lazy ..
static uint8_t  MB_hasdata;
static uint32_t MB_Data;
static int32_t  MB_Sem;

void nnOsMailboxInit(void){

  MB_hasdata = 0;
  MB_Data  = 0;
  nnOsSemaphoreInit(&MB_Sem,0);	
}

void nnOsMailboxSend(uint32_t data){

	  __disable_irq();
	  if(MB_hasdata){
		   __enable_irq();
			return;
		}
	MB_Data  = data;
	MB_hasdata = 1;
	__enable_irq();
	nnOsSignalSet((int *)&MB_Sem);	
		
}


uint32_t nnOsMailboxRecv(void){

	 nnOsSignalWait(&MB_Sem);
	 uint32_t data;
   __disable_irq();
	 data = MB_Data;
	 MB_hasdata = 0;
	__enable_irq();
	
	return data;
}


//TODO: write useable functions for queues ... e.g. forwarding the address of a queue and many more this implementation is very lazy ..
uint32_t PutI;
uint32_t GetI;
uint32_t OS_Fifo[FIFO_SIZE];
int32_t  current_fifo_size;
uint32_t lost_data;


void nnOsFifoInit(void){
	PutI =0;
	GetI =0;
	nnOsSemaphoreInit(&current_fifo_size,0);
	lost_data =0;
}


int8_t nnOsFifoPut(uint32_t data){
	
  if(current_fifo_size ==  FIFO_SIZE){
	  lost_data++;
		return -1;
	}
	OS_Fifo[PutI] = data;
	PutI = (PutI+1)% FIFO_SIZE;
	nnOsSignalSet((int *)&current_fifo_size);
	
	return 1;
}
 

uint32_t nnOsFifoGet(void){
    uint32_t data;
	nnOsSignalCooperativeWait(&current_fifo_size);
  __disable_irq();
  data  = OS_Fifo[GetI];
  GetI =  (GetI+1)%FIFO_SIZE;
  __enable_irq();	
	
	return data;
  
	
}