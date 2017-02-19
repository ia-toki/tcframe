#pragma once

#include "gmock/gmock.h"

#include "tcframe/grader/GraderLogger.hpp"

namespace tcframe {

class MockGraderLogger : public GraderLogger {
public:
    MockGraderLogger()
            : GraderLogger(nullptr) {}

    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(string));
    MOCK_METHOD1(logTestCaseScoringMessage, void(const string&));
    MOCK_METHOD2(logExecutionFailure, void(const string&, const ExecutionResult&));

    MOCK_METHOD1(logIntroduction, void(const string&));
    MOCK_METHOD1(logTestCaseVerdict, void(const Verdict&));

    MOCK_METHOD1(logResult, void(const map<int, Verdict>&));
};

}
