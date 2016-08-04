#pragma once

#include <map>

#include "Verdict.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/util.hpp"

using std::map;

namespace tcframe {

class SubmitterLogger : public BaseLogger {
public:
    virtual ~SubmitterLogger() {}

    SubmitterLogger(LoggerEngine* engine)
            : BaseLogger(engine) {}

    virtual void logIntroduction() {
        engine_->logParagraph(0, "Submitting...");
    }

    virtual void logTestCaseVerdict(const Verdict& verdict) {
        engine_->logParagraph(0, verdict.name());
    }

    virtual void logDiffFailure(const string& diff) {
        engine_->logListItem1(2, "Diff:");
        engine_->logParagraph(0, diff);
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
