#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.hpp"
#include "util.hpp"

#include <string>

using std::string;

namespace tcframe {

class TestCaseFailure {
public:
    virtual ~TestCaseFailure() { }

    virtual string toString() = 0;
};

class SubtaskUnsatisfiability {
public:
    virtual ~SubtaskUnsatisfiability() { }

    virtual string getUnsatisfiability() = 0;
};

class SubtaskSatisfiedButNotAssigned : public SubtaskUnsatisfiability {
public:
    SubtaskSatisfiedButNotAssigned(int subtaskId)
            : subtaskId(subtaskId) { }

    int getSubtaskId() {
        return subtaskId;
    }

    string getUnsatisfiability() override {
        return "* Satisfies subtask " + Util::toString(subtaskId) + " but is not assigned to it";
    }

private:
    int subtaskId;
};

class SubtaskUnsatisfiedButAssigned : public SubtaskUnsatisfiability {
public:
    SubtaskUnsatisfiedButAssigned(int subtaskId, vector<Constraint*> unsatisfiedConstraints)
            : subtaskId(subtaskId), unsatisfiedConstraints(unsatisfiedConstraints) { }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<Constraint*> getUnsatisfiedConstraints() {
        return unsatisfiedConstraints;
    }

    string getUnsatisfiability() override {
        string res;

        if (subtaskId == -1) {
            res += "    * Does not satisfy constraints, on:\n";
        } else {
            res += "    * Does not satisfy subtask " + Util::toString(subtaskId) + ", on constraints:\n";
        }

        for (Constraint* constraint : unsatisfiedConstraints) {
            res += "      -" + constraint->getDescription() + "\n";
        }

        return res;
    }

private:
    int subtaskId;
    vector<Constraint*> unsatisfiedConstraints;
};

class SubtaskFailure : public TestCaseFailure {
public:
    SubtaskFailure(vector<SubtaskUnsatisfiability*> unsatisfiabilities)
            : unsatisfiabilities(unsatisfiabilities) { }

    vector<SubtaskUnsatisfiability*> getUnsatisfiabilities() {
        return unsatisfiabilities;
    }

    string toString() override {
        string res;
        for (SubtaskUnsatisfiability* unsatisfiability : unsatisfiabilities) {
            res += unsatisfiability->getUnsatisfiability();
        }

        return res;
    }

private:
    vector<SubtaskUnsatisfiability*> unsatisfiabilities;
};

class IOFormatFailure : public TestCaseFailure {
public:
    IOFormatFailure(string message)
            : message(message) { }

    string toString() override {
        return message;
    }

private:
    string message;
};

}

#endif