#include "gmock/gmock.h"

#include "tcframe/runner/verdict/SubtaskVerdict.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SubtaskVerdictTests : public Test {};

TEST_F(SubtaskVerdictTests, ToString) {
    SubtaskVerdict verdict1(Verdict::ok(), 30);
    EXPECT_THAT(verdict1.toString(), Eq(verdict1.verdict().name() + " [30]"));
    SubtaskVerdict verdict2(Verdict::ok(), 30.1);
    EXPECT_THAT(verdict2.toString(), Eq(verdict2.verdict().name() + " [30.1]"));
    SubtaskVerdict verdict3(Verdict::ok(), 30.12);
    EXPECT_THAT(verdict3.toString(), Eq(verdict3.verdict().name() + " [30.12]"));
    SubtaskVerdict verdict4(Verdict::ok(), 30.123);
    EXPECT_THAT(verdict4.toString(), Eq(verdict4.verdict().name() + " [30.12]"));
}

TEST_F(SubtaskVerdictTests, ToBriefString) {
    SubtaskVerdict verdict(Verdict::ok(), 30);
    EXPECT_THAT(verdict.toBriefString(), Eq(verdict.verdict().code() + " 30"));
}

}
