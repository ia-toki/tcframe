#include "gmock/gmock.h"

#include "tcframe/runner/verdict/TestCaseVerdict.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseVerdictTests : public Test {};

TEST_F(TestCaseVerdictTests, ToString_WithoutPoints) {
    TestCaseVerdict verdict(Verdict::wa());
    EXPECT_THAT(verdict.toString(), Eq(verdict.verdict().name()));
}

TEST_F(TestCaseVerdictTests, ToString_WithPoints) {
    TestCaseVerdict verdict1(Verdict::ok(), 30);
    EXPECT_THAT(verdict1.toString(), Eq(verdict1.verdict().name() + " [30]"));
    TestCaseVerdict verdict2(Verdict::ok(), 30.1);
    EXPECT_THAT(verdict2.toString(), Eq(verdict2.verdict().name() + " [30.1]"));
    TestCaseVerdict verdict3(Verdict::ok(), 30.12);
    EXPECT_THAT(verdict3.toString(), Eq(verdict3.verdict().name() + " [30.12]"));
    TestCaseVerdict verdict4(Verdict::ok(), 30.123);
    EXPECT_THAT(verdict4.toString(), Eq(verdict4.verdict().name() + " [30.12]"));
}

TEST_F(TestCaseVerdictTests, ToBriefString_WithoutPoints) {
    TestCaseVerdict verdict(Verdict::wa());
    EXPECT_THAT(verdict.toBriefString(), Eq(verdict.verdict().code()));
}

TEST_F(TestCaseVerdictTests, ToBriefString_WithPoints) {
    TestCaseVerdict verdict(Verdict::ok(), 30);
    EXPECT_THAT(verdict.toBriefString(), Eq(verdict.verdict().code() + " 30"));
}

}
