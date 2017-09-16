#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/runner/verdict/VerdictCreator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class VerdictCreatorTests : public Test {
protected:
    VerdictCreator verdictCreator;
};

TEST_F(VerdictCreatorTests, FromStream_AC) {
    EXPECT_THAT(verdictCreator.fromStream(new istringstream("AC\n")), Eq(Verdict(VerdictStatus::ac())));
}

TEST_F(VerdictCreatorTests, FromStream_WA) {
    EXPECT_THAT(verdictCreator.fromStream(new istringstream("WA\n")), Eq(Verdict(VerdictStatus::wa())));
}

TEST_F(VerdictCreatorTests, FromStream_OK) {
    EXPECT_THAT(verdictCreator.fromStream(new istringstream("OK\n70\n")), Eq(Verdict(VerdictStatus::ok(), 70)));
}

TEST_F(VerdictCreatorTests, FromStream_OK_WithFeedback) {
    EXPECT_THAT(verdictCreator.fromStream(new istringstream("OK\n70 text\n")), Eq(Verdict(VerdictStatus::ok(), 70)));
}

TEST_F(VerdictCreatorTests, FromStream_OK_Failed_EmptySecondLine) {
    try {
        verdictCreator.fromStream(new istringstream("OK\n"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <points> on the second line"));
    }
}

TEST_F(VerdictCreatorTests, FromStream_OK_Failed_UnknownPointsFormat) {
    try {
        verdictCreator.fromStream(new istringstream("OK\nabc\n"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Unknown points format: abc"));
    }
}

TEST_F(VerdictCreatorTests, FromStream_Empty) {
    try {
        verdictCreator.fromStream(new istringstream(""));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <status> on the first line"));
    }
}

TEST_F(VerdictCreatorTests, FromStream_UnknownVerdict) {
    try {
        verdictCreator.fromStream(new istringstream("hokus pokus"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Unknown status: hokus pokus"));
    }
}

TEST_F(VerdictCreatorTests, FromExecutionResult_Nothing) {
    EXPECT_FALSE(verdictCreator.fromExecutionResult(ExecutionResult()));
}

TEST_F(VerdictCreatorTests, FromExecutionResult_TLE) {
    auto executionResult = ExecutionResultBuilder()
            .setExitSignal(SIGXCPU)
            .build();
    EXPECT_THAT(
            verdictCreator.fromExecutionResult(executionResult),
            Eq(optional<Verdict>(Verdict(VerdictStatus::tle()))));
}

TEST_F(VerdictCreatorTests, FromExecutionResult_RTE) {
    auto executionResult = ExecutionResultBuilder().setExitCode(1).build();
    EXPECT_THAT(
            verdictCreator.fromExecutionResult(executionResult),
            Eq(optional<Verdict>(Verdict(VerdictStatus::rte()))));
}

}
