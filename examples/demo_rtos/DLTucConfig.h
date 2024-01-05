


#if 1 /*Set it to "1" to enable content*/

#ifndef __DLT_CONFIG__
#define __DLT_CONFIG__

#include "nnRTOS.h"

#define LOGS_ENABLE /*Comment this line to turn off all logs..*/


#define DLTuc_OS_CRITICAL_START()  nnENTRY_CRITCAL()
#define DLTuc_OS_CRITICAL_END()    nnLEAVE_CRITCAL()


/**!
* \brief DLT_LOG_ENABLE_LEVEL
* \details to define the minimum level log which will printed using the debug macros
*
*/
#ifndef DLT_LOG_ENABLE_LEVEL
#define DLT_LOG_ENABLE_LEVEL     DL_VERBOSE
#endif

/*This values - it's only example*/
#ifndef DLT_LOG_CONTEX
#define DLT_LOG_CONTEX      "TEST"
#endif
/**!
* \brief DLT_LOG_ENABLE_LEVEL
* \details to define the minimum level log which will printed using the debug macros
*
*/

#ifndef DLT_LOG_APPID
#define DLT_LOG_APPID      "1234"
#endif
/**!
* \brief DLT_LOG_ENABLE_LEVEL
* \details to define the minimum level log which will printed using the debug macros
*
*/

#define DLT_LOG_ECUID           "STF1" /*Electronic Controller Unit ID*/


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