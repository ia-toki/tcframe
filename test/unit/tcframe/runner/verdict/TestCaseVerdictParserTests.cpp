#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/runner/verdict/TestCaseVerdictParser.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class TestCaseVerdictParserTests : public Test {
protected:
    TestCaseVerdictParser parser;
};

TEST_F(TestCaseVerdictParserTests, ParseStream_AC) {
    EXPECT_THAT(parser.parseStream(new istringstream("AC\n")), Eq(TestCaseVerdict(Verdict::ac())));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_WA) {
    EXPECT_THAT(parser.parseStream(new istringstream("WA\n")), Eq(TestCaseVerdict(Verdict::wa())));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Points) {
    EXPECT_THAT(parser.parseStream(new istringstream("OK\n70\n")), Eq(TestCaseVerdict(Verdict::ok(), 70)));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Points_WithFeedback) {
    EXPECT_THAT(parser.parseStream(new istringstream("OK\n70 text\n")), Eq(TestCaseVerdict(Verdict::ok(), 70)));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Percentage) {
    EXPECT_THAT(parser.parseStream(new istringstream("OK\n25%\n")), Eq(TestCaseVerdict(Verdict::ok(), optional<double>(), optional<double>(25))));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Percentage_WithFeedback) {
    EXPECT_THAT(parser.parseStream(new istringstream("OK\n25% text\n")), Eq(TestCaseVerdict(Verdict::ok(), optional<double>(), optional<double>(25))));
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Failed_EmptySecondLine) {
    try {
        parser.parseStream(new istringstream("OK\n"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <points> on the second line"));
    }
}

TEST_F(TestCaseVerdictParserTests, ParseStream_OK_Failed_UnknownPointsFormat) {
    try {
        parser.parseStream(new istringstream("OK\nabc\n"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Unknown points format: abc"));
    }
}

TEST_F(TestCaseVerdictParserTests, ParseStream_Empty) {
    try {
        parser.parseStream(new istringstream(""));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <verdict> on the first line"));
    }
}

TEST_F(TestCaseVerdictParserTests, ParseStream_UnknownVerdict) {
    try {
        parser.parseStream(new istringstream("hokus pokus"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Unknown verdict: hokus pokus"));
    }
}

TEST_F(TestCaseVerdictParserTests, ParseExecutionResult_Nothing) {
    EXPECT_FALSE(parser.parseExecutionResult(ExecutionResult()));
}

TEST_F(TestCaseVerdictParserTests, ParseExecutionResult_TLE) {
    auto executionResult = ExecutionResultBuilder()
            .setExitSignal(SIGXCPU)
            .build();
    EXPECT_THAT(
            parser.parseExecutionResult(executionResult),
            Eq(optional<TestCaseVerdict>(TestCaseVerdict(Verdict::tle()))));
}

TEST_F(TestCaseVerdictParserTests, ParseExecutionResult_RTE) {
    auto executionResult = ExecutionResultBuilder().setExitCode(1).build();
    EXPECT_THAT(
            parser.parseExecutionResult(executionResult),
            Eq(optional<TestCaseVerdict>(TestCaseVerdict(Verdict::rte()))));
}

}
