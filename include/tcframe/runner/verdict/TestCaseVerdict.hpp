#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "Verdict.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct TestCaseVerdict {
private:
    Verdict verdict_;
    optional<double> points_;

public:
    TestCaseVerdict()
            : TestCaseVerdict(Verdict::ac()) {}

    explicit TestCaseVerdict(Verdict status)
            : verdict_(move(status))
            , points_(optional<double>()) {}

    TestCaseVerdict(Verdict status, double points)
            : verdict_(move(status))
            , points_(optional<double>(points)) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const optional<double>& points() const {
        return points_;
    }

    bool operator==(const TestCaseVerdict& o) const {
        return tie(verdict_, points_) == tie(o.verdict_, o.points_);
    }

    string toBriefString() const {
        string points = pointsToString();
        if (!points.empty()) {
            points = " " + points;
        }
        return verdict_.code() + points;
    }

    string toString() const {
        string points = pointsToString();
        if (!points.empty()) {
            points = " [" + points + "]";
        }
        return verdict_.name() + points;
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
