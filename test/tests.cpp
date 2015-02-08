#include "gtest/gtest.h"

#include "constraint_test.cpp"
#include "exception_test.cpp"
#include "failure_test.cpp"
#include "io_test.cpp"
#include "tcframe_test.cpp"
#include "testcase_test.cpp"
#include "type_test.cpp"
#include "util_test.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
