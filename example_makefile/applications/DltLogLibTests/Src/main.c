
#include "main.h"

#include "ucDLTlib.h"

#define UseSysTick

UART_DMA_Handle_Td *Uart2HandlerPointer;  


/*CallBacks used by ucDltLibrary section start..*/
void SysTickCallBack(uint32_t TickCount)
{
	DLTuc_UpdateTimeStampMs(TickCount); /*Inform ucDLTlib about TimeStamp*/
}

void UART2_TransmitDMAEndCallBack()
{
	DLTuc_MessageTransmitDone(); /*Inform ucDLTlib about message transmission end*/
		/*This way to make this example simple to read...*/
}

void UART2_LowLevelDataTransmit(uint8_t *DltLogData, uint8_t Size)
{
	/*This function is called by ucDLTlib when is something to send*/
		/*This CallBack was registered in "main function using function: DLTuc_RegisterTransmitSerialDataCallback*/
	TUART_DMA_Trasmit(Uart2HandlerPointer,DltLogData,Size); /*Call of this function may block contex!!! */
		/*This way to make this example simple to read...*/
}
/*CallBacks used by ucDltLibrary section end..*/


int Main(void) 
{
	/*Microcontoler Initialization start...*/
	LowLevelSystemInit(SleepMode);
	ConfigSysTick1ms();
	// ConfigRTC(40000, 1); /*1 Seconds*/
	GPIO_Pin_Cfg(GPIOA,Px5,gpio_mode_output_PP_2MHz);
	GPIO_PinSet(GPIOA, Psm5); /*Led is very exicited! :) */
	/*Microcontoler Initialization end...*/

	RegisterSysTickCallBack(SysTickCallBack); 	/*Register SysTick Callback*/
	Uart2HandlerPointer = UART2_Init115200(UART2_TransmitDMAEndCallBack);
		/*Init UART with baud 115200 and pass pointer called after transmission end (transmit complet DMA)*/


	/*Register Low Level Transmit function for ucDltLibrary*/
	DLTuc_RegisterTransmitSerialDataCallback(UART2_LowLevelDataTransmit);

	/*Now ucDLTlib is ready to work!*/
	DEBUGL(DL_INFO, "DLT TESTS START!!!");

	for(int i=0; i<50; i++)
	{
		DEBUGL(DL_DEBUG, "Hello DLT with period 5ms");
		DelayMs(5);
	}
	DelayMs(100);
	/*LOG DROP TEST*/
	for(int i=0; i<20; i++)
	{
		DEBUGL(DL_INFO, "Log Drop testing0 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing1 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing2:)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing3 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing4 :)  %d" , 5);
	}
		DEBUGL(DL_INFO, "Dropped log...  %d" , 5);
		DEBUGL(DL_FATAL, "Dropped log...   %d" , 5);
	DelayMs(100);

	while(1)
	{
		/*Send example Logs in loop...*/
		DEBUGL(DL_ERROR, "Hello DLT Again Arg1 %d Arg2 :%d" , 2565, 56);
		DEBUGFF(DL_FATAL, "GENERALLY DLT Again1");
		DEBUGF(DL_DEBUG, "AnotherTest DLT Again");
		DEBUGL(DL_VERBOSE, "AnotherTest2 DLT Again");
		DelayMs(1000);
		DEBUGL(DL_WARN, "Orange is sweet fruit");
		DelayMs(1000);
		tooglePIN(GPIOA, Psm5);
	}
}


