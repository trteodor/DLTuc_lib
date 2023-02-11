default: startMessage simple infoMessage

startMessage:
	@echo Compiling and Flashing simple example..
	@echo --------------------------------

infoMessage:
	@echo --------------------------------
	@echo --------------------------------
	@echo possible another examples: simple \ demo_rtos \ performance_test
	@echo check directory: examples
	@echo to compile and program/flash board using another than default example just call:
	@echo make demo_rtos
	@echo or: make performance_test

demo_rtos:
	cd examples/demo_rtos && make

#Compile and flash simple example
simple:
	cd examples/simple && make

#Compile and flash simple example
performance_test:
	cd examples/performanceTestRtos && make

