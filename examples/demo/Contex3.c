#define DLT_LOG_CONTEX           "CTX3"
#define DLT_LOG_APPID            "NUM3"


#include "DLTuc.h"
#include "nnRTOS.h"

void TaskContex3(void)
{
    LOG("Contex3 Start! ")

    while(1)
    {
    LOG("Hello Message From Contex3")
    LOGFF(DL_FATAL,"Some more extra messages!")
    LOGFF(DL_FATAL,"I belive you care about yourself :) ")
    nnOsThreadSleep(4000);
    }

}