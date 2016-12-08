#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/GeneratorLogger.hpp"

namespace tcframe {

class MockGeneratorLogger : public GeneratorLogger {
public:
    MockGeneratorLogger()
            : GeneratorLogger(nullptr) {}

    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(string));
    MOCK_METHOD1(logSolutionExecutionFailure, void(const ExecutionResult&));

    MOCK_METHOD0(logIntroduction, void());
    MOCK_METHOD0(logSuccessfulResult, void());
    MOCK_METHOD0(logFailedResult, void());
    MOCK_METHOD0(logTestCaseSuccessfulResult, void());
    MOCK_METHOD1(logTestCaseFailedResult, void(const optional<string>&));
    MOCK_METHOD1(logMultipleTestCasesCombinationIntroduction, void(const string&));
    MOCK_METHOD0(logMultipleTestCasesCombinationSuccessfulResult, void());
    MOCK_METHOD0(logMultipleTestCasesCombinationFailedResult, void());
    MOCK_METHOD1(logConstraintsVerificationFailure, void(const ConstraintsVerificationResult&));
    MOCK_METHOD1(logMultipleTestCasesConstraintsVerificationFailure, void(
            const MultipleTestCasesConstraintsVerificationResult&));
    MOCK_METHOD1(logSampleTestCaseCheckFailure, void(const string&));
    MOCK_METHOD0(logSampleTestCaseNoOutputNeededFailure, void());
    MOCK_METHOD1(logSimpleFailure, void(const string&));
};

}
