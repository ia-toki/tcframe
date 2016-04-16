#pragma once

#include <tuple>
#include <vector>

#include "tcframe/constraint/Constraint.hpp"

using std::tie;
using std::vector;

namespace tcframe {

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

    bool operator==(const ConstraintGroup& o) const {
        return tie(id_, constraints_) == tie(o.id_, o.constraints_);
    }
};

}
