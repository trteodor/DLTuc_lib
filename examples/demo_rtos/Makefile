##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.17.1] date: [Fri Feb 10 10:04:33 CET 2023]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = STM32F1_DLTuc_Simple


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES += main.c
C_SOURCES += Contex1.c
C_SOURCES += Contex2.c
C_SOURCES += Contex3.c
C_SOURCES += ../Drivers/GPIO_f1/GPIO_fun.c 
C_SOURCES += ../Drivers/uart_dma/USART_UART_DMA.c 
C_SOURCES += ../Drivers/System/RCC_Config.c 
C_SOURCES += ../Drivers/RTC/RTC.c 
#Add DltLibFiles
C_SOURCES +=  ../../src/DLTuc.c

#nnRTOS sources
C_SOURCES += ../nnRtos/src/nnKernel.c
C_SOURCES += ../nnRtos/port/CM3F/port.c
C_SOURCES += ../nnRtos/src/semaphores.c
C_SOURCES += ../nnRtos/src/queues.c
#nnRTOS asm file



# ASM sources
ASM_SOURCES += startup_stm32f103xb.s
ASM_SOURCES += ../nnRtos/port/CM3F/portAsm.s
#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32F103xB


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES += -I../demo_rtos/
C_INCLUDES += -I../Drivers/CMSIS/Core/Include
C_INCLUDES += -I../Drivers/CMSIS/Include
C_INCLUDES += -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include
C_INCLUDES += -I../Drivers/System
C_INCLUDES += -I../Drivers/RTC
C_INCLUDES += -I../Drivers/uart_dma
C_INCLUDES += -I../Drivers/GPIO_f1
#Add DltLibFiles
C_INCLUDES += -I../../src

C_INCLUDES += -I../nnRtos
C_INCLUDES += -I../nnRtos/inc
C_INCLUDES += -I../nnRtos/port/CM3F
C_INCLUDES += -I../common/Inc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RBTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin flash


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	@echo CC $<

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(AS) -c $(CFLAGS) $< -o $@
	@echo AS $<

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo Linking...
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash board
#######################################
flash:
	openocd \
	-f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "init" -c "reset halt" \
	-c "flash write_image erase $(BUILD_DIR)/$(TARGET).bin 0x8000000" \
	-c "verify_image $(BUILD_DIR)/$(TARGET).bin" -c "reset" -c "shutdown"


#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***