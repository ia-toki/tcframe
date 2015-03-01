#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.h"

#include <vector>

using std::vector;

namespace tcframe {

enum FailureType {
    SUBTASK_SATISFIED_BUT_NOT_ASSIGNED,
    SUBTASK_UNSATISFIED_BUT_ASSIGNED
};

class Failure {
public:
    virtual FailureType getType() = 0;

    virtual ~Failure() { }
};


class SubtaskSatisfiedButNotAssignedFailure : public Failure {
private:
    int subtaskId;

public:
    SubtaskSatisfiedButNotAssignedFailure(int subtaskId) :
        subtaskId(subtaskId) { }

    FailureType getType() override {
        return FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }
};

class SubtaskUnsatisfiedButAssignedFailure : public Failure {
private:
    int subtaskId;
    vector<Constraint*> unsatisfiedConstraints;

public:
    SubtaskUnsatisfiedButAssignedFailure(int subtaskId, vector<Constraint*> unsatisfiedConstraints) :
        subtaskId(subtaskId), unsatisfiedConstraints(unsatisfiedConstraints) { }

    FailureType getType() override {
        return FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<Constraint*> getUnsatisfiedConstraints() {
        return unsatisfiedConstraints;
    }
};

}

#endif
