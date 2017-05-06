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

    virtual void logResult(const map<int, Verdict>& subtaskVerdicts, const Verdict& verdict) {
        if (subtaskVerdicts.size() > 1) {
            engine_->logHeading("SUBTASK RESULTS");
            for (auto entry : subtaskVerdicts) {
                engine_->logParagraph(
                        1,
                        "Subtask " + StringUtils::toString(entry.first) + ": " + entry.second.status().name());
            }
        }

        engine_->logHeading("RESULT");
        engine_->logParagraph(1, verdict.status().name());
    }
};

}
