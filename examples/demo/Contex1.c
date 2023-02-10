

#define DLT_LOG_CONTEX           "CTX1"
#define DLT_LOG_APPID            "NUM1"
#define DLT_LOG_ENABLE_LEVEL     DL_INFO

#include "DLTuc.h"
#include "nnRTOS.h"

void TaskContex1(void)
{
    LOG("Contex1 Start! ")


    while(1)
    {
    LOG("Hello Crazy World")
    LOGFF(DL_DEBUG,"Missed log! Check definition on top of the file: DLT_LOG_ENABLE_LEVEL")
    LOGFF(DL_FATAL,"I belive you care about yourself :) ")
    LOGFF(DL_FATAL,"I dk What we log here more")
    nnOsThreadSleep(3000);
    }
}