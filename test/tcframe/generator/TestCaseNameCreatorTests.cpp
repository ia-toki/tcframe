#include "gmock/gmock.h"

#include "tcframe/generator/TestCaseNameCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseNameCreatorTests : public Test {};

TEST_F(TestCaseNameCreatorTests, SampleTestCaseNameCreation) {
    EXPECT_THAT(TestCaseNameCreator::createSampleTestCaseName("foo", 42), Eq("foo_sample_42"));
}

TEST_F(TestCaseNameCreatorTests, OfficialTestCaseNameCreation) {
    EXPECT_THAT(TestCaseNameCreator::createOfficialTestCaseName("foo", -1, 42), Eq("foo_42"));
}

TEST_F(TestCaseNameCreatorTests, OfficialTestCaseNameCreation_WithGroups) {
    EXPECT_THAT(TestCaseNameCreator::createOfficialTestCaseName("foo", 7, 42), Eq("foo_7_42"));
}

}
