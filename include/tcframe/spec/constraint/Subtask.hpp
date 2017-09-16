#pragma once

#include <tuple>
#include <utility>
#include <vector>

#include "Constraint.hpp"

using std::move;
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
    Subtask() = default;

    Subtask(int id, vector<Constraint> constraints)
            : id_(id)
            , constraints_(move(constraints))
            , points_(0) {}

    Subtask(int id, double points, vector<Constraint> constraints)
            : id_(id)
            , points_(points)
            , constraints_(move(constraints)) {}

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
