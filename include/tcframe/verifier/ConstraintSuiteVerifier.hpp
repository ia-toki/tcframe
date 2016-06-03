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

    virtual VerificationResult verify(const set<int>& constraintGroupIds) {
        map<int, vector<string>> unsatisfiedConstraintDescriptionsByGroupId;
        set<int> satisfiedButNotAssignedGroupIds;

        for (const ConstraintGroup& constraintGroup : constraintSuite_.individualConstraints()) {
            vector<string> unsatisfiedConstraintDescriptions;
            for (const Constraint& constraint : constraintGroup.constraints()) {
                if (!constraint.predicate()()) {
                    unsatisfiedConstraintDescriptions.push_back(constraint.description());
                }
            }

            if (constraintGroupIds.count(constraintGroup.id())) {
                if (!unsatisfiedConstraintDescriptions.empty()) {
                    unsatisfiedConstraintDescriptionsByGroupId[constraintGroup.id()] = unsatisfiedConstraintDescriptions;
                }
            } else {
                if (unsatisfiedConstraintDescriptions.empty()) {
                    satisfiedButNotAssignedGroupIds.insert(constraintGroup.id());
                }
            }
        }
        return VerificationResult(unsatisfiedConstraintDescriptionsByGroupId, satisfiedButNotAssignedGroupIds);
    }
};

}
