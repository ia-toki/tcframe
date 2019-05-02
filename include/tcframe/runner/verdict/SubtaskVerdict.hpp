#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "Verdict.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct SubtaskVerdict {
private:
    Verdict verdict_;
    double points_;

public:
    SubtaskVerdict()
            : SubtaskVerdict(Verdict::ac(), 100.0) {}

    SubtaskVerdict(Verdict status, double points)
            : verdict_(move(status))
            , points_(points) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const double points() const {
        return points_;
    }

    bool operator==(const SubtaskVerdict& o) const {
        return tie(verdict_, points_) == tie(o.verdict_, o.points_);
    }

    string toBriefString() const {
        return verdict_.code() + " " + pointsToString();
    }

    string toString() const {
        string points = pointsToString();
        return verdict_.name() + " [" + pointsToString() + "]";
    }

private:
    string pointsToString() const {
        string points = StringUtils::toString(points_, 2);
        while (points.back() == '0') {
            points.pop_back();
        }
        if (points.back() == '.') {
            points.pop_back();
        }
        return points;
    }
};

}
