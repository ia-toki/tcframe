#pragma once

#include <tuple>

#include "ProblemConfig.hpp"
#include "TestConfig.hpp"

using std::tie;

namespace tcframe {

struct CoreConfig {
private:
    ProblemConfig problemConfig_;
    TestConfig testConfig_;

public:
    CoreConfig() = default;

    CoreConfig(const ProblemConfig& problemConfig, const TestConfig& testConfig)
            : problemConfig_(problemConfig)
            , testConfig_(testConfig)
    {}

    const ProblemConfig& problemConfig() const {
        return problemConfig_;
    }

    const TestConfig& testConfig() const {
        return testConfig_;
    }

    bool operator==(const CoreConfig& o) const {
        return tie(problemConfig_, testConfig_) == tie(o.problemConfig_, o.testConfig_);
    }
};

}
