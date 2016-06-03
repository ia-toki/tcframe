#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "TestCaseGenerationResult.hpp"
#include "VerificationFailure.hpp"
#include "tcframe/verifier.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;

namespace tcframe {

class GenerationLogger {
public:
    virtual ~GenerationLogger() {}

    virtual void logIntroduction() {
        cout << "Generating test cases..." << endl;
    }

    virtual void logSampleTestCasesIntroduction() {
        cout << endl;
        cout << "[ SAMPLE TEST CASES ]" << endl;
    }

    virtual void logTestGroupIntroduction(int testGroupId) {
        cout << endl;

        if (testGroupId == -1) {
            cout << "[ OFFICIAL TEST CASES ]" << endl;
        } else {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    virtual void logTestCaseIntroduction(string testCaseName) {
        cout << "  " << testCaseName << ": " << flush;
    }

    virtual void logTestCaseGenerationResult(const string&constraintDescription, const TestCaseGenerationResult& result) {
        if (result.isSuccessful()) {
            cout << "OK" << endl;
        } else {
            cout << "FAILED" << endl;
            cout << "    Description: " << constraintDescription << endl;
            cout << "    Reasons:" << endl;

            printTestCaseGenerationFailure(result.failure());
        }
    }

private:
    void printTestCaseGenerationFailure(TestCaseGenerationFailure* failure) {
        if (failure->type() == FailureType::VERIFICATION) {
            printVerificationFailure(((VerificationFailure*) failure)->verificationResult());
        }
    }

    void printVerificationFailure(const VerificationResult& result) {
        for (const auto& entry : result.unsatisfiedConstraintDescriptionsByConstraintGroupId()) {
            int constraintGroupId = entry.first;
            const vector<string>& unsatisfiedConstraintDescriptions = entry.second;

            if (constraintGroupId == -1) {
                cout << "    * Does not satisfy constraints, on:" << endl;
            } else {
                cout << "    * Does not satisfy subtask " << constraintGroupId << ", on constraints:" << endl;
            }

            for (const string& unsatisfiedConstraintDescription : unsatisfiedConstraintDescriptions) {
                cout << "       - " << unsatisfiedConstraintDescription << endl;
            }
        }
        for (int constraintGroupId : result.satisfiedButNotAssignedGroupIds()) {
            cout << "    * Satisfies subtask " << constraintGroupId << " but is not assigned to it" << endl;
        }
    }
 };

}
