#include "unity.h"
#include "unity_fixture.h"
#include "cmock.h"

#include "mock_AnotherSampleFile.h"
#include "SampleFileToUT.h"


static int data = -1;

TEST_GROUP(test_SFToUT);

TEST_SETUP(test_SFToUT)
{
    data = 0;
}

TEST_TEAR_DOWN(test_SFToUT)
{
    data = -1;
}

TEST(test_SFToUT, test_myfristtest)
{
    char c[]="ABC";

    FakeUartSendData_Ignore();
    FakeUartPrintInfo(1, c);
    TEST_ASSERT_EQUAL_INT(1,c[0]);
}
