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

    static constexpr double MAIN_POINTS = 100;

private:
    int id_;
    vector<Constraint> constraints_;
    double points_;

public:
    Subtask() {}

    Subtask(int id, const vector<Constraint>& constraints)
            : id_(id)
            , constraints_(constraints)
            , points_(0) {}

    Subtask(int id, double points, const vector<Constraint>& constraints)
            : id_(id)
            , points_(points)
            , constraints_(constraints) {}

    int id() const {
        return id_;
    }

    double points() const {
        return points_;
    }

    const vector<Constraint>& constraints() const {
        return constraints_;
    }

    bool operator==(const Subtask& o) const {
        return tie(id_, points_, constraints_) == tie(o.id_, o.points_, o.constraints_);
    }
};

}
