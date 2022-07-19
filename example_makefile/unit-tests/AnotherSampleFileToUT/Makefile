

SRC+= unit-tests/AnotherSampleFileToUT/test_ASFile.c   
SRC+= unit-tests/AnotherSampleFileToUT/test_runnerASFile.c 
SRC+= unit-tests/AnotherSampleFileToUT/test_main.c 


SRC+= applications/MCU_CoreWFI/Src/AnotherSampleFile.c


INC_DIR+= -Iapplications/MCU_CoreWFI/Inc/ 

 

COVERAGE_SOURCEDIR :=
COVERAGE_SOURCEDIR += applications/MCU_CoreWFI/Src


MOCKED_HEADER +=  applications/MCU_CoreWFI/Inc/AnotherSampleFile.h
MOCKED_HEADER +=  applications/MCU_CoreWFI/Inc/SampleFileToUT.h


MOCKED_SOURCE +=  $(BUILD_DIR_UT)/cmock/mock_SampleFileToUT.c


SRC+= $(MOCKED_SOURCE)
