#pragma once

#include <vector>

#include "tcframe/constraint/Constraint.hpp"

using std::vector;

namespace tcframe { namespace experimental {

class ConstraintGroup {
private:
    int id_;
    vector<Constraint> constraints_;

public:
    ConstraintGroup(int id, const vector<Constraint>& constraints)
            : id_(id)
            , constraints_(constraints)
    {}

    int id() const {
        return id_;
    }

    const vector<Constraint>& constraints() const {
        return constraints_;
    }
};

}}
