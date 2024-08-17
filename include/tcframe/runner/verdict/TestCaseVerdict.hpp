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
    optional<double> percentage_;

public:
    TestCaseVerdict()
            : TestCaseVerdict(Verdict::ac()) {}

    explicit TestCaseVerdict(Verdict status)
            : verdict_(move(status))
            , points_(optional<double>()) {}

    TestCaseVerdict(Verdict status, double points)
            : verdict_(move(status))
            , points_(optional<double>(points)) {}

    TestCaseVerdict(Verdict status, optional<double> points, optional<double> percentage)
            : verdict_(move(status))
            , points_(move(points))
            , percentage_(move(percentage)) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const optional<double>& points() const {
        return points_;
    }

    const optional<double>& percentage() const {
        return percentage_;
    }

    bool operator==(const TestCaseVerdict& o) const {
        return tie(verdict_, points_, percentage_) == tie(o.verdict_, o.points_, percentage_);
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
        if (points_) {
            string points = StringUtils::toString(points_.value(), 2);
            while (points.back() == '0') {
                points.pop_back();
            }
            if (points.back() == '.') {
                points.pop_back();
            }
            return points;
        }
        if (percentage_) {
            string points = StringUtils::toString(percentage_.value(), 2);
            while (points.back() == '0') {
                points.pop_back();
            }
            if (points.back() == '.') {
                points.pop_back();
            }
            return points + "%";
        }
        return "";
    }
};

}
