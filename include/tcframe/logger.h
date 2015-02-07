#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include "failure.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe {

class Logger {
public:
    virtual void logIntroduction() = 0;
    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(string testCaseName) = 0;
    virtual void logTestCaseOkResult() = 0;
    virtual void logTestCaseFailedResult(string testCaseDescription, vector<Failure*> failures) = 0;
};

class StandardLogger : public Logger {
public:
    void logIntroduction() override {
        cout << "Generating test cases..." << endl;
    }

    void logTestGroupIntroduction(int testGroupId) override {
        cout << endl;

        if (testGroupId == 0) {
            cout << "[ SAMPLE TEST CASES ]" << endl;
        } else if (testGroupId != -1) {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    void logTestCaseIntroduction(string testCaseName) override {
        cout << "  " << testCaseName << ": " << flush;
    }

    void logTestCaseOkResult() override {
        cout << "OK" << endl;
    }

    void logTestCaseFailedResult(string testCaseDescription, vector<Failure*> failures) override {
        cout << "FAILED" << endl;
        cout << "    Description: " << testCaseDescription << endl;
        cout << "    Reasons:" << endl;


        for (Failure* failure : failures) {
            switch (failure->getType()) {
                case FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED:
                    logFailure(static_cast<SubtaskSatisfiedButNotAssignedFailure*>(failure));
                    break;
                case FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED:
                    logFailure(static_cast<SubtaskUnsatisfiedButAssignedFailure*>(failure));
                    break;
            }
        }

        cout << endl;
    }

private:
    void logFailure(SubtaskSatisfiedButNotAssignedFailure* failure) {
        int subtaskId = failure->getSubtaskId();

        cout << "    * Satisfies subtask " << subtaskId << " but is not assigned to it" << endl;
    }

    void logFailure(SubtaskUnsatisfiedButAssignedFailure* failure) {
        int subtaskId = failure->getSubtaskId();
        vector<Constraint*> unsatisfiedConstraints = failure->getUnsatisfiedConstraints();

        if (subtaskId == -1) {
            cout << "    * Does not satisfy constraints, on:" << endl;
        } else {
            cout << "    * Does not satisfy subtask " << subtaskId << ", on constraints:" << endl;
        }

        for (Constraint* unsatisfiedConstraint : unsatisfiedConstraints) {
            cout << "      - " << unsatisfiedConstraint->getDescription() << endl;
        }
    }
};

}

#endif
