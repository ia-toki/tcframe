#pragma once

#include "tcframe/deprecated/problem/Constraint.hpp"

#include <vector>

using std::vector;

namespace tcframe_old {

class Subtask {
public:
    Subtask(int id)
            : id(id) { }

    void addConstraint(Constraint* constraint) {
        constraints.push_back(constraint);
    }

    int getId() {
        return id;
    }

    vector<Constraint*> getConstraints() {
        return constraints;
    }

private:
    int id;
    vector<Constraint*> constraints;
};

}
