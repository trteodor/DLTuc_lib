/**
 * @file DLTuc.h
 * @author teodor
 * @date 1 Jul 2022
 * @brief This file is a part of DLTuc library
 *
 * In this header you can find types, Api function provided by DLTuc library, 
 * and usefull macros for convience use DLTuc library in logging purposes
 */

#ifndef INC_DLT_LOGS_MCU_H_
#define INC_DLT_LOGS_MCU_H_


#include "stdint.h"
#include <stdarg.h>
#include <string.h>

#include "DLTucConfig.h"


/**!
 * \brief DltLogLevel_t 
 * \details Typdef used to identify the DLT log level
 * */
typedef enum
{
	DL_FATAL   =1,
	DL_ERROR   =2,
	DL_WARN    =3,
	DL_INFO    =4,
	DL_DEBUG   =5,
	DL_VERBOSE =6,
}DltLogLevel_t;



/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterTransmitSerialDataCallback
 * \details This simple stack/library must be initialized by "DLTuc_RegisterTransmitSerialDataCallback"
 *          As a parameter must be pass function which will transmit serial data
 * \param in LLSerialTrDataFunctionC transmit function pointer
 
 ************************************************************************************************/
void DLTuc_RegisterTransmitSerialDataCallback(void LLSerialTrDataFunctionC(uint8_t *DltLogData, uint8_t Size));

/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterGetTimeStampMsCallback
 * \details function to update time stamp in library
* \param in GetSysTime - transmit function pointer
 * \param Time - system time in ms
 *************************************************************************************************/
void DLTuc_RegisterGetTimeStampMsCallback(uint32_t GetSysTime(void));


/*!
 ************************************************************************************************
 * \brief DLTuc_MessageTransmitDone
 * \details IMPORTANT!!!!!
 *  Call this function when the transsmision is end
 *  For example in "DMA transmission end callback" to inform the lib that the message is transmitted
 ************************************************************************************************
 * */
void DLTuc_MessageTransmitDone(void);

/*!
 ************************************************************************************************
 * \brief DLTuc_LogOutVarArgs
 * \details default function to create DLT Log
 * \param DltLogLevel_t Level - of Dlt log
 * \param in AppId - size of the "DltLogData" (return value)
 * \param in ContextId - pointer to the message stored in RingBuffer (return value)
 * \param in Payload String to send as dlt log
 * \param in ... parameters same as in printf function
 *************************************************************************************************/
void DLTuc_LogOutVarArgs(DltLogLevel_t Level, uint32_t AppId, uint32_t ContextId, uint8_t *Payload, ...);




/*Additional macros for convience use the library.. start*/
#ifdef LOGS_ENABLE

/**!
 * \brief LOGL(level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add 
 * log_level, DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 *
 * */
#define LOG(str, ...)\
	if(DL_INFO <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(DL_INFO, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) str, ##__VA_ARGS__);\
	}

/**!
 * \brief LOGL(level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 *
 * */
#define LOGL(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) str, ##__VA_ARGS__);\
	}

/**!
 * @brief LOGF(log_level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function
 *
 *
 * */
#define LOGF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) "FUN:%s() LOG: "str, __FUNCTION__,##__VA_ARGS__);\
	}

/**!
 * \brief LOGFF(log_level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function and file name
 *
 *
 * */
#define LOGFF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *)"FILE:%s() FUN:%s() LOG: "str,__FILE__,__FUNCTION__,##__VA_ARGS__);\
	}

#else
LOG(str, ...)
LOGL(log_level, str, ...)
LOGF(log_level, str, ...)
LOGFF(log_level, str, ...)
#endif

#endif /* INC_DLT_LOGS_MCU_H_ */
