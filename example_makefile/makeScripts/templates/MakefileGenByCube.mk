TARGET = STM32F1_DisturbancesTests


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
C_SOURCES =  \
Core/Src/contables.c \
Core/Src/printf.c \
Core/Src/PrintToBuf.c \
Core/Src/RCC_Config.c \
Core/Src/USART_UART_DMA.c \
Core/Src/GPIO_fun.c \
Core/Src/main.c \

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xb.s


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
-DSTM32F103xB \
# -DUSE_HAL_DRIVER \
ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),debug build-all))

C_DEFS += -DDEBUG

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),debug build-all))

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

# default action: help
help:
	@echo     STM32F103RB Simple BuildSystem
	@echo     Call: make help \ \ \ \ \ \ \  \ \ -show this information
	@echo	  1. call: make build-all  \ \ -To compile output files for Microcontroller in debug mode - with uart logs
	@echo	  2. call: make debug  \ \ \ \ \ \ -To compile output files in debug mode and upload in MCU - in this mode uart debug logs normally are activ
	@echo	  2. call: make flash  \ \ \ \ \ \ -To compile output files in release mode and upload in MCU - without debug logs 
	@echo	  2. call: make reset  \ \ \ \ \ \ -To reset MCU\	
	@echo     3. call: make clean  \ \ \ \ \  -Remove build Folder


#######################################
# clean up
#######################################


.PHONY: clean    
clean:
	-rm -fR $(BUILD_DIR)
 
#######################################
# dependencies
#######################################

#Simple Flash
#To use it You have to add openocd in your environemnt path

debug: flash

flash:
	openocd \
	-f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
	-c "init" -c "reset halt" \
	-c "flash write_image erase build/STM32F1_DisturbancesTests.bin 0x8000000" \
	-c "verify_image build/STM32F1_DisturbancesTests.bin" -c "reset" -c "shutdown"

.PHONY : flash

#Simple reset
reset:
	openocd \
	-f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "init" -c "reset" -c "shutdown"



build-all: clean $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


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
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		




-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***