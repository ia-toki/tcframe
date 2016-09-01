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
    ConstraintSuite() {}

    ConstraintSuite(const vector<Subtask>& constraints, const vector<Constraint>& multipleTestCasesConstraints)
            : constraints_(constraints)
            , multipleTestCasesConstraints_(multipleTestCasesConstraints) {}

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
    int curSubtaskId_;
    bool isInMultipleTestCasesConstraints_;
    vector<Constraint> curConstraints;

public:
    virtual ~ConstraintSuiteBuilder() {}

    ConstraintSuiteBuilder()
            : hasCurrentSubtask_(false)
            , curSubtaskId_(-1)
            , isInMultipleTestCasesConstraints_(false) {}

    ConstraintSuiteBuilder& newSubtask() {
        if (hasCurrentSubtask_) {
            addCurrentSubtask();
        } else {
            curSubtaskId_ = 0;
        }

        hasCurrentSubtask_ = true;
        curSubtaskId_++;
        curConstraints.clear();

        return *this;
    }

    ConstraintSuiteBuilder& prepareForMultipleTestCasesConstraints() {
        isInMultipleTestCasesConstraints_ = true;
        return *this;
    }

    ConstraintSuiteBuilder& addConstraint(function<bool()> predicate, string description) {
        if (isInMultipleTestCasesConstraints_) {
            subject_.multipleTestCasesConstraints_.push_back(Constraint(predicate, description));
        } else {
            curConstraints.push_back(Constraint(predicate, description));
        }
        return *this;
    }

    ConstraintSuite build() {
        if (!curConstraints.empty()) {
            addCurrentSubtask();
        }
        return move(subject_);
    }

private:
    void addCurrentSubtask() {
        subject_.constraints_.push_back(Subtask(curSubtaskId_, curConstraints));
    }
};

}
