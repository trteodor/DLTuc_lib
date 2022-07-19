
#include "main.h"

#include "dlt_logs_mcu.h"

#define UseSysTick

UART_DMA_Handle_Td *Uart2HandlerPointer;  



void SysTickCallBack(uint32_t TickCount)
{
	DLTuc_UpdateTimeStampMs(TickCount);
}

void SerialUartTransmitDMATransmitEndCallBack()
{
	DLTuc_MessageTransmitDone(); 
		/*This way to make this example simple to read...*/
}

void LLSerialTrDataFunctionC(uint8_t *DltLogData, uint8_t Size)
{
	TUART_DMA_Trasmit(Uart2HandlerPointer,DltLogData,Size); 
		/*This way to make this example simple to read...*/
}



int Main(void) 
{
	LowLevelSystemInit(SleepMode);
	ConfigSysTick1ms();
	RegisterSysTickCallBack(SysTickCallBack);
	// ConfigRTC(40000, 1); /*1 Seconds*/


	Uart2HandlerPointer = UART2_Init115200(SerialUartTransmitDMATransmitEndCallBack);

	GPIO_Pin_Cfg(GPIOA,Px0,gpio_mode_output_OD_2MHz);
	GPIO_PinReset(GPIOA, Psm0);

	DLTuc_RegisterTransmitSerialDataCallback(LLSerialTrDataFunctionC);

	/*LOG DROP TEST*/
	for(int i=0; i<20; i++)
	{
		DEBUGL(DL_INFO, "Log Drop testing0 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing1 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing2:)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing3 :)  %d" , 5);
		DEBUGL(DL_INFO, "Log Drop testing4 :)  %d" , 5);
	}

	while(1)
	{
		/*Send example Logs in loop...*/
		DEBUGL(DL_ERROR, "Hello DLT Again %d Arg1 %d Arg2 :%d" , 2565, 56,23);
		DEBUGFF(DL_FATAL, "GENERALLY DLT Again1");
		DEBUGL(DL_DEBUG, "AnotherTest DLT Again",0);
		DEBUGL(DL_VERBOSE, "AnotherTest2 DLT Again",0);
		DelayMs(1000);
		DEBUGL(DL_WARN, "Orange is sweet fruit");
		DelayMs(1000);
	}
}


