#pragma once

#include <map>

#include "LoggerEngine.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec/testcase.hpp"
#include "tcframe/util.hpp"

using std::map;

namespace tcframe {

class BaseLogger {
protected:
    LoggerEngine* engine_;

public:
    virtual ~BaseLogger() {}

    BaseLogger(LoggerEngine* engine)
            : engine_(engine) {}

    virtual void logTestGroupIntroduction(int testGroupId) {
        if (testGroupId == TestGroup::SAMPLE_ID) {
            engine_->logHeading("SAMPLE TEST CASES");
        } else if (testGroupId == TestGroup::MAIN_ID) {
            engine_->logHeading("OFFICIAL TEST CASES");
        } else {
            engine_->logHeading("TEST GROUP " + StringUtils::toString(testGroupId));
        }
    }

    virtual void logTestCaseIntroduction(string testCaseName) {
        engine_->logHangingParagraph(1, testCaseName + ": ");
    }

    virtual void logExecutionResults(const map<string, ExecutionResult>& executionResults) {
        for (auto& entry : executionResults) {
            const string& key = entry.first;
            const ExecutionResult& executionResult = entry.second;

            if (!executionResult.isSuccessful()) {
                engine_->logListItem1(2, "Execution of " + key + " failed:");
                if (executionResult.exitCode()) {
                    engine_->logListItem2(3, "Exit code: " + StringUtils::toString(executionResult.exitCode().value()));
                    engine_->logListItem2(3, "Standard error: " + executionResult.standardError());
                } else {
                    engine_->logListItem2(3, "Exit signal: " + executionResult.exitSignal().value());
                }
            } else if (!executionResult.standardError().empty()) {
                engine_->logListItem1(2, key + ": " + executionResult.standardError());
            }
        }
    }
};

}
