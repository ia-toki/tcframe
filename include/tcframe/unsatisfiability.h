#ifndef TCFRAME_UNSATISFIABILITY_H
#define TCFRAME_UNSATISFIABILITY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe {

enum UnsatisfiabilityType {
    UNSATISFIED_SUBTASK,
    SATISFIED_SUBTASK_BUT_NOT_ASSIGNED
};

class Unsatisfiability {
public:
    virtual UnsatisfiabilityType getType() = 0;

    virtual ~Unsatisfiability() { }
};

class UnsatisfiedSubtaskUnsatisfiability : public Unsatisfiability {
private:
    int subtaskId;
    vector<Constraint*> unsatisfiedConstraints;
public:
    UnsatisfiedSubtaskUnsatisfiability(int subtaskId, vector<Constraint*> unsatisfiedConstraints) :
        subtaskId(subtaskId), unsatisfiedConstraints(unsatisfiedConstraints) { }

    UnsatisfiabilityType getType() override {
        return UnsatisfiabilityType::UNSATISFIED_SUBTASK;
    }

    int getSubtaskId() {
        return subtaskId;
    }

    vector<Constraint*> getUnsatisfiedConstraints() {
        return unsatisfiedConstraints;
    }
};

class SatisfiedSubtaskButNotAssignedUnsatisfiability : public Unsatisfiability {
private:
    int subtaskId;

public:
    SatisfiedSubtaskButNotAssignedUnsatisfiability(int subtaskId) :
        subtaskId(subtaskId) { }

    UnsatisfiabilityType getType() override {
        return UnsatisfiabilityType::SATISFIED_SUBTASK_BUT_NOT_ASSIGNED;
    }

    int getSubtaskId() {
        return subtaskId;
    }
};

class UnsatisfiabilitiesCollector {
private:
    vector<Unsatisfiability*> unsatisfiabilities;

public:
    void addUnsatisfiability(Unsatisfiability* unsatisfiability) {
        unsatisfiabilities.push_back(unsatisfiability);
    }

    bool hasUnsatisfiabilities() {
        return !unsatisfiabilities.empty();
    }

    vector<Unsatisfiability*> collectUnsatisfiabilities() {
        return unsatisfiabilities;
    }
};

}

#endif
