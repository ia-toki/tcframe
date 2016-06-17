#pragma once

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "VerificationResult.hpp"
#include "tcframe/constraint.hpp"

using std::map;
using std::move;
using std::string;
using std::vector;

namespace tcframe {

class ConstraintSuiteVerifier {
private:
    ConstraintSuite constraintSuite_;

public:
    virtual ~ConstraintSuiteVerifier() {}

    ConstraintSuiteVerifier(ConstraintSuite constraintSuite)
            : constraintSuite_(move(constraintSuite))
    {}

    virtual VerificationResult verify(const set<int>& subtaskIds) {
        map<int, vector<string>> unsatisfiedConstraintDescriptionsBySubtaskId;
        set<int> satisfiedButNotAssignedSubtaskIds;

        for (const Subtask& subtask : constraintSuite_.individualConstraints()) {
            vector<string> unsatisfiedConstraintDescriptions;
            for (const Constraint& constraint : subtask.constraints()) {
                if (!constraint.predicate()()) {
                    unsatisfiedConstraintDescriptions.push_back(constraint.description());
                }
            }

            if (subtaskIds.count(subtask.id())) {
                if (!unsatisfiedConstraintDescriptions.empty()) {
                    unsatisfiedConstraintDescriptionsBySubtaskId[subtask.id()] = unsatisfiedConstraintDescriptions;
                }
            } else {
                if (unsatisfiedConstraintDescriptions.empty()) {
                    satisfiedButNotAssignedSubtaskIds.insert(subtask.id());
                }
            }
        }
        return VerificationResult(unsatisfiedConstraintDescriptionsBySubtaskId, satisfiedButNotAssignedSubtaskIds);
    }
};

}
