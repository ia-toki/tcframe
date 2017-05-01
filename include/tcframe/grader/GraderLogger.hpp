#pragma once

#include <map>

#include "tcframe/logger.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;

namespace tcframe {

class GraderLogger : public BaseLogger {
public:
    virtual ~GraderLogger() {}

    GraderLogger(LoggerEngine* engine)
            : BaseLogger(engine) {}

    virtual void logIntroduction(const string& solutionCommand) {
        engine_->logParagraph(0, "Local grading with solution command: '" + solutionCommand + "'...");
    }

    virtual void logTestCaseVerdict(const Verdict& verdict) {
        engine_->logParagraph(0, verdict.status().name());
    }

    virtual void logResult(const map<int, Verdict>& subtaskVerdicts) {
        engine_->logHeading("RESULT");
        for (auto entry : subtaskVerdicts) {
            if (entry.first == Subtask::MAIN_ID) {
                engine_->logParagraph(1, entry.second.status().name());
            } else {
                engine_->logParagraph(
                        1,
                        "Subtask " + StringUtils::toString(entry.first) + ": " + entry.second.status().name());
            }
        }
    }
};

}
