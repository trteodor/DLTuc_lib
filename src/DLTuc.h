/**
 * @file DLTuc.h
 * @author teodor
 * @date 1 Jul 2022
 * @brief This file is a part of DLTuc library
 *
 * In this header, you can find types, Api functions, which are provided by DLTuc library
 * Usefull macros, for convience usage of DLTuc library in logging purposes
 *
 * Requirments:
 * Around ~2kB of RAM
 * Check Configuration file and defines:
 * DLT_TRANSMIT_RING_BUFFER_SIZE, DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE
 *
 */

#ifndef INC_DLT_LOGS_MCU_H_
#define INC_DLT_LOGS_MCU_H_


#include "stdint.h"
#include <stdarg.h>
#include <string.h>

#include "DLTucConfig.h"

/*@brief 
 *
 *  - convert the To strings to uint32_t
 */
#define DLT_LOG_ECUID_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_ECUID[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_ECUID[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_ECUID[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_ECUID[3]))))


#define DLT_LOG_APPID_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_APPID[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_APPID[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_APPID[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_APPID[3]))))


#define DLT_LOG_CONTEX_VALUE	((uint32_t)((((uint32_t) ((uint8_t)DLT_LOG_CONTEX[0])) << 24UL) | \
						 (((uint32_t) ((uint8_t)DLT_LOG_CONTEX[1])) << 16UL) | \
					  (((uint32_t) ((uint8_t)DLT_LOG_CONTEX[2])) << 8UL) | \
				  ((uint32_t)((uint8_t)DLT_LOG_CONTEX[3]))))




/*
 * Definitions of DLT services.
 */
#define DLT_SERVICE_ID_SET_LOG_LEVEL                   0x01 /**< Service ID: Set log level */
#define DLT_SERVICE_ID_SETRACE_STATUS                  0x02 /**< Service ID: Set trace status */
#define DLT_SERVICE_ID_GET_LOG_INFO                    0x03 /**< Service ID: Get log info */
#define DLT_SERVICE_ID_GET_DEFAULT_LOG_LEVEL           0x04 /**< Service ID: Get dafault log level */
#define DLT_SERVICE_ID_STORE_CONFIG                    0x05 /**< Service ID: Store configuration */
#define DLT_SERVICE_ID_RESETO_FACTORY_DEFAULT          0x06 /**< Service ID: Reset to factory defaults */
#define DLT_SERVICE_ID_SET_COM_INTERFACE_STATUS        0x07 /**< Service ID: Set communication interface status */
#define DLT_SERVICE_ID_SET_COM_INTERFACE_MAX_BANDWIDTH 0x08 /**< Service ID: Set communication interface maximum bandwidth */
#define DLT_SERVICE_ID_SET_VERBOSE_MODE                0x09 /**< Service ID: Set verbose mode */
#define DLT_SERVICE_ID_SET_MESSAGE_FILTERING           0x0A /**< Service ID: Set message filtering */
#define DLT_SERVICE_ID_SETIMING_PACKETS                0x0B /**< Service ID: Set timing packets */
#define DLT_SERVICE_ID_GET_LOCALIME                    0x0C /**< Service ID: Get local time */
#define DLT_SERVICE_ID_USE_ECU_ID                      0x0D /**< Service ID: Use ECU id */
#define DLT_SERVICE_ID_USE_SESSION_ID                  0x0E /**< Service ID: Use session id */
#define DLT_SERVICE_ID_USEIMESTAMP                     0x0F /**< Service ID: Use timestamp */
#define DLT_SERVICE_ID_USE_EXTENDED_HEADER             0x10 /**< Service ID: Use extended header */
#define DLT_SERVICE_ID_SET_DEFAULT_LOG_LEVEL           0x11 /**< Service ID: Set default log level */
#define DLT_SERVICE_ID_SET_DEFAULTRACE_STATUS          0x12 /**< Service ID: Set default trace status */
#define DLT_SERVICE_ID_GET_SOFTWARE_VERSION            0x13 /**< Service ID: Get software version */
#define DLT_SERVICE_ID_MESSAGE_BUFFER_OVERFLOW         0x14 /**< Service ID: Message buffer overflow */
#define DLT_SERVICE_ID_CALLSW_CINJECTION               0xFFF /**< Service ID: Message Injection (minimal ID) */


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
 * \brief DLTuc_RawDataReceiveDone
 * \details OPTIONAL - Use only if you need handle data receive, like Injection messages
 * Call the function to inform DLTuc that Raw data packet has been received
 ************************************************************************************************/
void DLTuc_RawDataReceiveDone(uint16_t Size);

/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterReceiveSerialDataFunction
 * \details OPTIONAL - Use only if you need handle data receive, like Injection messages
 * - Register callback function which will be used by DLTuc to start data receive process.
 * The Callback function is basicly called only after registration and if data packet has been received,
 * to start again receive data packet
 ************************************************************************************************/
void DLTuc_RegisterReceiveSerialDataFunction(void LLSerialRecDataFunctionC(uint8_t *DltLogData, uint16_t Size));

/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterInjectionDataReceivedCb
 * \details OPTIONAL - Use only if you need handle data receive, like Injection messages
 * Register injection data received call back
 * Use the function if you want register call back function to handle the Injection messages in your application
 ************************************************************************************************/
void DLTuc_RegisterInjectionDataReceivedCb
		(void InjectionDataRcvd(uint32_t AppId, uint32_t ConId,uint32_t ServId,uint8_t *Data, uint16_t Size));

/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterTransmitSerialDataFunction
 * \details This simple stack/library must be initialized by "DLTuc_RegisterTransmitSerialDataFunction"
 *          As a parameter must be passed function which will transmit serial data
 * \param in LLSerialTrDataFunctionC transmit function pointer
 ************************************************************************************************/
void DLTuc_RegisterTransmitSerialDataFunction(void LLSerialTrDataFunctionC(uint8_t *DltLogData, uint8_t Size));

/*!
 ************************************************************************************************
 * \brief DLTuc_RegisterGetTimeStampMsCallback
 * \details function to update time stamp in library
* \param in GetSysTime - pointer to function which allow to read system time in msec
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



/*Additional macros for convience use the library..*/
#ifdef LOGS_ENABLE

/**!
 * \brief LOGL(level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have to add
 * log_level, DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 *
 * */
#define LOG(str, ...)\
	if(DL_INFO <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(DL_INFO, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) str, ##__VA_ARGS__);\
	}

/**!
 * \brief LOGL(level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have to add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 *
 * */
#define LOGL(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) str, ##__VA_ARGS__);\
	}

/**!
 * @brief LOGF(log_level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have to add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function
 *
 *
 * */
#define LOGF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *) "FUN:%s LOG: "str, __FUNCTION__,##__VA_ARGS__);\
	}

/**!
 * \brief LOGFF(log_level, str, ...)
 * \details Transmit DltLog using function DLTuc_LogOutVarArgs but user don't have to add DLT_LOG_APPID_VALUE and DLT_LOG_CONTEX_VALUE
 * Additionally add the name of the calling function and file name
 *
 *
 * */
#define LOGFF(log_level, str, ...)\
	if(log_level <= DLT_LOG_ENABLE_LEVEL){\
		DLTuc_LogOutVarArgs(log_level, DLT_LOG_APPID_VALUE, DLT_LOG_CONTEX_VALUE,(uint8_t *)"FILE:%s LINE: %d FUN:%s LOG: "str,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);\
	}

#else
LOG(str, ...)
LOGL(log_level, str, ...)
LOGF(log_level, str, ...)
LOGFF(log_level, str, ...)
#endif

#endif /* INC_DLT_LOGS_MCU_H_ */
