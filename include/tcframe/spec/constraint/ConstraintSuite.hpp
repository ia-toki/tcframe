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
    vector<Subtask> constraints_;
    vector<Constraint> multipleTestCasesConstraints_;

public:
    const vector<Subtask>& constraints() const {
        return constraints_;
    }

    const vector<Constraint>& multipleTestCasesConstraints() const {
        return multipleTestCasesConstraints_;
    }

    bool operator==(const ConstraintSuite& o) const {
        return tie(constraints_, multipleTestCasesConstraints_)
               == tie(o.constraints_, multipleTestCasesConstraints_);
    }
};

class ConstraintSuiteBuilder {
private:
    ConstraintSuite subject_;

    bool hasCurrentSubtask_;
    int currentSubtaskId_;
    bool isInMultipleTestCasesConstraints_;
    vector<Constraint> currentConstraints;

public:
    virtual ~ConstraintSuiteBuilder() {}

    ConstraintSuiteBuilder()
            : hasCurrentSubtask_(false)
            , currentSubtaskId_(0)
            , isInMultipleTestCasesConstraints_(false) {}

    ConstraintSuiteBuilder& newSubtask() {
        if (hasCurrentSubtask_) {
            subject_.constraints_.push_back(Subtask(currentSubtaskId_, currentConstraints));
        }

        hasCurrentSubtask_ = true;
        currentSubtaskId_++;
        currentConstraints.clear();

        return *this;
    }

    ConstraintSuiteBuilder& prepareForMultipleTestCasesConstraints() {
        isInMultipleTestCasesConstraints_ = true;
        return *this;
    }

    ConstraintSuiteBuilder& addConstraint(Constraint constraint) {
        if (isInMultipleTestCasesConstraints_) {
            subject_.multipleTestCasesConstraints_.push_back(constraint);
            return *this;
        }

        if (!hasCurrentSubtask_) {
            hasCurrentSubtask_ = true;
            currentSubtaskId_ = -1;
        }

        currentConstraints.push_back(constraint);

        return *this;
    }

    ConstraintSuite build() {
        if (hasCurrentSubtask_) {
            subject_.constraints_.push_back(Subtask(currentSubtaskId_, currentConstraints));
        }
        return move(subject_);
    }

    ConstraintSuite buildWithoutLastSubtask() {
        return move(subject_);
    }
};

}
