

#define DLT_LOG_CONTEX           "CTX1"
#define DLT_LOG_APPID            "NUM1"


#include "DLTuc.h"
#include "nnRTOS.h"

void TaskContex1(void)
{
    LOG("Contex1 Start! ")


    while(1)
    {
    LOG("Hello Message From Contex1")
    nnOsThreadSleep(3);
    }


}