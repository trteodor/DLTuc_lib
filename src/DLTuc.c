/**
 * @file DLTuc.c
 * @author Teodor Rosolowski
 * @date 1 Jul 2022
 * @brief This file is a part of DLTuc library
 *
 * In this source file you will find entire implementation of the DLTuc lib.
 *
 * Requirments:
 * Around ~2kB of RAM
 * Check Configuration file and defines:
 * DLT_TRANSMIT_RING_BUFFER_SIZE, DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE
 *
 */

/*
 * *******************************************************************************************
 * Includes
 * *******************************************************************************************
 * */
#include "DLTuc.h"

#include "stdio.h"
#include "stdint.h"

#include "stdbool.h"
#include <stdarg.h>
#include <string.h>

/*
 * *******************************************************************************************
 * Local defines
 * *******************************************************************************************
 * */

/**!
* \brief DLT_ACT_HEADER_SIZE - actual DLT header size
* \details  Standard Header + Extended + TypeInfo
* Don't modify it if you aren't sure what are you doing!!!
*/
#define DLT_ACT_HEADER_SIZE 32

/**!
* \brief DLT_MINIMUM_LOG_DROP_PERIOD
* \details minimum period between two drop info log
*/
#define DLT_MINIMUM_LOG_DROP_PERIOD 200

/*
 * *******************************************************************************************
 * Local types
 * *******************************************************************************************
 * */

/* Prototypes to improve readability.. */
/*typedefs for future use..*/
// typedef
// {
// 	uint32_t HeaderStart;
// 	BaseHeaderConfig_t BaseHeaderConfig;
// 	uint8_t MessageCounter;
// 	uint16_t MessageLength;
// 	uint32_t EcuId;
// 	uint32_t TimeStamp;
// }DLT_BaseHeader_t;

// typedef
// {
// 	DLT_DebugLevel_t DebugLevel;
// 	uint8_t NumberOfArguments;
// 	uint32_t AppId;
// 	uint32_t ContexID;
// 	TypeInfo_t TypeInfo;
// 	uint16_t Argument1;
// }DLT_ExtendedHeader_t;


/**!
 * \brief RB_Status
 * \details ---
 * */
typedef enum
{
	RB_OK       = 0,
	RB_ERROR	= 1
} RB_Status;

/**!
 * \brief DltRingBufferTransmit_t
 * \details ---
 * */
typedef struct DltRingBufferTransmit_Tag
{
	uint16_t Head; // Pointer to write
	uint16_t Tail; // Pointer to read
	bool readyToTransmit[DLT_TRANSMIT_RING_BUFFER_SIZE];
	uint8_t dataSize[DLT_TRANSMIT_RING_BUFFER_SIZE]; // Array to keep message size
} DltRingBufferTransmit_t;

/**!
 * \brief BluRingBufferReceive_t
 * \details ---
 * */
typedef struct BluRingBufferReceive_Tag
{

	uint16_t Head; // Pointer to write
	uint16_t Tail; // Pointer to read
	uint8_t  MessageSize[DLT_RECEIVE_RING_BUFFER_SIZE]; // Array to keep message size
	bool ReadyToRead[DLT_RECEIVE_RING_BUFFER_SIZE];
} BluRingBufferReceive_t;

/**
 * *******************************************************************************************
 * Static variables
 * *******************************************************************************************
 * */

/**!
 * \brief ExtSerialTrDataFunctionCb
 * \details ---
 * */
static void (*ExtSerialTrDataFunctionCb)(uint8_t *DltLogData, uint8_t Size);

static void (*ExtSerialRecDataFunctionCb)(uint8_t *RecDataBuff, uint16_t Size);

static void (*ExtInfoInjectionDataRcvdCb)(uint32_t AppId, uint32_t ConId,uint32_t ServId,uint8_t *Data, uint16_t Size);

static uint32_t (*GetSystemTimeMs)(void);

/**!
 * \brief LogDroppedFlag
 * \details ---
 * */
static bool LogDroppedFlag =false;

static uint32_t PrevLogDropSendTime = 0u;

static uint8_t DltLogDroppedInfo[] = "LOG DROPPED!!!";

static uint8_t DltLogDroppedInfoBuffer[60] = {0}; /* TODO: Remove magic number..*/

static uint8_t DLtLogDroppedSize = 0;

static uint8_t ActDltMessageCounter =0;

static DltRingBufferTransmit_t DltTrsmtRingBuffer = {.readyToTransmit[0]=true};

static uint8_t DltTrsmtMessagesTab[DLT_TRANSMIT_RING_BUFFER_SIZE][DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE];

static BluRingBufferReceive_t BleMainReceiveRingBuffer;

static uint8_t BluMainReceiveMessagesTab[DLT_RECEIVE_RING_BUFFER_SIZE][DLT_REC_SINGLE_MESSAGE_MAX_SIZE];

static volatile uint8_t TransmitReadyStateFlag = true; /*TmpFromDma for example*/

/*
 *********************************************************************************************
 * Prototypes of static functions
 ********************************************************************************************
 */

/*!
 ************************************************************************************************
 * \brief PrepareDltHeader
 * \details A very lazy implementation of DLT Header - but it works fine
 * Please refer to: https://www.autosar.org/fileadmin/user_upload/standards/foundation/1-0/AUTOSAR_PRS_DiagnosticLogAndTraceProtocol.pdf
 ************************************************************************************************/
static void PrepareDltHeader(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint16_t size,uint8_t *headerAddrStart);

/*!
 ************************************************************************************************
 * \brief DLT_RB_TransmitRead Function to read data from ring buffer
 * \details --
 * \param RingBuffer_t *Buf - pointer to Ring Buffer structure
 * \param out MessageSize - size of the "DltLogData" (return value)
 * \param out MessagePointer - pointer to the message stored in RingBuffer (return value)
 * \return RB_OK if something read succesfully, otherwise RB_ERROR
 * */
static RB_Status DLT_RB_TransmitRead(DltRingBufferTransmit_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer);

/*!
 ************************************************************************************************
 * \brief DLT_RB_GetNextWriteIndex
 * \details --
 * \param in RingBuffer_t *Buf - pointer to Ring Buffer structure
 * \param out writeIndex - next index where DLT data should be stored, can be used only if fun return "RB_OK"
 * \return RB_OK if index available, otherwise RB_ERROR
 ************************************************************************************************/
static RB_Status DLT_RB_GetNextWriteIndex(DltRingBufferTransmit_t *Buf,uint16_t *writeIndex);

/*!
 ************************************************************************************************
 * \brief DLT_RB_Receive_GetNextMessageAddress
 * \details Function used to work with DMA - direct write to ring buffer by DMA
 * \param in Buf -
 * \param in WriteAddress -
 ************************************************************************************************/
static RB_Status DLT_RB_Receive_GetNextMessageAddress(BluRingBufferReceive_t *Buf, uint8_t **WriteAddress);

/*!
 ************************************************************************************************
 * \brief DLT_RB_Receive_Read Function to read data from ring buffer
 * \details --
 * \param RingBuffer_t *Buf - pointer to Ring Buffer structure
 * \param out MessageSize - size of the "BleLogData" (return value)
 * \param out MessagePointer - pointer to the message stored in RingBuffer (return value)
 *
 * */
static RB_Status DLT_RB_Receive_Read(BluRingBufferReceive_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer);

/*
 *********************************************************************************************
 * Static functions implementation
 ********************************************************************************************
 */

static RB_Status DLT_RB_Receive_GetNextMessageAddress(BluRingBufferReceive_t *Buf, uint8_t **WriteAddress)
{
	/* TODO: The implementation isn't optimal...*/
	static uint8_t DefaultBlindBuffer[DLT_REC_SINGLE_MESSAGE_MAX_SIZE];

	/*Mark previous message as ready to read*/
	Buf->ReadyToRead[Buf->Head] = true;

	// Compute new Head pointer value of a ring buffer
	uint16_t HeadTmp = (Buf->Head + 1) % DLT_RECEIVE_RING_BUFFER_SIZE;

	// Check if there is one free space ahead the Head buffer
	if(HeadTmp == Buf->Tail)
	{
		/*Even if buffer is full data must be received somewhere to don't crush application/ dma*/
		*WriteAddress = DefaultBlindBuffer;
		// There is no space in the buffer - return an error
		return RB_ERROR;
	}

	Buf->ReadyToRead[HeadTmp] = false;
	Buf->MessageSize[HeadTmp] = DLT_RECEIVE_RING_BUFFER_SIZE;
	Buf->Head = HeadTmp;

	*WriteAddress = &BluMainReceiveMessagesTab[HeadTmp][0];

	// Everything is ok - return OK status
	return RB_OK;
}

static RB_Status DLT_RB_Receive_Read(BluRingBufferReceive_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer)
{
	if(Buf->ReadyToRead[Buf->Tail] == false)
	{
		/*Any message in ring buffer isn't ready to read*/
		return RB_ERROR;
	}
	/*Mark again as not ready to read*/
	Buf->ReadyToRead[Buf->Tail] = false;

	// Check if Tail hit Head
	if(Buf->Head == Buf->Tail)
	{
		// If yes - there is nothing to read
		return RB_ERROR;
	}

	// Write current value from buffer to pointer from argument
	*MessageSize = Buf->MessageSize[Buf->Tail];
	*MessagePointer = &BluMainReceiveMessagesTab[Buf->Tail][0];

	// Calculate new Tail pointer
	Buf->Tail = (Buf->Tail + 1) % DLT_RECEIVE_RING_BUFFER_SIZE;

	// Everything is ok - return OK status
	return RB_OK;
}

static RB_Status DLT_RB_TransmitRead(DltRingBufferTransmit_t *Buf, uint8_t *MessageSize, uint8_t **MessagePointer)
{
	// Check if Tail hit Head
	if(Buf->Head == Buf->Tail)
	{
		// If yes - there is nothing to read
		return RB_ERROR;
	}
	else if(true == Buf->readyToTransmit[Buf->Tail])
	{
		// Write current value from buffer to pointer from argument
		*MessageSize = Buf->dataSize[Buf->Tail];
		*MessagePointer = &DltTrsmtMessagesTab[Buf->Tail][0];

		// Calculate new Tail pointer
		Buf->Tail = (Buf->Tail + 1) % DLT_TRANSMIT_RING_BUFFER_SIZE;
		// Everything is ok - return OK status
		return RB_OK;
	}
	else
	{
		/* Message still not ready to transmit */
		return RB_ERROR;
	}

}

static RB_Status DLT_RB_GetNextWriteIndex(DltRingBufferTransmit_t *Buf,uint16_t *writeIndex)
{
DLTuc_OS_CRITICAL_START();
	// Calculate new Head pointer value
	uint8_t HeadTmp = (Buf->Head + 1) % DLT_TRANSMIT_RING_BUFFER_SIZE;

	// Check if there is one free space ahead the Head buffer
	if(HeadTmp == Buf->Tail)
	{
		DLTuc_OS_CRITICAL_END();
		// There is no space in the buffer - return an error
		return RB_ERROR;
	}
	Buf->Head = HeadTmp;
	Buf->readyToTransmit[Buf->Head] = false;
	*writeIndex = Buf->Head;
	DLTuc_OS_CRITICAL_END();

	return RB_OK;
}

static void PrepareDltHeader(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint16_t size,uint8_t *headerAddrStart)
{
	uint32_t ActualTime = 0;

	if(GetSystemTimeMs != NULL)
	{
		ActualTime = GetSystemTimeMs() * 10U; 
				/*Multiply by 10 to get value in MS also in DLTViewer
				*Reson: Resolution in DLT Viewer is equal: 10^-4
				*/
	}

	if(size > ((UINT8_MAX -1) - DLT_ACT_HEADER_SIZE) )
	{
		/*Error to handle, or please develop this function to handle input size > UINT8_MAX */
		// while(1)
		// {
		// 	/*For development phase: lock the app*/
		// }
		size = size - DLT_ACT_HEADER_SIZE -1;
	}


	/*START HEADER*/
	headerAddrStart[0] =  0x44; /*'D'*/
	headerAddrStart[1] =  0x4c; /*'L'*/
	headerAddrStart[2] =  0x53; /*'S'*/
	headerAddrStart[3] =  0x01; /*'0x01'*/


	headerAddrStart[4] =  0x35; /*'Dlt base header config
	* Use extended header - true
	* MSBF - false
	* With ECU ID - true
	* With Seesion ID - false
	* With time stamp - true
	* version number -random
	'*/

	headerAddrStart[5] =  ActDltMessageCounter++; /*'Message counter value '*/

	/*TODO: - it must be fixed!!!! - Length*/
	headerAddrStart[6] =  0x00; /*Message length general*/
//	headerAddrStart[7] =  0x37; /*'Message length general '*/
	headerAddrStart[7]=28+size; /*General size */


	uint32_t TempEcuId = DLT_LOG_ECUID_VALUE;
	/*ECU ID*/
	headerAddrStart[8]= ((uint8_t*)&TempEcuId)[3];
	headerAddrStart[9]= ((uint8_t*)&TempEcuId)[2];
	headerAddrStart[10]= ((uint8_t*)&TempEcuId)[1];
	headerAddrStart[11]= ((uint8_t*)&TempEcuId)[0];

	/*Time stamp*/
	headerAddrStart[12]= ((uint8_t*)&ActualTime)[3];
	headerAddrStart[13]= ((uint8_t*)&ActualTime)[2];
	headerAddrStart[14]= ((uint8_t*)&ActualTime)[1];
	headerAddrStart[15]= ((uint8_t*)&ActualTime)[0];

	/*Extended header --verbose | type serial*/
//	headerAddrStart[16]= 0x41; /**/
	headerAddrStart[16]= (Level << 4) | 1;

	/*Number of arguments*/
	headerAddrStart[17]= 0x01; /**/

	/*App id */
	headerAddrStart[18]= ((uint8_t*)&AppId)[3];
	headerAddrStart[19]= ((uint8_t*)&AppId)[2];
	headerAddrStart[20]= ((uint8_t*)&AppId)[1];
	headerAddrStart[21]= ((uint8_t*)&AppId)[0];

	/*Contex ID  (4 bytes*/
	headerAddrStart[22]= ((uint8_t*)&ContextId)[3];
	headerAddrStart[23]= ((uint8_t*)&ContextId)[2];
	headerAddrStart[24]= ((uint8_t*)&ContextId)[1];
	headerAddrStart[25]= ((uint8_t*)&ContextId)[0];

	/*Type info*/
	headerAddrStart[26]= 0x01; /**/
	headerAddrStart[27]= 0x82; /**/
	headerAddrStart[28]= 0x00; /**/
	headerAddrStart[29]= 0x00; /**/

	/*Argument 1*/
	headerAddrStart[30]= size; 
	/* Size of the load in simplified form, please verify in documentation of DLT protcool documentation */
	headerAddrStart[31]= 0x00; /**/
}

/*
 ****************************************************************************************************
 * Exported functions implementation.
 * Descriptions are added in header file
 *****************************************************************************************************
 */

void DLTuc_RawDataReceiveDone(uint16_t Size)
{
	static uint8_t *MessageReceiveBufferAddress = NULL;
	uint8_t *MessageToRead_p = NULL;
	uint8_t MessageToReadSize= 0U;

	DLT_RB_Receive_GetNextMessageAddress(&BleMainReceiveRingBuffer,&MessageReceiveBufferAddress);
	if(ExtSerialRecDataFunctionCb != NULL){
		ExtSerialRecDataFunctionCb(MessageReceiveBufferAddress,DLT_REC_SINGLE_MESSAGE_MAX_SIZE);
	}

	/*
	* The receive buffer isn't handled fully correctly, it require deeper investigation.
	* However, it is possible to receive the Injection messages, and base commands if are transmitted with breakes..
	* Received DTL messages are divided by the "IDLE" irq for now.., not by the size and etc...
	*/
	if(DLT_RB_Receive_Read(&BleMainReceiveRingBuffer, &MessageToReadSize,&MessageToRead_p) == RB_OK)
	{
		if(MessageToRead_p[4] == 53) /**/
		{
			/*Use Extended, with EcuId, with Timestamp, end at[15]
			then Extended data as follow:
			[16] ==22-? - non verbose, type: control, MSTP: fatal
			[17] numOfArgs == 1 - TODO: Only 1 argument is for nowsupported!!!
			[18-21] - AppId
			[22-25] - ContexId
			[26-29] - ServiceId
			[30-33] - Size*/

			uint32_t AppId =         MessageToRead_p[21] << 24 |MessageToRead_p[20] << 16 | MessageToRead_p[19] << 8 | MessageToRead_p[18] << 0;
			uint32_t RecContexId =   MessageToRead_p[25] << 24 |MessageToRead_p[24] << 16 | MessageToRead_p[23] << 8 | MessageToRead_p[22] << 0;
			uint32_t RecServiceId =  MessageToRead_p[29] << 24 |MessageToRead_p[28] << 16 | MessageToRead_p[27] << 8 | MessageToRead_p[26] << 0;

			if(RecServiceId >= DLT_SERVICE_ID_CALLSW_CINJECTION)
			{
				if(NULL != ExtInfoInjectionDataRcvdCb)
				{
					/*MSB LSB, wtf..?, it is somehow mixed? */
					uint32_t DltDatSize =  MessageToRead_p[33] << 24 | MessageToRead_p[32] << 16 | MessageToRead_p[31] << 8 | MessageToRead_p[30] << 0;

					ExtInfoInjectionDataRcvdCb(AppId,RecContexId,RecServiceId,&MessageToRead_p[34],(uint16_t)DltDatSize);
				}
			}
			else if(RecServiceId == DLT_SERVICE_ID_SET_LOG_LEVEL)
			{
				uint32_t NewLogLevel =  MessageToRead_p[30];
				LOG("Set new log level request: %d How you triggered it?? , not supported", NewLogLevel);
				/* It is handled correctly by DLT Viewer?? */
			}
			else if(RecServiceId == DLT_SERVICE_ID_SET_DEFAULT_LOG_LEVEL)
			{
				uint32_t NewLogLevel =  MessageToRead_p[30];
				LOG("Set default log level request: %d", NewLogLevel);

				LOG("Not supported yet, I'm too lazy :)");
			}
			else if(DLT_SERVICE_ID_GET_SOFTWARE_VERSION == RecServiceId)
			{
				LOG("ECU_SW_VERSION: %d", DLT_ECU_SW_VER);
				/*TODO: The lib should send here answer of control message... */
			}
			else if(DLT_SERVICE_ID_GET_DEFAULT_LOG_LEVEL == RecServiceId)
			{
				LOG("Default log level: %d", DLT_LOG_ENABLE_LEVEL);
			}
		}
	}
}

void DLTuc_RegisterInjectionDataReceivedCb(
	void InjectionDataRcvd(uint32_t AppId, uint32_t ConId,uint32_t ServId,uint8_t *Data, uint16_t Size))
{
	ExtInfoInjectionDataRcvdCb = InjectionDataRcvd;
}

void DLTuc_RegisterReceiveSerialDataFunction(void LLSerialRecDataFunctionC(uint8_t *DltLogData, uint16_t Size))
{
	ExtSerialRecDataFunctionCb = LLSerialRecDataFunctionC;

	if(ExtSerialRecDataFunctionCb != NULL)
	{
		ExtSerialRecDataFunctionCb(&BluMainReceiveMessagesTab[0][0],DLT_REC_SINGLE_MESSAGE_MAX_SIZE);
	}
}

void DLTuc_RegisterTransmitSerialDataFunction(void LLSerialTrDataFunctionC(uint8_t *DltLogData, uint8_t Size))
{
	ExtSerialTrDataFunctionCb = LLSerialTrDataFunctionC;

	/*Preapre LOG DROP Info Log*/
	/* 0x444C5443 - In Ascii code it is: DLTC. It is more convience to put magic numbers here :) */
	PrepareDltHeader(DL_ERROR,0x444C5443, 0x444C5443,sizeof(DltLogDroppedInfo),DltLogDroppedInfoBuffer);
	for(int i=0; i<sizeof(DltLogDroppedInfo); i++)
	{
		DltLogDroppedInfoBuffer[i+DLT_ACT_HEADER_SIZE] = DltLogDroppedInfo[i];
	}

	DLtLogDroppedSize = DLT_ACT_HEADER_SIZE + sizeof(DltLogDroppedInfo);
}

void DLTuc_MessageTransmitDone(void)
{
	uint8_t TmpMessageSize=0;
	uint8_t *TmpMessagePointer = NULL;
	uint32_t ActualSysTime = 0u;

	if(GetSystemTimeMs != NULL)
	{
		ActualSysTime = GetSystemTimeMs();
	}

	if(LogDroppedFlag == true && (ActualSysTime - PrevLogDropSendTime > DLT_MINIMUM_LOG_DROP_PERIOD) )
	{
		/* If DLTuc will always send the DROP Message info,
		* then will not read any message from RB..*/
		PrevLogDropSendTime = ActualSysTime;
		LogDroppedFlag = false;

		if(ExtSerialTrDataFunctionCb != NULL)
		{
			ExtSerialTrDataFunctionCb(DltLogDroppedInfoBuffer, DLtLogDroppedSize);
		}
		return;
	}

	DLTuc_OS_CRITICAL_START();
	if(DLT_RB_TransmitRead(&DltTrsmtRingBuffer,&TmpMessageSize,&TmpMessagePointer) == RB_OK)
	{
		if(ExtSerialTrDataFunctionCb != NULL)
		{
			DLTuc_OS_CRITICAL_END();
			ExtSerialTrDataFunctionCb(TmpMessagePointer, TmpMessageSize);
		}
	}
	else
	{
		TransmitReadyStateFlag = true;
	}
	DLTuc_OS_CRITICAL_END();
}

void DLTuc_LogOutVarArgs(DltLogLevel_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...)
{
	uint16_t Size;                        /*  */
	uint8_t TmpMessageSize=0U;            /*  */
	uint8_t *TmpMessagePointer = NULL;    /*  */
	uint16_t writeIndex = 0U;             /*  */

	if(DLT_RB_GetNextWriteIndex(&DltTrsmtRingBuffer,&writeIndex) != RB_OK)
	{
		DLTuc_OS_CRITICAL_START();
		LogDroppedFlag = true;
		DLTuc_OS_CRITICAL_END();
	}
	else
	{
		/* Put the DLT message data directly in ring buffer*/
		va_list ap;                           /*  */
		va_start(ap, Payload);
		Size = vsprintf((char *)&(DltTrsmtMessagesTab[writeIndex][DLT_ACT_HEADER_SIZE]), (char *)Payload,ap);
		va_end(ap);

		Size += DLT_ACT_HEADER_SIZE;
		/*Add additional zeros on the end of message - thanks to that it work stable */
		Size++;
		DltTrsmtMessagesTab[writeIndex][Size] = 0U;
		Size++;
		DltTrsmtMessagesTab[writeIndex][Size] = 0U;

		PrepareDltHeader(Level,AppId,ContextId,Size,&(DltTrsmtMessagesTab[writeIndex][0]));

		Size = Size + DLT_ACT_HEADER_SIZE;
		DltTrsmtRingBuffer.dataSize[writeIndex] = Size;

		DLTuc_OS_CRITICAL_START();
		DltTrsmtRingBuffer.readyToTransmit[writeIndex] = true;
		DLTuc_OS_CRITICAL_END();
	}

	DLTuc_OS_CRITICAL_START();
	if(TransmitReadyStateFlag == true)
	{
		if(DLT_RB_TransmitRead(&DltTrsmtRingBuffer,&TmpMessageSize,&TmpMessagePointer) == RB_OK)
		{
			if(TmpMessageSize != 0U)
			{
				TransmitReadyStateFlag = false;
				DLTuc_OS_CRITICAL_END();
							/*Log transmission must be started in this contex...*/
							/***********************************************/
							/* It may be a bug in implementation - it must be investigated.. */
							/* It's important to be aware of this fact!!*/
							/***********************************************/
				if(ExtSerialTrDataFunctionCb != NULL)
				{
					ExtSerialTrDataFunctionCb(TmpMessagePointer, TmpMessageSize);
				}
				else
				{
					// while(1); /*Please Register the callback...*/
				}
			}
		}
	}
	DLTuc_OS_CRITICAL_END();
}

void DLTuc_RegisterGetTimeStampMsCallback(uint32_t GetSysTime(void))
{
	GetSystemTimeMs = GetSysTime;
}


