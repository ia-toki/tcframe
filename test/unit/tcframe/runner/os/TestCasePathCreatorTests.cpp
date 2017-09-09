#include "gmock/gmock.h"

#include "tcframe/runner/os/TestCasePathCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCasePathCreatorTests : public Test {};

TEST_F(TestCasePathCreatorTests, InputPath) {
    EXPECT_THAT(TestCasePathCreator::createInputPath("foo_1", "dir"), Eq("dir/foo_1.in"));
}

TEST_F(TestCasePathCreatorTests, OutputPath) {
    EXPECT_THAT(TestCasePathCreator::createOutputPath("foo_1", "dir"), Eq("dir/foo_1.out"));
}

}
