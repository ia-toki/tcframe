#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ConstraintsVerificationResult.hpp"
#include "MultipleTestCasesConstraintsVerificationResult.hpp"
#include "tcframe/spec/constraint.hpp"

using std::map;
using std::move;
using std::string;
using std::vector;

namespace tcframe {

class Verifier {
private:
    ConstraintSuite constraintSuite_;

public:
    virtual ~Verifier() = default;

    explicit Verifier(ConstraintSuite constraintSuite)
            : constraintSuite_(move(constraintSuite)) {}

    virtual ConstraintsVerificationResult verifyConstraints(const set<int>& subtaskIds) {
        map<int, vector<string>> unsatisfiedConstraintDescriptionsBySubtaskId;
        set<int> satisfiedButNotAssignedSubtaskIds;

        for (const Subtask& subtask : constraintSuite_.constraints()) {
            vector<string> unsatisfiedConstraintDescriptions;
            for (const Constraint& constraint : subtask.constraints()) {
                if (!constraint.predicate()()) {
                    unsatisfiedConstraintDescriptions.push_back(constraint.description());
                }
            }

            if (subtask.id() == Subtask::MAIN_ID || subtaskIds.count(subtask.id())) {
                if (!unsatisfiedConstraintDescriptions.empty()) {
                    unsatisfiedConstraintDescriptionsBySubtaskId[subtask.id()] = unsatisfiedConstraintDescriptions;
                }
            } else {
                if (unsatisfiedConstraintDescriptions.empty()) {
                    satisfiedButNotAssignedSubtaskIds.insert(subtask.id());
                }
            }
        }
        return {unsatisfiedConstraintDescriptionsBySubtaskId, satisfiedButNotAssignedSubtaskIds};
    }

    virtual MultipleTestCasesConstraintsVerificationResult verifyMultipleTestCasesConstraints() {
        set<string> unsatisfiedConstraintDescriptions;
        for (const Constraint& constraint : constraintSuite_.multipleTestCasesConstraints()) {
            if (!constraint.predicate()()) {
                unsatisfiedConstraintDescriptions.insert(constraint.description());
            }
        }
        return MultipleTestCasesConstraintsVerificationResult(unsatisfiedConstraintDescriptions);
    }
};

}
