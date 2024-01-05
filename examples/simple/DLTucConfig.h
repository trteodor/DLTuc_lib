/**
 * @file DLTuc.c
 * @author teodor
 * @date 1 Jul 2022
 * @brief This file is a part of DLTuc library
 *
 * In this header file is placed DLTuc configuration template
 *
 */

#if 1 /*Set it to "1" to enable content*/

#ifndef __DLT_CONFIG__

/*Comment this line to turn off all logs..*/
#define LOGS_ENABLE 

/*Pass here the entry critical entry function for your RTOS or
  Interrupt block functions if you want use the library from diffrent context*/
#define DLTuc_OS_CRITICAL_START()
#define DLTuc_OS_CRITICAL_END()


/*Default minimum log level to transmit the log*/
#ifndef DLT_LOG_ENABLE_LEVEL
#define DLT_LOG_ENABLE_LEVEL     DL_VERBOSE
#endif

/**!
* \brief DLT_LOG_CONTEX
* \details to define the minimum level log which will printed using the debug macros
*
*/
#ifndef DLT_LOG_CONTEX
#define DLT_LOG_CONTEX      "DFLT"
#endif

/**!
* \brief DLT_LOG_APPID
* \details to define the minimum level log which will printed using the debug macros
*
*/
#ifndef DLT_LOG_APPID
#define DLT_LOG_APPID      "0000"
#endif

/**!
* \brief DLT_LOG_ECUID
* \details you can define here you ECUID ..
*
*/
#define DLT_LOG_ECUID           "uCID" /*Electronic Controller Unit ID*/

#define DLT_ECU_SW_VER          0001

/*
*@brief DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE & DLT_TRANSMIT_RING_BUFFER_SIZE
* these values define the size of the circular buffer and the maximum size of a single DLT message
* Size of out Circular DLT messages buffer is equal: DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE*DLT_TRANSMIT_RING_BUFFER_SIZE
*/
#define DLT_TRANSMIT_MAX_SINGLE_MESSAGE_SIZE 255
#define DLT_TRANSMIT_RING_BUFFER_SIZE 15


#define DLT_REC_SINGLE_MESSAGE_MAX_SIZE      255
#define DLT_RECEIVE_RING_BUFFER_SIZE     2

#endif  //__DLT_CONFIG__

#endif //Content enable