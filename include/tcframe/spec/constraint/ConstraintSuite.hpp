#pragma once

#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "Constraint.hpp"
#include "Subtask.hpp"

using std::move;
using std::set;
using std::tie;
using std::vector;

namespace tcframe {

struct ConstraintSuite {
    friend class ConstraintSuiteBuilder;

private:
    vector<Subtask> constraints_;
    vector<Constraint> multipleTestCasesConstraints_;

public:
    ConstraintSuite() = default;

    ConstraintSuite(vector<Subtask> constraints, vector<Constraint> multipleTestCasesConstraints)
            : constraints_(move(constraints))
            , multipleTestCasesConstraints_(move(multipleTestCasesConstraints)) {}

    const vector<Subtask>& constraints() const {
        return constraints_;
    }

    const vector<Constraint>& multipleTestCasesConstraints() const {
        return multipleTestCasesConstraints_;
    }

    bool operator==(const ConstraintSuite& o) const {
        return tie(constraints_, multipleTestCasesConstraints_) == tie(o.constraints_, multipleTestCasesConstraints_);
    }

    bool hasSubtasks() const {
        return constraints_.size() > 1;
    }

    set<int> getDefinedSubtaskIds() const {
        set<int> definedSubtaskIds;
        for (const Subtask& subtask : constraints_) {
            if (subtask.id() != Subtask::MAIN_ID) {
                definedSubtaskIds.insert(subtask.id());
            }
        }
        return definedSubtaskIds;
    }
};

class ConstraintSuiteBuilder {
private:
    ConstraintSuite subject_;

    bool hasCurrentSubtask_ = false;
    int curSubtaskId_ = Subtask::MAIN_ID;
    double curPoints_ = 0;
    bool isInMultipleTestCasesConstraints_ = false;
    vector<Constraint> curConstraints_;

public:
    virtual ~ConstraintSuiteBuilder() = default;

    ConstraintSuiteBuilder& newSubtask() {
        addCurrentSubtask();

        if (!hasCurrentSubtask_) {
            curSubtaskId_ = 0;
        }

        hasCurrentSubtask_ = true;
        curSubtaskId_++;
        curConstraints_.clear();
        curPoints_ = 0;

        return *this;
    }

    ConstraintSuiteBuilder& Points(double points) {
        curPoints_ = points;
        return *this;
    }

    ConstraintSuiteBuilder& prepareForMultipleTestCasesConstraints() {
        isInMultipleTestCasesConstraints_ = true;
        return *this;
    }

    ConstraintSuiteBuilder& addConstraint(function<bool()> predicate, string description) {
        if (isInMultipleTestCasesConstraints_) {
            subject_.multipleTestCasesConstraints_.emplace_back(predicate, description);
        } else {
            curConstraints_.emplace_back(predicate, description);
        }
        return *this;
    }

    ConstraintSuite build() {
        if (subject_.constraints_.empty() || !curConstraints_.empty()) {
            addCurrentSubtask();
        }
        if (subject_.constraints_.size() == 1) {
            assignMainSubtaskPoints();
        }
        return move(subject_);
    }

private:
    void addCurrentSubtask() {
        subject_.constraints_.emplace_back(curSubtaskId_, curPoints_, curConstraints_);
    }

    void assignMainSubtaskPoints() {
        Subtask mainSubtask = subject_.constraints_.back();
        subject_.constraints_.pop_back();
        subject_.constraints_.emplace_back(Subtask::MAIN_ID, double(Subtask::MAIN_POINTS), mainSubtask.constraints());
    }
};

}
