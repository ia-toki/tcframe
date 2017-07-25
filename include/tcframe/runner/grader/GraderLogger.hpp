#pragma once

#include <map>

#include "tcframe/logger.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;

namespace tcframe {

class GraderLogger : public virtual BaseLogger {
public:
    virtual ~GraderLogger() {}

    virtual void logIntroduction(const string& solutionCommand) = 0;
    virtual void logTestCaseVerdict(const Verdict& verdict) = 0;
    virtual void logResult(const map<int, Verdict>& subtaskVerdicts, const Verdict& verdict) = 0;
};

}
