#pragma once

#include <tuple>

using std::tie;

namespace tcframe {

struct ExecutionInfo {
private:
    int exitStatus_;

public:
    ExecutionInfo(int exitStatus)
            : exitStatus_(exitStatus) {}

    int exitStatus() const {
        return exitStatus_;
    }

    bool operator==(const ExecutionInfo& o) const {
        return tie(exitStatus_) == tie(o.exitStatus_);
    }
};

}
