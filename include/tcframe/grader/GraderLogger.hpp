#pragma once

#include <map>

#include "tcframe/logger.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;

namespace tcframe {

class GraderLogger : public BaseLogger {
public:
    virtual ~GraderLogger() {}

    GraderLogger(LoggerEngine* engine)
            : BaseLogger(engine) {}

    virtual void logIntroduction() {
        engine_->logParagraph(0, "Grading...");
    }

    virtual void logTestCaseVerdict(const Verdict& verdict) {
        engine_->logParagraph(0, verdict.name());
    }

    virtual void logResult(const map<int, Verdict>& subtaskVerdicts) {
        engine_->logHeading("RESULT");
        for (auto entry : subtaskVerdicts) {
            if (entry.first == -1) {
                engine_->logParagraph(1, entry.second.name());
            } else {
                engine_->logParagraph(1, "Subtask " + StringUtils::toString(entry.first) + ": " + entry.second.name());
            }
        }
    }
};

}
