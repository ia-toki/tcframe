#include "gmock/gmock.h"

#include "tcframe/verdict/Verdict.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class VerdictTests : public Test {};

TEST_F(VerdictTests, ToString_WithoutPoints) {
    Verdict verdict(VerdictStatus::wa());
    EXPECT_THAT(verdict.toString(), Eq(verdict.status().name()));
}

TEST_F(VerdictTests, ToString_WithPoints) {
    Verdict verdict1(VerdictStatus::ok(), 30);
    EXPECT_THAT(verdict1.toString(), Eq(verdict1.status().name() + " [30]"));
    Verdict verdict2(VerdictStatus::ok(), 30.1);
    EXPECT_THAT(verdict2.toString(), Eq(verdict2.status().name() + " [30.1]"));
    Verdict verdict3(VerdictStatus::ok(), 30.12);
    EXPECT_THAT(verdict3.toString(), Eq(verdict3.status().name() + " [30.12]"));
    Verdict verdict4(VerdictStatus::ok(), 30.123);
    EXPECT_THAT(verdict4.toString(), Eq(verdict4.status().name() + " [30.12]"));
}

}
