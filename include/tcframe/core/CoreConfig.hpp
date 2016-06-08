#pragma once

#include "ProblemConfig.hpp"
#include "TestConfig.hpp"

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
};

}
