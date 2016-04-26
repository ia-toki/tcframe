#pragma once

#include <utility>
#include <vector>

#include "Constraint.hpp"
#include "ConstraintGroup.hpp"

using std::move;
using std::vector;

namespace tcframe {

struct ConstraintSuite {
    friend class ConstraintSuiteBuilder;

private:
    vector<ConstraintGroup> individualConstraints_;

public:
    const vector<ConstraintGroup>& individualConstraints() const {
        return individualConstraints_;
    }
};

class ConstraintSuiteBuilder {
private:
    ConstraintSuite subject_;

    bool hasCurrentConstraintGroup_;
    int currentConstraintGroupId_;
    vector<Constraint> currentIndividualConstraints_;

public:
    virtual ~ConstraintSuiteBuilder() {}

    ConstraintSuiteBuilder()
            : hasCurrentConstraintGroup_(false)
            , currentConstraintGroupId_(0)
    {}

    ConstraintSuiteBuilder& newConstraintGroup() {
        if (hasCurrentConstraintGroup_) {
            subject_.individualConstraints_.push_back(ConstraintGroup(currentConstraintGroupId_, currentIndividualConstraints_));
        }

        hasCurrentConstraintGroup_ = true;
        currentConstraintGroupId_++;
        currentIndividualConstraints_.clear();

        return *this;
    }

    ConstraintSuiteBuilder& addConstraint(Constraint constraint) {
        if (!hasCurrentConstraintGroup_) {
            hasCurrentConstraintGroup_ = true;
            currentConstraintGroupId_ = -1;
        }

        currentIndividualConstraints_.push_back(constraint);

        return *this;
    }

    ConstraintSuite build() {
        if (hasCurrentConstraintGroup_) {
            subject_.individualConstraints_.push_back(ConstraintGroup(currentConstraintGroupId_, currentIndividualConstraints_));
        }
        return move(subject_);
    }

    ConstraintSuite buildWithoutLastConstraintGroup() {
        return move(subject_);
    }
};

}
