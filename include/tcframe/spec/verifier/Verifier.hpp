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

    // Validator (SPEC.md T7.1): the main constraints, which every input must satisfy.
    virtual ConstraintsVerificationResult verifyMainConstraints() {
        vector<string> unsatisfiedConstraintDescriptions;
        for (const Subtask& subtask : constraintSuite_.constraints()) {
            if (subtask.id() != Subtask::MAIN_ID) {
                continue;
            }
            for (const Constraint& constraint : subtask.constraints()) {
                if (!constraint.predicate()()) {
                    unsatisfiedConstraintDescriptions.push_back(constraint.description());
                }
            }
        }

        map<int, vector<string>> unsatisfiedConstraintDescriptionsBySubtaskId;
        if (!unsatisfiedConstraintDescriptions.empty()) {
            unsatisfiedConstraintDescriptionsBySubtaskId[Subtask::MAIN_ID] = unsatisfiedConstraintDescriptions;
        }
        return {unsatisfiedConstraintDescriptionsBySubtaskId, {}};
    }

    // Validator (SPEC.md T7.1): ids of the subtasks whose own constraints all hold.
    // The main constraints are not included; check them with verifyMainConstraints().
    virtual set<int> getSatisfiedSubtaskIds() {
        set<int> satisfiedSubtaskIds;
        for (const Subtask& subtask : constraintSuite_.constraints()) {
            if (subtask.id() == Subtask::MAIN_ID) {
                continue;
            }
            bool satisfied = true;
            for (const Constraint& constraint : subtask.constraints()) {
                if (!constraint.predicate()()) {
                    satisfied = false;
                    break;
                }
            }
            if (satisfied) {
                satisfiedSubtaskIds.insert(subtask.id());
            }
        }
        return satisfiedSubtaskIds;
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
