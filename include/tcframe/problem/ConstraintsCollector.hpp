#pragma once

#include "tcframe/problem/Constraint.hpp"
#include "tcframe/problem/Subtask.hpp"

#include <functional>
#include <string>
#include <vector>

using std::function;
using std::string;
using std::vector;

namespace tcframe {

class ConstraintsCollector {
public:
    void newSubtask() {
        curSubtaskId++;
        subtasks.push_back(new Subtask(curSubtaskId));
    }

    void setMultipleTestCasesMode() {
        this->multipleTestCasesMode = true;
    }

    void addConstraint(function<bool()> predicate, string description) {
        if (multipleTestCasesMode) {
            multipleTestCasesConstraints.push_back(new Constraint(predicate, description));
        } else {
            if (subtasks.empty()) {
                subtasks.push_back(new Subtask(-1));
            }

            subtasks.back()->addConstraint(new Constraint(predicate, description));
        }
    }

    vector<Subtask*> collectSubtasks() {
        return subtasks;
    }

    vector<Constraint*> collectMultipleTestCasesConstraints() {
        return multipleTestCasesConstraints;
    }

private:
    bool multipleTestCasesMode = false;
    int curSubtaskId = 0;
    vector<Subtask*> subtasks;
    vector<Constraint*> multipleTestCasesConstraints;
};

}
