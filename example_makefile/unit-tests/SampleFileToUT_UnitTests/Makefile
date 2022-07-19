

SRC+= unit-tests/SampleFileToUT_UnitTests/test_SFToUT.c   
SRC+= unit-tests/SampleFileToUT_UnitTests/test_runnerSFToUT.c 
SRC+= unit-tests/SampleFileToUT_UnitTests/test_main.c 


SRC+= applications/MCU_CoreWFI/Src/SampleFileToUT.c 


INC_DIR+= -Iapplications/MCU_CoreWFI/Inc/


COVERAGE_SOURCEDIR :=
COVERAGE_SOURCEDIR +=applications/MCU_CoreWFI/Src


MOCKED_HEADER +=  applications/MCU_CoreWFI/Inc/AnotherSampleFile.h
MOCKED_HEADER +=  applications/MCU_CoreWFI/Inc/SampleFileToUT.h


MOCKED_SOURCE +=  $(BUILD_DIR_UT)/cmock/mock_AnotherSampleFile.c


SRC+= $(MOCKED_SOURCE)
