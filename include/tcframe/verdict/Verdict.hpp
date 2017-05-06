#pragma once

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

    Verdict(double points)
            : status_(VerdictStatus::ac())
            , points_(optional<double>(points)) {}

    const VerdictStatus& status() const {
        return status_;
    }

    bool operator==(const Verdict& o) const {
        return tie(status_, points_) == tie(o.status_, o.points_);
    }

    bool operator<(const Verdict& o) const {
        return status_ < o.status_;
    }
};

}
