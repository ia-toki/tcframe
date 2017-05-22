#pragma once

#include <string>
#include <tuple>

#include "VerdictStatus.hpp"
#include "tcframe/util.hpp"

using std::tie;

namespace tcframe {

struct Verdict {
private:
    VerdictStatus status_;
    optional<double> points_;

public:
    Verdict()
            : Verdict(VerdictStatus::ac()) {}

    Verdict(VerdictStatus status)
            : status_(status)
            , points_(optional<double>()) {}

    Verdict(VerdictStatus status, double points)
            : status_(status)
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

    string toString() const {
        return status_.name() + pointsToString();
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
        while (points.back() == '.') {
            points.pop_back();
        }
        return " [" + points + "]";
    }
};

}
