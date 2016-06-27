#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "TestCaseGenerationResult.hpp"
#include "MultipleTestCasesCombinationResult.hpp"
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

            logFailure(result.failure());
        }
    }

    virtual void logMultipleTestCasesCombinationIntroduction(const string& testCaseBaseId) {
        engine_->logHangingParagraph(1, "Combining test cases into a single file (" + testCaseBaseId + "): ");
    }

    virtual void logMultipleTestCasesCombinationResult(const MultipleTestCasesCombinationResult& result) {
        if (result.isSuccessful()) {
            engine_->logParagraph(0, "OK");
        } else {
            engine_->logParagraph(0, "FAILED");
            engine_->logParagraph(2, "Reasons:");

            logFailure(result.failure());
        }
    }

private:
    void logFailure(Failure* failure) {
        if (failure->type() == FailureType::CONSTRAINTS_VERIFICATION) {
            logConstraintsVerificationFailure(((ConstraintsVerificationFailure*) failure)->verificationResult());
        } else if (failure->type() == FailureType::MULTIPLE_TEST_CASES_CONSTRAINTS_VERIFICATION) {
            logMultipleTestCasesConstraintsVerificationFailure(
                    ((MultipleTestCasesConstraintsVerificationFailure*) failure)->verificationResult());
        }
    }

    void logConstraintsVerificationFailure(const ConstraintsVerificationResult& result) {
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

    void logMultipleTestCasesConstraintsVerificationFailure(
            const MultipleTestCasesConstraintsVerificationResult& result) {

        engine_->logListItem1(2, "Does not satisfy constraints, on:");

        for (const string& unsatisfiedConstraintDescription : result.unsatisfiedConstraintDescriptions()) {
            engine_->logListItem2(3, unsatisfiedConstraintDescription);
        }
    }
 };

}
