#pragma once

#include "tcframe/deprecated/core/Failure.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe_old {

class Logger {
public:
    virtual ~Logger() { }

    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(string testCaseName) = 0;
    virtual void logFailures(vector<Failure> failures) = 0;
};

}

