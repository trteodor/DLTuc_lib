#ifndef __NN_PORT_MACROS_H__
#define __NN_PORT_MACROS_H__

#define nnENTRY_CRITCAL() __disable_irq() 
#define nnLEAVE_CRITCAL() __enable_irq()

#endif