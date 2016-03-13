#pragma once

#include "tcframe/core/Failure.hpp"
#include "tcframe/logger/Logger.hpp"

#include <map>

using std::map;

namespace tcframe {

class SubmitterLogger : public Logger {
public:
    virtual ~SubmitterLogger() { }

    virtual void logIntroduction() = 0;
    virtual void logTestCaseVerdict(Verdict verdict) = 0;
    virtual void logSubmissionResult(map<int, Verdict> subtaskVerdicts) = 0;
    virtual void logBriefSubmissionResult(map<int, Verdict> subtaskVerdicts) = 0;
};

}
