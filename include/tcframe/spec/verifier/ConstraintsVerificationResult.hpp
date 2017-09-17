#pragma once

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "tcframe/exception.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::move;
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
            map<int, vector<string>> unsatisfiedConstraintDescriptionsBySubtaskId,
            set<int> satisfiedButNotAssignedSubtaskIds)
            : unsatisfiedConstraintDescriptionsBySubtaskId_(move(unsatisfiedConstraintDescriptionsBySubtaskId))
            , satisfiedButNotAssignedSubtaskIds_(move(satisfiedButNotAssignedSubtaskIds)) {}

    ConstraintsVerificationResult() = default;

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

    FormattedError asFormattedError() const {
        vector<pair<int, string>> messages;
        for (const auto& entry : unsatisfiedConstraintDescriptionsBySubtaskId_) {
            int subtaskId = entry.first;
            const vector<string>& unsatisfiedConstraintDescriptions = entry.second;

            if (subtaskId == Subtask::MAIN_ID) {
                messages.emplace_back(0, "Does not satisfy constraints, on:");
            } else {
                string s = StringUtils::toString(subtaskId);
                messages.emplace_back(0, "Does not satisfy subtask " + s + ", on constraints:");
            }

            for (const string& unsatisfiedConstraintDescription : unsatisfiedConstraintDescriptions) {
                messages.emplace_back(1, unsatisfiedConstraintDescription);
            }
        }
        for (int subtaskId : satisfiedButNotAssignedSubtaskIds_) {
            string s = StringUtils::toString(subtaskId);
            messages.emplace_back(0, "Satisfies subtask " + s + " but is not assigned to it");
        }
        return FormattedError(messages);
    }
};

}
