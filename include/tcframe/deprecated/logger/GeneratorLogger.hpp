#pragma once

#include "tcframe/deprecated/logger/Logger.hpp"

#include <string>

using std::string;

namespace tcframe_old {

class GeneratorLogger : public Logger {
public:
    virtual ~GeneratorLogger() { }

    virtual void logIntroduction() = 0;
    virtual void logTestCaseOkResult() = 0;
    virtual void logTestCaseFailedResult(string testCaseDescription) = 0;
    virtual void logMultipleTestCasesCombinationIntroduction(string testCaseBaseName) = 0;
    virtual void logMultipleTestCasesCombinationOkResult() = 0;
    virtual void logMultipleTestCasesCombinationFailedResult() = 0;
};

}
