#define DLT_LOG_CONTEX           "CTX2"
#define DLT_LOG_APPID            "NUM2"


#include "DLTuc.h"
#include "nnRTOS.h"

void TaskContex2(void)
{
    LOG("Contex2 Start! ")

    while(1)
    {
    LOGL(DL_ERROR,  "Hello DLT Again Arg1 %d Arg2 :%d" , 2565, 56);
    LOGFF(DL_FATAL, "GENERALLY DLT Again1");
    LOGF(DL_DEBUG,  "AnotherTest DLT Again");
    LOG("AnotherTest2 DLT Again");
    LOGL(DL_WARN, "Orange is sweet fruit");
    nnOsThreadSleep(2000);
    }

}