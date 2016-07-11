#pragma once

#include "ProblemConfig.hpp"
#include "tcframe/constraint.hpp"
#include "tcframe/io.hpp"
#include "tcframe/testcase.hpp"

namespace tcframe {

struct CoreSpec {
private:
    ProblemConfig problemConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    CoreSpec(
            const ProblemConfig& problemConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : problemConfig_(problemConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite)
    {}

    const ProblemConfig& problemConfig() const {
        return problemConfig_;
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
