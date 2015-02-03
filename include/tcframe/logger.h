#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include "unsatisfiability.h"

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

    static void logTestCaseUnsatisfiedResult(string description, vector<Unsatisfiability*> unsatisfiabilities) {
        cout << "FAILED" << endl;
        cout << "    Description: " << description << endl;
        cout << "    Reasons:" << endl;

        for (Unsatisfiability* unsatisfiability : unsatisfiabilities) {
            switch (unsatisfiability->getType()) {
                case UnsatisfiabilityType::UNSATISFIED_SUBTASK:
                    logUnsatisfiedSubtaskUnsatisfiability(static_cast<UnsatisfiedSubtaskUnsatisfiability*>(unsatisfiability));
                    break;
                case UnsatisfiabilityType::SATISFIED_SUBTASK_BUT_NOT_ASSIGNED:
                    logSatisfiedSubtaskButNotAssignedUnsatisfiability(static_cast<SatisfiedSubtaskButNotAssignedUnsatisfiability*>(unsatisfiability));
                    break;
            }
        }

        cout << endl;
    }

private:
    static void logUnsatisfiedSubtaskUnsatisfiability(UnsatisfiedSubtaskUnsatisfiability* unsatisfiability) {
        int subtaskId = unsatisfiability->getSubtaskId();
        vector<Constraint*> unsatisfiedConstraints = unsatisfiability->getUnsatisfiedConstraints();

        if (subtaskId == -1) {
            cout << "    * Does not satisfy constraints, on:" << endl;
        } else {
            cout << "    * Does not satisfy subtask " << subtaskId << ", on constraints:" << endl;
        }

        for (Constraint* unsatisfiedConstraint : unsatisfiedConstraints) {
            cout << "      - " << unsatisfiedConstraint->getDescription() << endl;
        }
    }

    static void logSatisfiedSubtaskButNotAssignedUnsatisfiability(SatisfiedSubtaskButNotAssignedUnsatisfiability* unsatisfiability) {
        int subtaskId = unsatisfiability->getSubtaskId();

        cout << "    * Satisfies subtask " << subtaskId << " but is not assigned to it" << endl;
    }
};

}

#endif
