#ifndef __NN_CONFIGURE_H__
#define __NN_CONFIGURE_H__

#include <stdint.h>
#include <stddef.h>

#define	QUANTA	1

#define MAX_NUM_OF_THREADS 4 //+1 for IDLE task.. 
#define STACKSIZE 500 //Stack Size for Each Task (in Words! X*4bytes)
/*Now it's 5*500*4 = 10kb RAM!!*/
#define BUS_FREQ		64000000



#endif /* __NN_CONFIGURE_H__ */