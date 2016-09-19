#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestCaseIdCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseIdCreatorTests : public Test {};

TEST_F(TestCaseIdCreatorTests, Creation_Sample) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", 0, 42), Eq("foo_sample_42"));
}

TEST_F(TestCaseIdCreatorTests, Creation_Sample_Base) {
    EXPECT_THAT(TestCaseIdCreator::createBaseId("foo", 0), Eq("foo_sample"));
}

TEST_F(TestCaseIdCreatorTests, Creation_Official) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", -1, 42), Eq("foo_42"));
}

TEST_F(TestCaseIdCreatorTests, Creation_Official_Base) {
    EXPECT_THAT(TestCaseIdCreator::createBaseId("foo", -1), Eq("foo"));
}

TEST_F(TestCaseIdCreatorTests, Creation_Official_WithGroups) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", 7, 42), Eq("foo_7_42"));
}

TEST_F(TestCaseIdCreatorTests, Creation_Official_WithGroups_Base) {
    EXPECT_THAT(TestCaseIdCreator::createBaseId("foo", 7), Eq("foo_7"));
}

}
