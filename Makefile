
all:
	@echo "Please change directory using following command:"
	@echo  \ cd example_makefile
	@echo    \ \ \ \ \ \  And call again: '|'make'|'  to view '|'help view'|'
	@echo    But.. after change directory I suggest you for this example use only: '|'make debug'|' command to rebuild firmware and flash to Nucleo-STM32F103RB board
	@echo Or just call: '|'make debug'|'
	@echo     \ \ For: '|'example_makefile'|' main file is in: example_makefile/applications/Src/main.c


debug:
	cd example_makefile && make CompileAppDebug App=DltLogLibTests

