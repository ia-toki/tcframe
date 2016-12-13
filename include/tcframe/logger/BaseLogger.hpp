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

    virtual void logSolutionExecutionFailure(const ExecutionResult& result) {
        engine_->logListItem1(2, "Execution of solution failed:");
        if (result.info().exitCode()) {
            engine_->logListItem2(3, "Exit code: " + StringUtils::toString(result.info().exitCode().value()));
            engine_->logListItem2(3, "Standard error: " + string(
                    StringUtils::streamToString(result.errorStream())));
        } else {
            engine_->logListItem2(3, "Exit signal: " + result.info().exitSignal().value());
        }
    }
};

}
