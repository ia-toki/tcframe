#pragma once

#include "LoggerEngine.hpp"
#include "tcframe/grade.hpp"
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

    virtual void logTestCaseIntroduction(string testCaseName) {
        engine_->logHangingParagraph(1, testCaseName + ": ");
    }

    virtual void logTestCaseGradeDetails(const TestCaseGrade& grade) {
        if (grade.evaluationExecutionResult() && !grade.evaluationExecutionResult().value().isSuccessful()) {
            logExecutionFailure("solution", grade.evaluationExecutionResult().value());
        }
        if (grade.scoringExecutionResult() && !grade.scoringExecutionResult().value().isSuccessful()) {
            logExecutionFailure("scorer", grade.scoringExecutionResult().value());
        }
        if (grade.privateScoringInfo()) {
            engine_->logListItem1(2, grade.privateScoringInfo().value());
        }
    }

    /* Visible for testing */
    void logExecutionFailure(const string& context, const ExecutionResult& result) {
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
