#pragma once

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "tcframe/constraint.hpp"

using std::map;
using std::set;
using std::string;
using std::tie;
using std::vector;

namespace tcframe {

struct ConstraintsVerificationResult {
private:
    map<int, vector<string>> unsatisfiedConstraintDescriptionsBySubtaskId_;
    set<int> satisfiedButNotAssignedSubtaskIds_;

public:
    ConstraintsVerificationResult(
            const map<int, vector<string>>& unsatisfiedConstraintDescriptionsBySubtaskId,
            const set<int>& satisfiedButNotAssignedSubtaskIds)
            : unsatisfiedConstraintDescriptionsBySubtaskId_(unsatisfiedConstraintDescriptionsBySubtaskId)
            , satisfiedButNotAssignedSubtaskIds_(satisfiedButNotAssignedSubtaskIds)
    {}

    static ConstraintsVerificationResult validResult() {
        return ConstraintsVerificationResult({}, {});
    }

    bool isValid() const {
        return unsatisfiedConstraintDescriptionsBySubtaskId_.empty() &&
               satisfiedButNotAssignedSubtaskIds_.empty();
    }

    const map<int, vector<string>>& unsatisfiedConstraintDescriptionsBySubtaskId() const {
        return unsatisfiedConstraintDescriptionsBySubtaskId_;
    }

    const set<int>& satisfiedButNotAssignedSubtaskIds() const {
        return satisfiedButNotAssignedSubtaskIds_;
    }

    bool operator==(const ConstraintsVerificationResult& o) const {
        return tie(unsatisfiedConstraintDescriptionsBySubtaskId_, satisfiedButNotAssignedSubtaskIds_)
                == tie(o.unsatisfiedConstraintDescriptionsBySubtaskId_, o.satisfiedButNotAssignedSubtaskIds_);
    }
};

}
