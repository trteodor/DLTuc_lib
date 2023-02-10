#define DLT_LOG_CONTEX           "MAIN"
#define DLT_LOG_APPID            "NUM1"

#include "main.h"
#include "DLTuc.h"
#include "nnRTOS.h"
#include "stdint.h"


UART_DMA_Handle_Td *Uart2HandlerPointer;  


/*CallBacks used by ucDltLibrary section start..*/
void UART2_TransmitDMAEndCallBack()
{
	DLTuc_MessageTransmitDone(); /*Inform ucDLTlib about message transmission end*/
		/*This way to make this example simple to read...*/
}


/*This CallBack was registered in main function using function: DLTuc_RegisterTransmitSerialDataCallback*/
void UART2_LowLevelDataTransmit(uint8_t *DltLogData, uint8_t Size)
{
	/*This function is called by ucDLTlib when is something to send*/

	TUART_DMA_Trasmit(Uart2HandlerPointer,DltLogData,Size); /*Call of this function may block contex!!! */
		/*This way to make this example simple to read...*/
}
/*CallBacks used by ucDltLibrary section end..*/

extern void TaskContex1(void);
extern void TaskContex2(void);
extern void TaskContex3(void);


uint32_t count6,count7;

int main(void)
{
	PLL_Config64MHZ();

	nnOsKernelInit();
	if(nnOsAddThread(&TaskContex1,1) != Call_ok)
	{
		while(1); //Error!
	}
	if(nnOsAddThread(&TaskContex2,2) != Call_ok)
	{
		while(1); //Error!
	}
	if(nnOsAddThread(&TaskContex3,2) != Call_ok)
	{
		while(1); //Error!
	}

	GPIO_Pin_Cfg(GPIOA,Px5,gpio_mode_output_PP_2MHz);
	GPIO_PinSet(GPIOA, Psm5); /*Led is very exicited! :) */
	Uart2HandlerPointer = UART2_Init115200(UART2_TransmitDMAEndCallBack);
	/*Init UART with baud 115200 and pass pointer called after transmission end (transmit complet DMA)*/
	/*Microcontoler Initialization end...*/
	/**********************************************************/


	/*Register Low Level Transmit function for DLTuc Library*/
	DLTuc_RegisterTransmitSerialDataCallback(UART2_LowLevelDataTransmit);
	DLTuc_RegisterGetTimeStampMsCallback(nnOs_GetSystemTime); 	/*Register GetSysTime function*/
	/*The function "GetSysTime" must return the time in ms*/

	/*Now ucDLTlib is ready to work!*/
	// LOGL(DL_INFO, "DLT TESTS START!!!");

	nnOsKernelLaunch(QUANTA);

	while(1); /*We shouldn't reach never*/

}

