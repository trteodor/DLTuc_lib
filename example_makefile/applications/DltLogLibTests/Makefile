


SRC +=  common/Src/RCC_Config.c 
SRC +=  common/Src/GPIO_fun.c 
SRC +=  common/Src/USART_UART_DMA.c 
SRC +=  common/Src/SystemConfig.c 
SRC +=  common/Src/RTC.c 
SRC +=  applications/DltLogLibTests/Src/main.c 

ASM += common/startup_stm32f103xb.s

INC_DIR += -Icommon/Inc 
INC_DIR += -Iapplications/DltLogLibTests/Inc 
INC_DIR += -IDrivers/CMSIS/Device/ST/STM32F1xx/Include 
INC_DIR += -IDrivers/CMSIS/Include

#Add DltLibFiles
INC_DIR += -I../ucDLTlibFiles
SRC +=  ./../ucDLTlibFiles/ucDLTlib.c

# link script
LDSCRIPT = common/STM32F103RBTx_FLASH.ld