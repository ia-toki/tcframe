#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/grader/GraderLogger.hpp"

namespace tcframe {

class MockGraderLogger : public GraderLogger {
public:
    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(const string&));
    MOCK_METHOD1(logError, void(runtime_error*));

    MOCK_METHOD1(logIntroduction, void(const string&));
    MOCK_METHOD1(logTestCaseVerdict, void(const Verdict&));
    MOCK_METHOD2(logResult, void(const map<int, Verdict>&, const Verdict&));
};

}
