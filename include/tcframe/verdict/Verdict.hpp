#pragma once

#include <tuple>

#include "VerdictStatus.hpp"

using std::tie;

namespace tcframe {

struct Verdict {
private:
    VerdictStatus status_;

public:
    Verdict()
            : Verdict(VerdictStatus::ac()) {}

    Verdict(VerdictStatus status)
            : status_(status) {}

    const VerdictStatus& status() const {
        return status_;
    }

    bool operator==(const Verdict& o) const {
        return tie(status_) == tie(o.status_);
    }

    bool operator<(const Verdict& o) const {
        return status_ < o.status_;
    }
};

}
