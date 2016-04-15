#pragma once

#include <utility>
#include <vector>

#include "tcframe/experimental/constraint/Constraint.hpp"

using std::move;
using std::vector;

namespace tcframe { namespace experimental {

class ConstraintGroup {
private:
    int id_;
    vector<Constraint> constraints_;

public:
    ConstraintGroup(int id, vector<Constraint> constraints)
            : id_(id)
            , constraints_(move(constraints))
    {}

    int id() const {
        return id_;
    }

    const vector<Constraint>& constraints() const {
        return constraints_;
    }
};

}}
