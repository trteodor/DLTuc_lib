default: startMessage demo infoMessage

startMessage:
	@echo Compiling and Flashing demo example..
	@echo --------------------------------

infoMessage:
	@echo --------------------------------
	@echo --------------------------------
	@echo possible another examples: demo simple perf_test ...
	@echo check directory: examples
	@echo to compile and flash another than default example just call:
	@echo make simple
	@echo or: make performance_test

demo:
	cd examples/demo && make

#Compile and flash simple example
simple:
	cd examples/simple && make

#Compile and flash simple example
performance_test:
	cd examples/performanceTestRtos && make

