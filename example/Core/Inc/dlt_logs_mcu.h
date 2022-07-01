/*
 * dlt_logs_mcu.h
 *
 *  Created on: 1 lip 2022
 *      Author: teodor
 */

#ifndef INC_DLT_LOGS_MCU_H_
#define INC_DLT_LOGS_MCU_H_


#include "stdint.h"


#include <stdarg.h>
#include <string.h>




/*Debug dlt log level macros start
 *
 * */
#define DL_FATALd   0x01
#define DL_ERRORd  0x02
#define DL_WARNd    0x03
#define DL_INFOd    0x04
#define DL_DEBUGd   0x05
#define DL_VERBOSEd 0x06
/*Debug dlt log level macros end
 *
 * */

typedef enum
{
	DL_FATAL   = 1,
	DL_ERROR   =2,
	DL_WARN    =3,
	DL_INFO    =4,
	DL_DEBUG   =5,
	DL_VERBOSE =6,
}DltLogLevel_t;


/*
 ****************************************************************************************************
 * M_DLT Configuration defines start
 * SECTION START
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */
#define DLT_LOG_ECUID           "STF1" /*Electronic Controller Unit ID*/

/*@brief DLT_LOG_ECUID_VALUE
 *
 *  - convert the ECUID string to uint32_t
 */
#define DLT_LOG_ECUID_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_ECUID[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_ECUID[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_ECUID[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_ECUID[3]))))


/*
* @brief DLT_ACT_HOLE_HEADER_SIZE hole header sum
* Standard Header + Extended + TypeInfo
* Don't modify it if you aren't sure what are you doing!!!
*/
#define DLT_ACT_HOLE_HEADER_SIZE 32

/*
*@brief DLT_MAX_SINGLE_MESSAGE_SIZE & DLT_RING_BUFFER_SIZE
* these values define the size of the circular buffer and the maximum size of a single DLT message
* Size of out Circular DLT messages buffer is equal: DLT_MAX_SINGLE_MESSAGE_SIZE*DLT_RING_BUFFER_SIZE
*/
#define DLT_MAX_SINGLE_MESSAGE_SIZE 255
#define DLT_RING_BUFFER_SIZE 15


/*
 ****************************************************************************************************
 * API Function prototypes section START
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */
/*DLTuc - Data Log Trace microcontroler "u" micro*/

/*
 *@brief DLTuc_RegisterTransmitSerialDataCallback
 * IMPORTANT!!!!!
 *  This simple stack/library must have initialized by "DLTuc_RegisterTransmitSerialDataCallback"
 *  As a parameter must by pass function which will transmit serial data
 *
 * */
void DLTuc_RegisterTransmitSerialDataCallback(void LLSerialTrDataFunctionC(uint8_t *DltLogData, uint8_t Size));

 /*
  *@brief DLTuc_MessageTransmitDone
  * IMPORTANT!!!!!
  *  Call this function when the transsmision is end
  * For example in "DMA transmission end callback" to inform the lib that the message is transmitted
  *
  * */
void DLTuc_MessageTransmitDone(void);


/*
 *@brief DLTuc_UpdateTimeStampMs
 *
 *
 * */
void DLTuc_UpdateTimeStampMs(uint32_t Time);



/*
 ****************************************************************************************************
 * Api functions
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */

/*
 * @brief DLTuc_LogOut
 *
 * Simple function to send DLT LOG
 *
 * */
void DLTuc_LogOut(DltLogLevel_t Level, uint32_t AppId, uint32_t ContextId, uint8_t data[],uint16_t size);


/*
 * @brief DLTuc_LogOutVarArgs(uint8_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...);
 *
 * Typical most usefull function to create DLT Log
 *
 * */
void DLTuc_LogOutVarArgs(DltLogLevel_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...); /*Typical most usefull DLT log function*/


/*
 ****************************************************************************************************
 * FEW USEFULL MACROS WHICH CAN BE USE FULL TO CRATE DEBUG LOGS
 * SECTION START
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */
/*
* @brief DLT_LOG_ENABLE_LEVEL
* to define the minimum level log which will printed using the debug macros
*
*/

#define DLT_LOG_ENABLE_LEVEL     DL_VERBOSE


/*This values - it's only example*/
#define DLT_LOG_CONTEX      "TEST"
#define DLT_LOG_APPID      "1234"

#define DLT_LOG_APPID_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_APPID[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_APPID[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_APPID[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_APPID[3]))))




#define DLT_LOG_CONTEX_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_CONTEX[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_CONTEX[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_CONTEX[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_CONTEX[3]))))





/*
 * @brief DEBUGL(level, str, ...)
 *
 * Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 *
 * */
#define DEBUGL(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) str, ##__VA_ARGS__);\
	}

/*
 * @brief DEBUGF(log_level, str, ...)
 *
 * Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function
 *
 *
 * */
#define DEBUGF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) "FUN:%s() "str, __FUNCTION__,##__VA_ARGS__);\
	}

/*
 * @brief DEBUGFF(log_level, str, ...)
 *
 * Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function and file name
 *
 *
 * */
#define DEBUGFF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *)"FUN:%s() FILE:%s()  "str, __FUNCTION__,__FILE__,##__VA_ARGS__);\
	}

/*
 ****************************************************************************************************
 * FEW USEFULL MACROS WHICH SHOULD USE FULL TO CRATE DEBUG LOGS END
 *****************************************************************************************************
 *****************************************************************************************************
 *
 * */


#endif /* INC_DLT_LOGS_MCU_H_ */
