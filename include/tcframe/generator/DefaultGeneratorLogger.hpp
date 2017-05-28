#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "GeneratorLogger.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verifier.hpp"

using std::string;
using std::vector;

namespace tcframe {

class DefaultGeneratorLogger : public GeneratorLogger, public DefaultBaseLogger {
public:
    virtual ~DefaultGeneratorLogger() {}

    DefaultGeneratorLogger(LoggerEngine* engine)
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

    void logConstraintsVerificationFailure(const ConstraintsVerificationResult& result) {
        for (const auto& entry : result.unsatisfiedConstraintDescriptionsBySubtaskId()) {
            int subtaskId = entry.first;
            const vector<string>& unsatisfiedConstraintDescriptions = entry.second;

            if (subtaskId == Subtask::MAIN_ID) {
                engine_->logListItem1(2, "Does not satisfy constraints, on:");
            } else {
                engine_->logListItem1(2, "Does not satisfy subtask " + StringUtils::toString(subtaskId) + ", on constraints:");
            }

            for (const string& unsatisfiedConstraintDescription : unsatisfiedConstraintDescriptions) {
                engine_->logListItem2(3, unsatisfiedConstraintDescription);
            }
        }
        for (int subtaskId : result.satisfiedButNotAssignedSubtaskIds()) {
            engine_->logListItem1(2, "Satisfies subtask " + StringUtils::toString(subtaskId) + " but is not assigned to it");
        }
    }

    void logMultipleTestCasesConstraintsVerificationFailure(
            const MultipleTestCasesConstraintsVerificationResult& result) {

        engine_->logListItem1(2, "Does not satisfy constraints, on:");

        for (const string& unsatisfiedConstraintDescription : result.unsatisfiedConstraintDescriptions()) {
            engine_->logListItem2(3, unsatisfiedConstraintDescription);
        }
    }

    void logSampleTestCaseCheckFailure() {
        engine_->logListItem1(2, "Sample test case output does not match with actual output produced by the solution");
    }

    void logSampleTestCaseNoOutputNeededFailure() {
        engine_->logListItem1(2, "Problem does not need test case outputs, but this sample test case has output");
    }

    void logSimpleFailure(const string& message) {
        engine_->logListItem1(2, message);
    }
};

}
