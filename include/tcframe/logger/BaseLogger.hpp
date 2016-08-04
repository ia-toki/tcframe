#pragma once

#include <streambuf>

#include "LoggerEngine.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::istreambuf_iterator;

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
        if (result.info().exitStatus() <= 128) {
            engine_->logListItem2(3, "Exit code: " + StringUtils::toString(result.info().exitStatus()));
            engine_->logListItem2(3, "Standard error: " + string(
                    istreambuf_iterator<char>(*result.errorStream()), istreambuf_iterator<char>()));
        } else {
            engine_->logListItem2(3, string(strsignal(result.info().exitStatus() - 128)));
        }
    }
};

}
