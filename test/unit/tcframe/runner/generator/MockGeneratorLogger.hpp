#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/generator/GeneratorLogger.hpp"

namespace tcframe {

class MockGeneratorLogger : public GeneratorLogger {
public:
    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(const string&));
    MOCK_METHOD1(logExecutionResults, void(const map<string, ExecutionResult>&));

    MOCK_METHOD0(logIntroduction, void());
    MOCK_METHOD0(logSuccessfulResult, void());
    MOCK_METHOD0(logFailedResult, void());
    MOCK_METHOD0(logTestCaseSuccessfulResult, void());
    MOCK_METHOD1(logTestCaseFailedResult, void(const optional<string>&));
    MOCK_METHOD1(logMultipleTestCasesCombinationIntroduction, void(const string&));
    MOCK_METHOD0(logMultipleTestCasesCombinationSuccessfulResult, void());
    MOCK_METHOD0(logMultipleTestCasesCombinationFailedResult, void());
    MOCK_METHOD1(logSimpleError, void(const runtime_error&));
    MOCK_METHOD1(logFormattedError, void(const FormattedError&));
};

}
