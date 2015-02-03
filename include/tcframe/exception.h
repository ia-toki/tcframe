#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include "constraint.h"

#include <exception>

using std::exception;

namespace tcframe {

class NotImplementedException : public exception { };

enum SubtaskFailureType {
    SATISFIED_BUT_NOT_ASSIGNED,
    UNSATISFIED_BUT_ASSIGNED
};

class SubtaskFailure {
public:
    virtual SubtaskFailureType getType() = 0;

    virtual ~SubtaskFailure() { }
};


class SatisfiedSubtaskFailure : public SubtaskFailure {
private:
    int subtaskId;

public:
    SatisfiedSubtaskFailure(int subtaskId) :
        subtaskId(subtaskId) { }

    SubtaskFailureType getType() override {
        return SubtaskFailureType::SATISFIED_BUT_NOT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }
};

class UnsatisfiedSubtaskFailure : public SubtaskFailure {
private:
    int subtaskId;
    vector<Constraint*> unsatisfiedConstraints;

public:
    UnsatisfiedSubtaskFailure(int subtaskId, vector<Constraint*> unsatisfiedConstraints) :
        subtaskId(subtaskId), unsatisfiedConstraints(unsatisfiedConstraints) { }

    SubtaskFailureType getType() override {
        return SubtaskFailureType::UNSATISFIED_BUT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<Constraint*> getUnsatisfiedConstraints() {
        return unsatisfiedConstraints;
    }
};

class ConstraintsUnsatisfiedException : public exception {
private:
    vector<SubtaskFailure*> subtaskFailures;

public:
    ConstraintsUnsatisfiedException(vector<SubtaskFailure*> subtaskFailures) :
        subtaskFailures(subtaskFailures) { }

    vector<SubtaskFailure*> getSubtaskFailures() {
        return subtaskFailures;
    }
};

}

#endif
