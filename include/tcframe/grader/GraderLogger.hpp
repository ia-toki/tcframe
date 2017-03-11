#pragma once

#include <map>

#include "tcframe/grade.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/scorer.hpp"
#include "tcframe/util.hpp"

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

    virtual void logTestCaseGradeSummary(const TestCaseGrade& grade) {
        engine_->logParagraph(0, grade.verdict().name());
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
