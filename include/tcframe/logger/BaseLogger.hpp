#pragma once

#include <map>
#include <string>

#include "tcframe/os.hpp"

using std::map;
using std::string;

namespace tcframe {

class BaseLogger {
public:
    virtual ~BaseLogger() {}

    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(const string& testCaseName) = 0;
    virtual void logExecutionResults(const map<string, ExecutionResult>& executionResults) = 0;
};

}
