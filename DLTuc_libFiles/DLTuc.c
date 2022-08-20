/*
 * dlt_logs_mcu.c
 *
 *  Created on: 1 lip 2022
 *      Author: teodor
 */

/*****************************************************************************************************
******************************************************************************************************
******************************************************************************************************/

#include "DLTuc.h"

#include "stdio.h"
#include "stdint.h"

#include "stdbool.h"
#include <stdarg.h>
#include <string.h>


/*ucDLT Logs Ring buffer typedefs
 *
 * */

typedef enum
{
	RB_OK       = 0,
	RB_ERROR	= 1
} RB_Status;

typedef struct
{
	uint16_t Head; // Pointer to write
	uint16_t Tail; // Pointer to read
	uint8_t Buffer[DLT_RING_BUFFER_SIZE]; // Array to store data
} DltRingBuffer_t;



/*
 *
 * *******************************************************************************************
 *
 * Static variables declaration and function prototypes
 * *******************************************************************************************
 *  * *******************************************************************************************
 *  * *******************************************************************************************
 * */


static void (*ExtSerialTrDataFunctionCb)(uint8_t *DltLogData, uint8_t Size);

static bool LogDroppedFlag =false;

static uint8_t DltLogDroppedInfo[] = "LOG DROPPED!!!";
static uint8_t DltLogDroppedInfoBuffer[60] = {0};
static uint8_t DLtLogDroppedSize = 0;

static uint8_t DltDebugTmpBuf[DLT_MAX_SINGLE_MESSAGE_SIZE]; /*Buffer used to prepare single message for OutPut Circular buffer*/

static uint8_t ActDltMessageCounter =0;

static DltRingBuffer_t DltRingBuffer;

static uint8_t DltMessagesTab[DLT_RING_BUFFER_SIZE][DLT_MAX_SINGLE_MESSAGE_SIZE];

static volatile uint8_t TransmitReadyStateFlag = true; /*TmpFromDma for example*/

static uint32_t TimestampValue =1000;
//TestValue += 1000;

/*brief for each funtion is added above implemention... */
static void PrepareHoleHeader(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint16_t size);
static RB_Status DLT_RB_Read(DltRingBuffer_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer);
static RB_Status DLT_RB_Write(DltRingBuffer_t *Buf,uint8_t *DltLogData, uint8_t MessageSize);

/*
 *
 ********************************************************************************************
 *
 * Static function declaration section
 ********************************************************************************************
 ********************************************************************************************
 *********************************************************************************************/

/*
 * @@brief DLT_RB_Write
 * RingBuffer_t *Buf - pointer to Ring Buffer structure
 * MessageSize - size of the "DltLogData" (return value)
 * MessagePointer - pointer to the message stored in RingBuffer (return value)
 * 
 * */
static RB_Status DLT_RB_Read(DltRingBuffer_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer)
{
	// Check if Tail hit Head
	if(Buf->Head == Buf->Tail)
	{
		// If yes - there is nothing to read
		return RB_ERROR;
	}

	// Write current value from buffer to pointer from argument
	*MessageSize = Buf->Buffer[Buf->Tail];
	*MessagePointer = &DltMessagesTab[Buf->Tail][0];

	// Calculate new Tail pointer
	Buf->Tail = (Buf->Tail + 1) % DLT_RING_BUFFER_SIZE;

	// Everything is ok - return OK status
	return RB_OK;
}

/*
 * @@brief DLT_RB_Write
 * RingBuffer_t *Buf - pointer to Ring Buffer structure
 * *DltLogData - pointer to the data stored in RingBuffer
 * MessageSize - size of the "DltLogData" 
 * 
 * */
static RB_Status DLT_RB_Write(DltRingBuffer_t *Buf,uint8_t *DltLogData, uint8_t MessageSize)
{
	// Calculate new Head pointer value
	uint8_t HeadTmp = (Buf->Head + 1) % DLT_RING_BUFFER_SIZE;

	// Check if there is one free space ahead the Head buffer
	if(HeadTmp == Buf->Tail)
	{
		// There is no space in the buffer - return an error
		return RB_ERROR;
	}

	// Store a value into the buffer
	Buf->Buffer[Buf->Head] = MessageSize;

	/*Copy the values to new buffer*/
	for(int i=0; i<MessageSize; i++)
	{
		DltMessagesTab[Buf->Head][i] = DltLogData[i];
	}

	// Remember a new Head pointer value
	Buf->Head = HeadTmp;

	// Everything is ok - return OK status
	return RB_OK;
}


/*
 * @@brief PrepareHoleHeader
 * A very stupid implementation of DLT Header - but it works fine
 * It is possible create dedicated bitfiels a structure but it require time...
 * refer to: https://www.autosar.org/fileadmin/user_upload/standards/foundation/1-0/AUTOSAR_PRS_DiagnosticLogAndTraceProtocol.pdf
 *
 * */
static void PrepareHoleHeader(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint16_t size)
{
	if(size > (254 -32) )
	{
		/*Error to handle or please develop this function */
		while(1)
		{
			/*For develop phase lock the app*/
		}

	}


	/*START HEADER*/
	DltDebugTmpBuf[0] =  0x44; /*'D'*/
	DltDebugTmpBuf[1] =  0x4c; /*'L'*/
	DltDebugTmpBuf[2] =  0x53; /*'S'*/
	DltDebugTmpBuf[3] =  0x01; /*'0x01'*/


	DltDebugTmpBuf[4] =  0x35; /*'Dlt base header config
	* Use extended header - true
	* MSBF - false
	* With ECU ID - true
	* With Seesion ID - false
	* With time stamp - true
	* version number -random
	'*/

	DltDebugTmpBuf[5] =  ActDltMessageCounter++; /*'Message counter value '*/

	/*TODO: - it must be fixed!!!! - Length*/
	DltDebugTmpBuf[6] =  0x00; /*Message length general*/
//	DltDebugTmpBuf[7] =  0x37; /*'Message length general '*/
	DltDebugTmpBuf[7]=28+size; /*General size */


	uint32_t TempEcuId = DLT_LOG_ECUID_VALUE;
	/*ECU ID*/
	DltDebugTmpBuf[8]= ((uint8_t*)&TempEcuId)[3];
	DltDebugTmpBuf[9]= ((uint8_t*)&TempEcuId)[2];
	DltDebugTmpBuf[10]= ((uint8_t*)&TempEcuId)[1];
	DltDebugTmpBuf[11]= ((uint8_t*)&TempEcuId)[0];

	/*Time stamp*/
	DltDebugTmpBuf[12]= ((uint8_t*)&TimestampValue)[3];
	DltDebugTmpBuf[13]= ((uint8_t*)&TimestampValue)[2];
	DltDebugTmpBuf[14]= ((uint8_t*)&TimestampValue)[1];
	DltDebugTmpBuf[15]= ((uint8_t*)&TimestampValue)[0];

	/*Extended header --verbose | type serial*/
//	DltDebugTmpBuf[16]= 0x41; /**/
	DltDebugTmpBuf[16]= (Level << 4) | 1;

	/*Number of arguments*/
	DltDebugTmpBuf[17]= 0x01; /**/

	/*App id */
	DltDebugTmpBuf[18]= ((uint8_t*)&AppId)[3];
	DltDebugTmpBuf[19]= ((uint8_t*)&AppId)[2];
	DltDebugTmpBuf[20]= ((uint8_t*)&AppId)[1];
	DltDebugTmpBuf[21]= ((uint8_t*)&AppId)[0];

	/*Contex ID  (4 bajty*/
	DltDebugTmpBuf[22]= ((uint8_t*)&ContextId)[3];
	DltDebugTmpBuf[23]= ((uint8_t*)&ContextId)[2];
	DltDebugTmpBuf[24]= ((uint8_t*)&ContextId)[1];
	DltDebugTmpBuf[25]= ((uint8_t*)&ContextId)[0];

	/*Type info*/
	DltDebugTmpBuf[26]= 0x01; /**/
	DltDebugTmpBuf[27]= 0x82; /**/
	DltDebugTmpBuf[28]= 0x00; /**/
	DltDebugTmpBuf[29]= 0x00; /**/

	/*Argument 1*/
	DltDebugTmpBuf[30]= size; /*the size of the load in simplified form, but not exactly but generally yes*/
	DltDebugTmpBuf[31]= 0x00; /**/
}


/*
 ****************************************************************************************************
 * API Function declarations section START
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */

/*
 *@brief DLTuc_RegisterTransmitSerialDataCallback
 * IMPORTANT!!!!!
 *  This simple stack/library must be initialized by "DLTuc_RegisterTransmitSerialDataCallback"
 *  As a parameter must be pass function which will transmit serial data
 *
 * */
void DLTuc_RegisterTransmitSerialDataCallback(void UART2_LowLevelDataTransmit(uint8_t *DltLogData, uint8_t Size))
{
	ExtSerialTrDataFunctionCb = UART2_LowLevelDataTransmit;

	/*Preapre LOG DROPPED Info Log*/
	PrepareHoleHeader(DL_ERROR,0x444C5443, 0x444C5443,sizeof(DltLogDroppedInfo) );

	/*Payload!!!*/
	/*Copy payload text temporary*/
	for(int i=DLT_ACT_HOLE_HEADER_SIZE; i<(sizeof(DltLogDroppedInfo)+DLT_ACT_HOLE_HEADER_SIZE); i++)
	{
		DltDebugTmpBuf[i]= DltLogDroppedInfo[i-DLT_ACT_HOLE_HEADER_SIZE];
	}

	for(int i=0; i<sizeof(DltLogDroppedInfoBuffer); i++)
	{
		DltLogDroppedInfoBuffer[i] = DltDebugTmpBuf[i];
	}

	DLtLogDroppedSize = DLT_ACT_HOLE_HEADER_SIZE + sizeof(DltLogDroppedInfo);
}

/*
 *@brief DLTuc_MessageTransmitDone
 * IMPORTANT!!!!!
 *  Call this function when the transsmision is end
 * For example in "DMA transmission end callback" to inform the lib that the message is transmitted
 *
 * */
void DLTuc_MessageTransmitDone(void)
{
	uint8_t TmpMessageSize=0;
	uint8_t *TmpMessagePointer = NULL;

	if(LogDroppedFlag == true)
	{
		LogDroppedFlag = false;

		if(ExtSerialTrDataFunctionCb != NULL)
		{
			ExtSerialTrDataFunctionCb(DltLogDroppedInfoBuffer, DLtLogDroppedSize);
		}
		return;
	}

	if(DLT_RB_Read(&DltRingBuffer,&TmpMessageSize,&TmpMessagePointer) == RB_OK)
	{
		if(ExtSerialTrDataFunctionCb != NULL)
		{
			ExtSerialTrDataFunctionCb(TmpMessagePointer, TmpMessageSize);
		}
	}
	else
	{
		TransmitReadyStateFlag = true;
	}
}

/*
 * @brief DLTuc_LogOutVarArgs(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...);
 *
 * function to create DLT Log
 *
 * */
void DLTuc_LogOutVarArgs(DltLogLevel_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...)
{
va_list ap;
uint16_t Size;

	va_start(ap, Payload);
	Size = vsprintf((char *)DltDebugTmpBuf + DLT_ACT_HOLE_HEADER_SIZE, (char *)Payload,ap);
	va_end(ap);

	PrepareHoleHeader(Level,AppId,ContextId,Size);
	Size = Size +DLT_ACT_HOLE_HEADER_SIZE;

	/*Additional zero on the end of message -therefore it works with more stability */
	Size++;
	DltDebugTmpBuf[DLT_ACT_HOLE_HEADER_SIZE + Size] = 0x00;
	Size++;
	DltDebugTmpBuf[DLT_ACT_HOLE_HEADER_SIZE + Size] = 0x00;

	if(DLT_RB_Write(&DltRingBuffer,DltDebugTmpBuf, Size) != RB_OK)
	{
		LogDroppedFlag = true;
	}

	uint8_t TmpMessageSize=0;
	uint8_t *TmpMessagePointer = NULL;

	if(TransmitReadyStateFlag == true)
	{
		if(DLT_RB_Read(&DltRingBuffer,&TmpMessageSize,&TmpMessagePointer) == RB_OK)
			{
				TransmitReadyStateFlag = false;
				if(ExtSerialTrDataFunctionCb != NULL)
				{
					ExtSerialTrDataFunctionCb(TmpMessagePointer, TmpMessageSize);
				}
				else
				{
					while(1); /*Please Register the callback...*/
				}
			}
	}
}


/*
 *@brief DLTuc_UpdateTimeStampMs
 *
 *
 * */
void DLTuc_UpdateTimeStampMs(uint32_t Time)
{
	TimestampValue = (Time*10);
}


