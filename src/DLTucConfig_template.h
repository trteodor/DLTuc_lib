/**
 * @file DLTuc.c
 * @author teodor
 * @date 1 Jul 2022
 * @brief This file is a part of DLTuc library
 *
 * In this header file is placed DLTuc configuration template
 *
 * DLTuc Requirments:
 * Around ~2kB of RAM
 * Check Configuration file and defines:
 * DLT_TRANSMIT_RING_BUFFER_SIZE, DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE
 *
 */

#if 0 /*Set it to "1" to enable content*/

#ifndef __DLT_CONFIG__
#define __DLT_CONFIG__

/*
 * *******************************************************************************************
 * Exported config defines
 * *******************************************************************************************
 * */

/*Comment the line below to turn off all logs..*/
#define LOGS_ENABLE

/**!
* \brief DLTuc_OS_CRITICAL_START & DLTuc_OS_CRITICAL_END
* \details Pass here "critical entry" function for your RTOS or
* irq block function. If you want use the library from diffrent contexts
*/
#define DLTuc_OS_CRITICAL_START()
#define DLTuc_OS_CRITICAL_END()

/**!
* \brief DLT_LOG_ENABLE_LEVEL
* \details Default minimum log level to transmit the log
*/
#ifndef DLT_LOG_ENABLE_LEVEL
#define DLT_LOG_ENABLE_LEVEL     DL_VERBOSE
#endif

/**!
* \brief DLT_LOG_CONTEX
* \details define dlt log contex
*/
#ifndef DLT_LOG_CONTEX
#define DLT_LOG_CONTEX      "DFLT"
#endif

/**!
* \brief DLT_LOG_APPID
* \details define appid
*/
#ifndef DLT_LOG_APPID
#define DLT_LOG_APPID      "0000"
#endif

/**!
* \brief DLT_LOG_ECUID
* \details define here your ECUID ..
*/
#define DLT_LOG_ECUID           "uCID" /*Electronic Controller Unit ID*/

/**!
* \brief DLT_ECU_SW_VER
* \details define sw version
*/
#define DLT_ECU_SW_VER          0001

/**
* \brief DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE & DLT_TRANSMIT_RING_BUFFER_SIZE
* \details these values are used to define the size of the circular buffer and the maximum size of a single DLT message.
* Size of out Circular DLT messages buffer is equal: DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE*DLT_TRANSMIT_RING_BUFFER_SIZE bytes
*/
#define DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE 255
#define DLT_TRANSMIT_RING_BUFFER_SIZE 15

#define DLT_REC_SINGLE_MESSAGE_MAX_SIZE      255
#define DLT_RECEIVE_RING_BUFFER_SIZE     2

#endif  //__DLT_CONFIG__

#endif //Content enable