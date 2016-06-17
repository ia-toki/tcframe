#pragma once

#include <tuple>
#include <utility>
#include <vector>

#include "Constraint.hpp"
#include "Subtask.hpp"

using std::move;
using std::tie;
using std::vector;

namespace tcframe {

struct ConstraintSuite {
    friend class ConstraintSuiteBuilder;

private:
    vector<Subtask> individualConstraints_;

public:
    const vector<Subtask>& individualConstraints() const {
        return individualConstraints_;
    }

    bool operator==(const ConstraintSuite& o) const {
        return tie(individualConstraints_) == tie(o.individualConstraints_);
    }
};

class ConstraintSuiteBuilder {
private:
    ConstraintSuite subject_;

    bool hasCurrentSubtask_;
    int currentSubtaskId_;
    vector<Constraint> currentIndividualConstraints_;

public:
    virtual ~ConstraintSuiteBuilder() {}

    ConstraintSuiteBuilder()
            : hasCurrentSubtask_(false)
            , currentSubtaskId_(0)
    {}

    ConstraintSuiteBuilder& newSubtask() {
        if (hasCurrentSubtask_) {
            subject_.individualConstraints_.push_back(Subtask(currentSubtaskId_, currentIndividualConstraints_));
        }

        hasCurrentSubtask_ = true;
        currentSubtaskId_++;
        currentIndividualConstraints_.clear();

        return *this;
    }

    ConstraintSuiteBuilder& addConstraint(Constraint constraint) {
        if (!hasCurrentSubtask_) {
            hasCurrentSubtask_ = true;
            currentSubtaskId_ = -1;
        }

        currentIndividualConstraints_.push_back(constraint);

        return *this;
    }

    ConstraintSuite build() {
        if (hasCurrentSubtask_) {
            subject_.individualConstraints_.push_back(Subtask(currentSubtaskId_, currentIndividualConstraints_));
        }
        return move(subject_);
    }

    ConstraintSuite buildWithoutLastSubtask() {
        return move(subject_);
    }
};

}
