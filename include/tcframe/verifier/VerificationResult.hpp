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

struct VerificationResult {
private:
    map<int, vector<string>> unsatisfiedConstraintDescriptionsByGroupId_;
    set<int> satisfiedButNotAssignedGroupIds_;

public:
    VerificationResult(
            const map<int, vector<string>>& unsatisfiedConstraintDescriptionsByGroupId,
            const set<int>& satisfiedButNotAssignedGroupIds)
            : unsatisfiedConstraintDescriptionsByGroupId_(unsatisfiedConstraintDescriptionsByGroupId)
            , satisfiedButNotAssignedGroupIds_(satisfiedButNotAssignedGroupIds)
    {}

    static VerificationResult validResult() {
        return VerificationResult({}, {});
    }

    bool isValid() const {
        return unsatisfiedConstraintDescriptionsByGroupId_.empty() &&
               satisfiedButNotAssignedGroupIds_.empty();
    }

    const map<int, vector<string>>& unsatisfiedConstraintDescriptionsByConstraintGroupId() const {
        return unsatisfiedConstraintDescriptionsByGroupId_;
    }

    const set<int>& satisfiedButNotAssignedGroupIds() const {
        return satisfiedButNotAssignedGroupIds_;
    }

    bool operator==(const VerificationResult& o) const {
        return tie(unsatisfiedConstraintDescriptionsByGroupId_, satisfiedButNotAssignedGroupIds_)
                == tie(o.unsatisfiedConstraintDescriptionsByGroupId_, o.satisfiedButNotAssignedGroupIds_);
    }
};

}
