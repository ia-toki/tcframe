#pragma once

#include <map>
#include <string>

#include "BaseLogger.hpp"
#include "LoggerEngine.hpp"
#include "tcframe/exception.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/spec/testcase.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::string;

namespace tcframe {

class DefaultBaseLogger : public virtual BaseLogger {
protected:
    LoggerEngine* engine_;

public:
    virtual ~DefaultBaseLogger() = default;

    explicit DefaultBaseLogger(LoggerEngine* engine)
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

    virtual void logTestCaseIntroduction(const string& testCaseName) {
        engine_->logHangingParagraph(1, testCaseName + ": ");
    }

    virtual void logError(runtime_error* e) {
        auto formattedE = dynamic_cast<FormattedError*>(e);
        if (formattedE != nullptr) {
            logFormattedError(*formattedE);
        } else {
            logSimpleError(*e);
        }
    }

private:
    void logSimpleError(const runtime_error& e) {
        engine_->logListItem1(2, e.what());
    }

    void logFormattedError(const FormattedError& e) {
        for (const auto& entry : e.messages()) {
            if (entry.first == 0) {
                engine_->logListItem1(2, entry.second);
            } else {
                engine_->logListItem2(3, entry.second);
            }
        }
    }
};

}
