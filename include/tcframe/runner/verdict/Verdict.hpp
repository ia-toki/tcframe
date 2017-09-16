#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "VerdictStatus.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct Verdict {
private:
    VerdictStatus status_;
    optional<double> points_;

public:
    Verdict()
            : Verdict(VerdictStatus::ac()) {}

    explicit Verdict(VerdictStatus status)
            : status_(move(status))
            , points_(optional<double>()) {}

    Verdict(VerdictStatus status, double points)
            : status_(move(status))
            , points_(optional<double>(points)) {}

    const VerdictStatus& status() const {
        return status_;
    }

    const optional<double>& points() const {
        return points_;
    }

    bool operator==(const Verdict& o) const {
        return tie(status_, points_) == tie(o.status_, o.points_);
    }

    string toBriefString() const {
        string points = pointsToString();
        if (!points.empty()) {
            points = " " + points;
        }
        return status_.code() + points;
    }

    string toString() const {
        string points = pointsToString();
        if (!points.empty()) {
            points = " [" + points + "]";
        }
        return status_.name() + points;
    }

private:
    string pointsToString() const {
        if (!points_) {
            return "";
        }
        string points = StringUtils::toString(points_.value(), 2);
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
