#include "gmock/gmock.h"

#include "tcframe/generation/TestCaseNameCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseNameCreatorTests : public Test {};

TEST_F(TestCaseNameCreatorTests, NameWithoutTestGroups) {
    EXPECT_THAT(TestCaseNameCreator::createTestCaseName("foo", -1, 42), Eq("foo_42"));
}

TEST_F(TestCaseNameCreatorTests, NameWithTestGroups) {
    EXPECT_THAT(TestCaseNameCreator::createTestCaseName("foo", 7, 42), Eq("foo_7_42"));
}

}
