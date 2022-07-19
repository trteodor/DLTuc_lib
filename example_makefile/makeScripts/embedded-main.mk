#Simple multi embedded App build system Template created by Teodor R.
# Created By Teodor Rosolowski 02.2022 trteodor@gmail.com

include makeScripts/config_embedded.mk


################################################################################################
################################################################################################
#PARSE INPUT
################################################################################################
ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),CompileAllApps CompileAllAppsDebug))

include applications/makefile

else

App ?= $(DEFAULT_App)

endif

################################################################################################
ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),CompileAllAppsDebug CompileAppDebug debug  debugC cleanD))

C_DEFS += -DDEBUG

BUILD_DIR = .build_debug

else

BUILD_DIR = .build_release

endif

ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),debug debugC CompileAppDebug CompileAllAppsDebug ))
##############################################################################
#FLAGS TO ADD DEBUG INFORMATION IN OUT FILES
##############################################################################
CFLAGS += -g -gdwarf-2

endif
################################################################################################

OBJECTS_FOLDERS := $(addprefix $(BUILD_DIR)/CompilationFiles/,$(App))
OUT_PUT_Folder := $(addprefix $(BUILD_DIR)/CompilationOutputFiles/,$(App))

H_INC_DIR :=

ExecuteActualAppInfo :=
ExecuteCompilation_C_Files :=
ExecuteCompilation_ASM_Files :=

#SCRIPT START
################################################################################################
define COLLECT_PROGRAM_SOURCES

ExecuteActualAppInfo += $(1)

SRC :=
INC_DIR :=
OBJECTS :=
H_INC_DIR :=
ASM :=

-include $$(wildcard $$(BUILD_DIR)/CompilationFiles/$(1)/*.d)

-include applications/$(1)/Makefile


ASMOBJECTS = $$(addprefix $$(BUILD_DIR)/CompilationFiles/$(1)/,$$(notdir $$(ASM:.s=.o)))
vpath %.c $$(sort $$(dir $$(ASM)))

OBJECTS = $$(addprefix $$(BUILD_DIR)/CompilationFiles/$(1)/,$$(notdir $$(SRC:.c=.o)))
vpath %.c $$(sort $$(dir $$(SRC)))

H_INC_DIR := $$(INC_DIR)

endef

################################################################################################
#LinkingProcedure



define LinkingProcedure
ExecuteActualAppInfo += $(HelperVariable)/$(1)
ExecuteCompilation_C_Files += $(HelperVariable)/$(1)
ExecuteCompilation_ASM_Files += $(HelperVariable)/$(1)

$(HelperVariable)/$(1): $$(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(ASMOBJECTS) $$(H_INC_DIR) $$(LDFLAGS) -Wl,-Map=$$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).map,--cref -Wl,--gc-sections \
	 -o $$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).elf
	$(HEX) $$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).elf $$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).hex
	$(BIN) $$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).elf $$(BUILD_DIR)/CompilationOutputFiles$$@/$$(@F).bin
	@echo ---------------------------------------------------------------------------------------------------------
	@echo -------------------Compilation Succesfull----------------------------------------------------------------
	@echo ---------------------------------------------------------------------------------------------------------
	@echo \ \ \ \ \ \ \ Project: \ \   $$(@F)
	@echo ---------------------------------------------------------------------------------------------------------

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	

endef #LinkingProcedure


########################################################################################################################
define ExecuteCompilation_ASM_FilesDefine
ExecuteCompilation_ASM_Files += $(1) $(2)
$(1):
$(2): $(1)
	$(AS) -c $$(CFLAGS) $(H_INC_DIR) $$< -o $$@

endef

########################################################################################################################
define ExecuteCompilation_C_FilesDefine
ExecuteCompilation_C_Files += $(1) $(2)

$(1):
$(2): $(1)	
	$(CC) -c $$(CFLAGS) $(H_INC_DIR) -Wa,-a,-ad,-alms=$$(@:.o=.lst)  $$< -o $$@

endef
########################################################################################################################

#LOOPS
########################################################################################################################
define CompilationAsmFilesLoop
$(foreach ASM,$(ASM),$(eval $(call ExecuteCompilation_ASM_FilesDefine,$(ASM),$$(ASMOBJECTS))))
endef
########################################################################################################################
define CompilationCSourcesLoop
$(foreach SRC,$(SRC),$(eval $(call ExecuteCompilation_C_FilesDefine,$$(SRC),$$(OBJECTS) )))
endef
########################################################################################################################

.PHONY : AppListHeaderStart
AppListHeaderStart:
	@echo -------------------------App List---------------------------------

.PHONY : AppListHeaderEnd
AppListHeaderEnd:
	@echo -----------------------------------------------------------------

#Actual App Name



$(foreach App,$(App),$(eval $(call COLLECT_PROGRAM_SOURCES,$(App))) \
$(eval $(call CompilationAsmFilesLoop,$$(App))) \
$(eval $(call CompilationCSourcesLoop,$$(App))) \
$(eval $(call LinkingProcedure,$$(App))) )



#Targets
########################################################################################################################
#TODO - I dont need this target!!!! its fake
$(HelperVariable):

$(BUILD_DIR): $(HelperVariable)
	mkdir -p $(BUILD_DIR)

$(OBJECTS_FOLDERS):
	@mkdir -p $@

$(OUT_PUT_Folder):
	@mkdir -p $@

$(App):
	@echo Application to Compile: $@
	@echo ------------------


#######################################
#Targets:

CompileApp: $(App) $(OUT_PUT_Folder) $(OBJECTS_FOLDERS) $(ExecuteCompilation_ASM_Files) $(ExecuteCompilation_C_Files)


cleanR:
	-rm -fR $(BUILD_DIR)

cleanD:
	-rm -fR $(BUILD_DIR)

App: CompileApp

debug: flash

debugC: cleanD flash #the same target because upper is added special compile flag

flashC: cleanR flash

flash: CompileApp
	openocd \
	-f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "init" -c "reset halt" \
	-c "flash write_image erase $(BUILD_DIR)/CompilationOutputFiles/$(App)/$(App).bin 0x8000000" \
	-c "verify_image $(BUILD_DIR)/CompilationOutputFiles/$(App)/$(App).bin" -c "reset" -c "shutdown"

.PHONY : flash

#Simple reset
reset:
	openocd \
	-f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "init" -c "reset" -c "shutdown"

CompileAllApps: CompileApp

CompileAllAppsDebug: CompileApp

CompileAppflash: flash

CompileAppDebug: flash









