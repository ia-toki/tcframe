#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.hpp"
#include "util.hpp"

#include <string>

using std::string;

namespace tcframe {

class FailureDescriptionItem {
public:
    FailureDescriptionItem(string message, int level)
            : message(message), level(level) { }

    string getMessage() {
        return message;
    }

    int getLevel() {
        return level;
    }

private:
    string message;
    int level;
};

class TestCaseFailure {
public:
    virtual ~TestCaseFailure() { }

    virtual vector<FailureDescriptionItem> getDescription() = 0;
};

class SubtaskUnsatisfiability {
public:
    virtual ~SubtaskUnsatisfiability() { }

    virtual vector<FailureDescriptionItem> getDescription() = 0;
};

class SubtaskSatisfiedButNotAssigned : public SubtaskUnsatisfiability {
public:
    SubtaskSatisfiedButNotAssigned(int subtaskId)
            : subtaskId(subtaskId) { }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<FailureDescriptionItem> getDescription() override {
        return vector<FailureDescriptionItem>{
                FailureDescriptionItem("Satisfies subtask " + Util::toString(subtaskId) + " but is not assigned to it", 0)
        };
    };

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

    vector<FailureDescriptionItem> getDescription() override {
        vector<FailureDescriptionItem> unsatisfiabilities;

        if (subtaskId == -1) {
            unsatisfiabilities.push_back(FailureDescriptionItem("Does not satisfy constraints, on:", 0));
        } else {
            unsatisfiabilities.push_back(FailureDescriptionItem("Does not satisfy subtask " + Util::toString(subtaskId) + ", on constraints:", 0));
        }

        for (Constraint* constraint : unsatisfiedConstraints) {
            unsatisfiabilities.push_back(FailureDescriptionItem(constraint->getDescription(), 1));
        }

        return unsatisfiabilities;
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

    vector<FailureDescriptionItem> getDescription() override {
        vector<FailureDescriptionItem> description;
        for (SubtaskUnsatisfiability* unsatisfiability : unsatisfiabilities) {
            vector<FailureDescriptionItem> description2 = unsatisfiability->getDescription();
            description.insert(description.begin(), description2.begin(), description2.end());
        }

        return description;
    }

private:
    vector<SubtaskUnsatisfiability*> unsatisfiabilities;
};

class IOFormatFailure : public TestCaseFailure {
public:
    IOFormatFailure(string message)
            : message(message) { }

    vector<FailureDescriptionItem> getDescription() override {
        return vector<FailureDescriptionItem>{
                FailureDescriptionItem(message, 0)
        };
    };

    string getMessage() {
        return message;
    }

private:
    string message;
};

class IOSegmentFailure : public TestCaseFailure {
public:
    IOSegmentFailure(string message)
            : message(message) { }

    vector<FailureDescriptionItem> getDescription() override {
        return vector<FailureDescriptionItem>{
                FailureDescriptionItem(message, 0)
        };
    };

    string getMessage() {
        return message;
    }

private:
    string message;
};

class TypeFailure : public TestCaseFailure {
public:
    TypeFailure(string message)
            : message(message) { }

    vector<FailureDescriptionItem> getDescription() override {
        return vector<FailureDescriptionItem>{
                FailureDescriptionItem(message, 0)
        };
    };

    string getMessage() {
        return message;
    }

private:
    string message;
};

}

#endif