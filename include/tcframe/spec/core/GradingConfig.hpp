#pragma once

#include <tuple>
#include <utility>

using std::move;
using std::tie;

namespace tcframe {

struct GradingConfig {
    friend class GradingConfigBuilder;

private:
    int timeLimit_;
    int memoryLimit_;

public:
    int timeLimit() const {
        return timeLimit_;
    }

    int memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const GradingConfig& o) const {
        return tie(timeLimit_, memoryLimit_) == tie(o.timeLimit_, o.memoryLimit_);
    }
};

class GradingConfigBuilder {
private:
    GradingConfig subject_;

public:
    GradingConfigBuilder() {
        subject_.timeLimit_ = 2;
        subject_.memoryLimit_ = 64;
    }

    GradingConfigBuilder& TimeLimit(int timeLimitInSeconds) {
        subject_.timeLimit_ = timeLimitInSeconds;
        return *this;
    }

    GradingConfigBuilder& MemoryLimit(int memoryLimitInMegabytes) {
        subject_.memoryLimit_ = memoryLimitInMegabytes;
        return *this;
    }

    GradingConfig build() {
        return move(subject_);
    }
};

}
