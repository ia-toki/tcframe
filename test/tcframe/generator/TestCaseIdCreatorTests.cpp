#include "gmock/gmock.h"

#include "tcframe/generator/TestCaseIdCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseIdCreatorTests : public Test {};

TEST_F(TestCaseIdCreatorTests, SampleTestCaseIdCreation) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", 0, 42), Eq("foo_sample_42"));
}

TEST_F(TestCaseIdCreatorTests, OfficialTestCaseIdCreation) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", -1, 42), Eq("foo_42"));
}

TEST_F(TestCaseIdCreatorTests, OfficialTestCaseIdCreation_WithGroups) {
    EXPECT_THAT(TestCaseIdCreator::create("foo", 7, 42), Eq("foo_7_42"));
}

}
