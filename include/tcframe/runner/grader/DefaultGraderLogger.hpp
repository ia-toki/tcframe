#pragma once

#include <map>

#include "GraderLogger.hpp"
#include "tcframe/runner/logger.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::map;

namespace tcframe {

class DefaultGraderLogger : public GraderLogger, public DefaultBaseLogger {
public:
    virtual ~DefaultGraderLogger() = default;

    explicit DefaultGraderLogger(LoggerEngine* engine)
            : DefaultBaseLogger(engine) {}

    void logIntroduction(const string& solutionCommand) {
        engine_->logParagraph(0, "Local grading with solution command: '" + solutionCommand + "'...");
    }

    void logTestCaseVerdict(const Verdict& verdict) {
        engine_->logParagraph(0, verdict.toString());
    }

    void logResult(const map<int, Verdict>& subtaskVerdicts, const Verdict& verdict) {
        if (subtaskVerdicts.size() > 1) {
            engine_->logHeading("SUBTASK VERDICTS");
            for (auto entry : subtaskVerdicts) {
                engine_->logParagraph(
                        1,
                        "Subtask " + StringUtils::toString(entry.first) + ": " + entry.second.toString());
            }
        }

        engine_->logHeading("VERDICT");
        engine_->logParagraph(1, verdict.toString());
    }
};

}
