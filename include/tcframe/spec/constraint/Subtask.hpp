#pragma once

#include <tuple>
#include <vector>

#include "Constraint.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct Subtask {
    enum {
        MAIN_ID = -1
    };

private:
    int id_;
    vector<Constraint> constraints_;

public:
    Subtask() {}

    Subtask(int id, const vector<Constraint>& constraints)
            : id_(id)
            , constraints_(constraints) {}

    int id() const {
        return id_;
    }

    const vector<Constraint>& constraints() const {
        return constraints_;
    }

    bool operator==(const Subtask& o) const {
        return tie(id_, constraints_) == tie(o.id_, o.constraints_);
    }
};

}
