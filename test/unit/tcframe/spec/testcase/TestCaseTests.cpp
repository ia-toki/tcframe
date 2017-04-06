#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestCase.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseTests : public Test {};

TEST_F(TestCaseTests, CreateName) {
    EXPECT_THAT(TestCase::createName("foo", 42), Eq("foo_42"));
}

}
