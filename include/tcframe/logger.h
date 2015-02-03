#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include "exception.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe {

class Logger {
public:
    static void logIntroduction() {
        cout << "Generating test cases..." << endl;
    }

    static void logTestGroupHeader(int testGroupId) {
        cout << endl;

        if (testGroupId == 0) {
            cout << "[ SAMPLE TEST CASES ]" << endl;
        } else if (testGroupId != -1) {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    static void logTestCaseHeader(string name) {
        cout << "  " << name << ": " << flush;
    }

    static void logTestCaseSatisfiedResult() {
        cout << "OK" << endl;
    }

    static void logTestCaseFailedResult(string testCaseDescription) {
        cout << "FAILED" << endl;
        cout << "    Description: " << testCaseDescription << endl;
        cout << "    Reasons:" << endl;
    }

    static void logConstraintsUnsatisfiedException(ConstraintsUnsatisfiedException e) {

        for (SubtaskFailure* subtaskFailure : e.getSubtaskFailures()) {
            switch (subtaskFailure->getType()) {
                case SubtaskFailureType::UNSATISFIED_BUT_ASSIGNED:
                    logUnsatisfiedSubtaskFailure(static_cast<UnsatisfiedSubtaskFailure*>(subtaskFailure));
                    break;
                case SubtaskFailureType::SATISFIED_BUT_NOT_ASSIGNED:
                    logSatisfiedSubtaskFailure(static_cast<SatisfiedSubtaskFailure*>(subtaskFailure));
                    break;
            }
        }

        cout << endl;
    }

private:
    static void logUnsatisfiedSubtaskFailure(UnsatisfiedSubtaskFailure* subtaskFailure) {
        int subtaskId = subtaskFailure->getSubtaskId();
        vector<Constraint*> unsatisfiedConstraints = subtaskFailure->getUnsatisfiedConstraints();

        if (subtaskId == -1) {
            cout << "    * Does not satisfy constraints, on:" << endl;
        } else {
            cout << "    * Does not satisfy subtask " << subtaskId << ", on constraints:" << endl;
        }

        for (Constraint* unsatisfiedConstraint : unsatisfiedConstraints) {
            cout << "      - " << unsatisfiedConstraint->getDescription() << endl;
        }
    }

    static void logSatisfiedSubtaskFailure(SatisfiedSubtaskFailure* subtaskFailure) {
        int subtaskId = subtaskFailure->getSubtaskId();

        cout << "    * Satisfies subtask " << subtaskId << " but is not assigned to it" << endl;
    }
};

}

#endif
