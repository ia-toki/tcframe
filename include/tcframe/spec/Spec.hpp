#pragma once

#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/testcase.hpp"

namespace tcframe {

struct Spec {
private:
    ProblemConfig problemConfig_;
    GradingConfig gradingConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    Spec(
            const ProblemConfig& problemConfig,
            const GradingConfig& gradingConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : problemConfig_(problemConfig)
            , gradingConfig_(gradingConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite) {}

    const ProblemConfig& problemConfig() const {
        return problemConfig_;
    }

    const GradingConfig& gradingConfig() const {
        return gradingConfig_;
    }

    const IOFormat& ioFormat() const {
        return ioFormat_;
    }

    const ConstraintSuite& constraintSuite() const {
        return constraintSuite_;
    }

    const TestSuite& testSuite() const {
        return testSuite_;
    }
};

}
