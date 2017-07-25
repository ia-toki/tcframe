#pragma once

#include <map>

#include "GraderLogger.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/verdict.hpp"

using std::map;

namespace tcframe {

class BriefGraderLogger : public GraderLogger {
private:
    LoggerEngine* engine_;

public:
    virtual ~BriefGraderLogger() {}

    BriefGraderLogger(LoggerEngine* engine)
            : engine_(engine) {}

    void logTestGroupIntroduction(int) {}
    void logTestCaseIntroduction(const string&) {}
    void logExecutionResults(const map<string, ExecutionResult>&) {}
    void logIntroduction(const string&) {}
    void logTestCaseVerdict(const Verdict&) {}

    void logResult(const map<int, Verdict>& subtaskVerdicts, const Verdict& verdict) {
        engine_->logParagraph(0, verdict.toBriefString());

        if (subtaskVerdicts.size() > 1) {
            for (auto entry : subtaskVerdicts) {
                engine_->logParagraph(0, entry.second.toBriefString());
            }
        }
    }
};

}
