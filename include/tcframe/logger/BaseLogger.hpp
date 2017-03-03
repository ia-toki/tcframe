#pragma once

#include "LoggerEngine.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

namespace tcframe {

class BaseLogger {
protected:
    LoggerEngine* engine_;

public:
    virtual ~BaseLogger() {}

    BaseLogger(LoggerEngine* engine)
            : engine_(engine) {}

    virtual void logTestGroupIntroduction(int testGroupId) {
        if (testGroupId == 0) {
            engine_->logHeading("SAMPLE TEST CASES");
        } else if (testGroupId == -1) {
            engine_->logHeading("OFFICIAL TEST CASES");
        } else {
            engine_->logHeading("TEST GROUP " + StringUtils::toString(testGroupId));
        }
    }

    virtual void logTestCaseIntroduction(string testCaseId) {
        engine_->logHangingParagraph(1, testCaseId + ": ");
    }

    virtual void logTestCaseScoringMessage(const string& message) {
        if (!message.empty()) {
            engine_->logListItem1(2, message);
        }
    }

    virtual void logExecutionFailure(const string& context, const ExecutionResult& result) {
        engine_->logListItem1(2, "Execution of " + context + " failed:");
        if (result.exitCode()) {
            engine_->logListItem2(3, "Exit code: " + StringUtils::toString(result.exitCode().value()));
            engine_->logListItem2(3, "Standard error: " + result.standardError());
        } else {
            engine_->logListItem2(3, "Exit signal: " + result.exitSignal().value());
        }
    }
};

}
