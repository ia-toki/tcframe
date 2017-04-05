#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestGroup.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestGroupTests : public Test {};

TEST_F(TestGroupTests, CreateName_Sample) {
    EXPECT_THAT(TestGroup::createName("foo", TestGroup::SAMPLE_ID), Eq("foo_sample"));
}

TEST_F(TestGroupTests, CreateName_Main) {
    EXPECT_THAT(TestGroup::createName("foo", TestGroup::MAIN_ID), Eq("foo"));
}

TEST_F(TestGroupTests, CreateName_Others) {
    EXPECT_THAT(TestGroup::createName("foo", 42), Eq("foo_42"));
}

}
