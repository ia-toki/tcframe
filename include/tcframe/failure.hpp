#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.hpp"

#include <vector>

using std::vector;

namespace tcframe {

enum FailureType {
    SUBTASK_SATISFIED_BUT_NOT_ASSIGNED,
    SUBTASK_UNSATISFIED_BUT_ASSIGNED
};

class Failure {
public:
    virtual ~Failure() { }
    
    virtual FailureType getType() = 0;
};


class SubtaskSatisfiedButNotAssignedFailure : public Failure {
public:
    SubtaskSatisfiedButNotAssignedFailure(int subtaskId)
            : subtaskId(subtaskId) { }

    FailureType getType() override {
        return FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }

private:
    int subtaskId;
};

class SubtaskUnsatisfiedButAssignedFailure : public Failure {
public:
    SubtaskUnsatisfiedButAssignedFailure(int subtaskId, vector<Constraint*> unsatisfiedConstraints)
            : subtaskId(subtaskId), unsatisfiedConstraints(unsatisfiedConstraints) { }

    FailureType getType() override {
        return FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<Constraint*> getUnsatisfiedConstraints() {
        return unsatisfiedConstraints;
    }

private:
    int subtaskId;
    vector<Constraint*> unsatisfiedConstraints;
};

}

#endif
