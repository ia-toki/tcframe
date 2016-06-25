#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "TestCaseGenerationResult.hpp"
#include "tcframe/failure.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verifier.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;

namespace tcframe {

class GeneratorLogger : public BaseLogger {
public:
    virtual ~GeneratorLogger() {}

    GeneratorLogger(LoggerEngine* engine)
            : BaseLogger(engine)
    {}

    virtual void logIntroduction() {
        engine_->logParagraph(0, "Generating test cases...");
    }

    virtual void logTestCaseResult(const string& testCaseDescription, const TestCaseGenerationResult& result) {
        if (result.isSuccessful()) {
            engine_->logParagraph(0, "OK");
        } else {
            engine_->logParagraph(0, "FAILED");
            engine_->logParagraph(2, "Description: " + testCaseDescription);
            engine_->logParagraph(2, "Reasons:");

            printTestCaseFailure(result.failure());
        }
    }

private:
    void printTestCaseFailure(Failure* failure) {
        if (failure->type() == FailureType::VERIFICATION) {
            printVerificationFailure(((VerificationFailure*) failure)->verificationResult());
        }
    }

    void printVerificationFailure(const VerificationResult& result) {
        for (const auto& entry : result.unsatisfiedConstraintDescriptionsBySubtaskId()) {
            int subtaskId = entry.first;
            const vector<string>& unsatisfiedConstraintDescriptions = entry.second;

            if (subtaskId == -1) {
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
 };

}
