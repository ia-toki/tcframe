#ifndef TCFRAME_CONSTRAINT_H
#define TCFRAME_CONSTRAINT_H

#include <functional>
#include <string>
#include <vector>

using std::function;
using std::string;
using std::vector;

namespace tcframe {

struct Constraint {
    int id;
    function<bool()> predicate;
    string description;
};

struct Subtask {
    int id;
    vector<Constraint*> constraints;
};

class ConstraintsCollector {
private:
    int curSubtaskNo;
    int curConstraintNo;

    vector<Subtask*> subtasks;

public:
    ConstraintsCollector() :curSubtaskNo(0), curConstraintNo(0) { }

    void newSubtask() {
        curSubtaskNo++;
        curConstraintNo = 0;
        subtasks.push_back(new Subtask{curSubtaskNo});
    }

    void addConstraint(function<bool()> predicate, string description) {
        if (!curSubtaskNo) {
            subtasks.push_back(new Subtask{0});
        }

        curConstraintNo++;
        subtasks.back()->constraints.push_back(new Constraint{curConstraintNo, predicate, description});
    }

    vector<Subtask*> collectSubtasks() {
        return subtasks;
    }
};

}

#endif