#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/verdict/TestCaseVerdictParser.hpp"

namespace tcframe {

class MockTestCaseVerdictParser : public TestCaseVerdictParser {
public:
    MOCK_METHOD1(parseStream, TestCaseVerdict(istream*));

    MOCK_METHOD1(parseExecutionResult, optional<TestCaseVerdict>(const ExecutionResult&));
};

}
