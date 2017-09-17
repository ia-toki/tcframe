#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "GeneratorLogger.hpp"
#include "tcframe/runner/logger.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/verifier.hpp"
#include "tcframe/util.hpp"

using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

class DefaultGeneratorLogger : public GeneratorLogger, public DefaultBaseLogger {
public:
    virtual ~DefaultGeneratorLogger() = default;

    explicit DefaultGeneratorLogger(LoggerEngine* engine)
            : DefaultBaseLogger(engine) {}

    void logIntroduction() {
        engine_->logParagraph(0, "Generating test cases...");
    }

    void logSuccessfulResult() {
        engine_->logParagraph(0, "");
        engine_->logParagraph(0, "Generation finished. All test cases OK.");
    }

    void logFailedResult() {
        engine_->logParagraph(0, "");
        engine_->logParagraph(0, "Generation finished. Some test cases FAILED.");
    }

    void logTestCaseSuccessfulResult() {
        engine_->logParagraph(0, "OK");
    }

    void logTestCaseFailedResult(const optional<string>& testCaseDescription) {
        engine_->logParagraph(0, "FAILED");
        if (testCaseDescription) {
            engine_->logParagraph(2, "Description: " + testCaseDescription.value());
        }
        engine_->logParagraph(2, "Reasons:");
    }

    void logMultipleTestCasesCombinationIntroduction(const string& testGroupName) {
        engine_->logHangingParagraph(1, "Combining test cases into a single file (" + testGroupName + "): ");
    }

    void logMultipleTestCasesCombinationSuccessfulResult() {
        engine_->logParagraph(0, "OK");
    }

    void logMultipleTestCasesCombinationFailedResult() {
        engine_->logParagraph(0, "FAILED");
        engine_->logParagraph(2, "Reasons:");
    }
};

}
