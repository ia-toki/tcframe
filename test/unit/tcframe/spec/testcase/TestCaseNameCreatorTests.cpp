#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestCaseNameCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseNameCreatorTests : public Test {};

TEST_F(TestCaseNameCreatorTests, Creation_Sample) {
    EXPECT_THAT(TestCaseNameCreator::create("foo", 0, 42), Eq("foo_sample_42"));
}

TEST_F(TestCaseNameCreatorTests, Creation_Sample_Base) {
    EXPECT_THAT(TestCaseNameCreator::createBaseName("foo", 0), Eq("foo_sample"));
}

TEST_F(TestCaseNameCreatorTests, Creation_Official) {
    EXPECT_THAT(TestCaseNameCreator::create("foo", -1, 42), Eq("foo_42"));
}

TEST_F(TestCaseNameCreatorTests, Creation_Official_Base) {
    EXPECT_THAT(TestCaseNameCreator::createBaseName("foo", -1), Eq("foo"));
}

TEST_F(TestCaseNameCreatorTests, Creation_Official_WithGroups) {
    EXPECT_THAT(TestCaseNameCreator::create("foo", 7, 42), Eq("foo_7_42"));
}

TEST_F(TestCaseNameCreatorTests, Creation_Official_WithGroups_Base) {
    EXPECT_THAT(TestCaseNameCreator::createBaseName("foo", 7), Eq("foo_7"));
}

}
