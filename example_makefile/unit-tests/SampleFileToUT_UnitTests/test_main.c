#include "unity.h"
#include "unity_fixture.h"

static void runAllTests(void)
{
    RUN_TEST_GROUP(test_SFToUT);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

