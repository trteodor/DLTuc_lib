#include "unity.h"
#include "unity_fixture.h"
#include "cmock.h"

#include "mock_SampleFileToUT.h"

#include "AnotherSampleFile.h"



static int data = -1;

TEST_GROUP(test_ASFile);

TEST_SETUP(test_ASFile)
{
    data = 0;
}

TEST_TEAR_DOWN(test_ASFile)
{
    data = -1;
}

TEST(test_ASFile, test_myfristtest1)
{
    FakeUartSendData((uint8_t *)"ABC", 2);
}
