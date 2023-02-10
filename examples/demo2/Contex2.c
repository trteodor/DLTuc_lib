#define DLT_LOG_CONTEX           "CTX2"
#define DLT_LOG_APPID            "NUM2"


#include "DLTuc.h"
#include "nnRTOS.h"

void TaskContex2(void)
{
    LOG("Contex2 Start! ")

    while(1)
    {
    LOG("Hello Message From Contex2")
    nnOsThreadSleep(7);
    }

}