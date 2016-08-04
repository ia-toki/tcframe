#pragma once

#include "gmock/gmock.h"

#include "tcframe/submitter/SubmitterLogger.hpp"

namespace tcframe {

class MockSubmitterLogger : public SubmitterLogger {
public:
    MockSubmitterLogger()
            : SubmitterLogger(nullptr) {}

    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(string));
    MOCK_METHOD1(logSolutionExecutionFailure, void(const ExecutionResult&));

    MOCK_METHOD0(logIntroduction, void());
    MOCK_METHOD1(logTestCaseVerdict, void(const Verdict&));
    MOCK_METHOD1(logDiffFailure, void(const string&));

    MOCK_METHOD1(logResult, void(const map<int, Verdict>&));
};

}
