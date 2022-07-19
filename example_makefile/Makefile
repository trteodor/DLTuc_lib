###########################################################################
#To use this build system you have to add in your environment: (path)
# arm-none-eabi-gcc  
# OpenOCD
# ruby
# gcovr 5.0 or newer
# make 3.81 or newer
# gcc (for desktop)
# Maybe something else (but probably no)
###########################################################################

ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),))
helptest: help
endif
E :=
S := $E   $E
help:
	@echo     Call: make help \ \ \ -show this information
	@echo	  How Use this build system:
	@echo	  1. Call: make App  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ -To compile Default App - Firmware for MCU '('without debug information')'
	@echo	  2. Call: make debugC  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ -To remove build folder, compile Default App with Debug information and flash Firmware in MCU
	@echo	  3. Call: make debug  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ -To compile Default App with Debug information and flash Firmware in MCU
	@echo	  4. Call: make flash  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ -To compile and flash Default App  - Firmware for MCU '('without debug information')'
	@echo	  5. Call: make CompileAllApps  \ \ \ \  \ \ \ \ \ \ \ \ \ -To compile All Apps - Firmwares for MCU
	@echo	  6. Call: make CompileAllAppsDebug  \ \ \ \ \ \ \ \ \ -To compile All Apps with Debug information - Firmwares for MCU
	@echo	  7. Call: make CompileApp App=AppName \ \ \ \ \ \ -To compile Choosen App - Firmware for MCU '('AppName its Parameter')' '('without debug information')'
	@echo	  8. Call: make CompileAppflash App=AppName \ -To compile Choosen App and flash firmware in MCU '('AppName its Parameter')' '('without debug information')'
	@echo	  9. Call: make CompileAppDebug App=AppName \ -To compile Choosen App with Debug information and flash Firmware in MCU '('AppName its Parameter')'
	@echo	  ------------------------------------
	@echo	  10. Call: make test-ra \ \ \ \ \ \ -To start compilation or fully recompilation all unit-test
	@echo	  11. Call: make test-s \ \ \  \ \ \ -Only to rebuild source files and start unit tests
	@echo	  12. Call: make coverage \ \ \ \ \ -To refresh coverage report
	@echo	  ------------------------------------
	@echo	  13. Call: make clean-test \ \ \ -To delete all test result
	@echo	  14. Call: make clean-docs  \ \ \ -Remove Docs Folder
	@echo	  ------------------------------------
	@echo	  15. Call: make cleanR  \ \ \ \ \ \ \ \ -Remove build Release Folder
	@echo	  15. Call: make cleanD  \ \ \ \ \ \ \ \ -Remove build Debug Folder


ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),test test-ra test-s coverage clean-test clean-docs createmock))
include makeScripts/test-main.mk
else
include makeScripts/embedded-main.mk
endif
