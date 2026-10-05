#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class ValidatorEteTests : public BaseEteTests {
protected:
    void SetUp() {
        ASSERT_THAT(execStatus("cd test-ete/validator && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));
    }
};

TEST_F(ValidatorEteTests, Normal_ReportsAllSatisfiedSubtasks) {
    ASSERT_THAT(execStatus("cd test-ete/validator && ./build/spec validate < both.in > out.txt"), Eq(0));

    EXPECT_THAT(readFile("test-ete/validator/out.txt"), StrEq("2\n1\n2\n"));
}

TEST_F(ValidatorEteTests, Normal_ReportsOnlySatisfiedSubtask) {
    ASSERT_THAT(execStatus("cd test-ete/validator && ./build/spec validate < subtask2-only.in > out.txt"), Eq(0));

    EXPECT_THAT(readFile("test-ete/validator/out.txt"), StrEq("1\n2\n"));
}

TEST_F(ValidatorEteTests, Normal_NoSubtaskSatisfied) {
    ASSERT_THAT(execStatus("cd test-ete/validator && ./build/spec validate < none.in > out.txt"), Eq(0));

    EXPECT_THAT(readFile("test-ete/validator/out.txt"), StrEq("0\n"));
}

TEST_F(ValidatorEteTests, Invalid_MainConstraintsFail) {
    ASSERT_THAT(execStatus("cd test-ete/validator && ./build/spec validate < invalid.in > out.txt 2> err.txt; test $? -eq 1"), Eq(0));

    EXPECT_THAT(readFile("test-ete/validator/out.txt"), StrEq(""));
    EXPECT_THAT(readFile("test-ete/validator/err.txt"), HasSubstr("Does not satisfy constraints, on:"));
}

TEST_F(ValidatorEteTests, Distribution_MakeShipsValidatorThatAcceptsEveryTestCase) {
    ASSERT_THAT(execStatus("cd test-ete/validator && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe make --solution=./solution"), Eq(0));
    ASSERT_THAT(execStatus("cd test-ete/validator && test -x build/validator"), Eq(0));

    EXPECT_THAT(execStatus("cd test-ete/validator && for f in build/tc/*.in; do ./build/validator validate < \"$f\" > /dev/null || exit 1; done"), Eq(0));
}

TEST_F(ValidatorEteTests, Multi_UnsupportedSpecExitsWithCode2) {
    ASSERT_THAT(execStatus("cd test-ete/multi && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));

    EXPECT_THAT(execStatus("cd test-ete/multi && ./build/spec validate < /dev/null > /dev/null 2>&1; test $? -eq 2"), Eq(0));
}

}
