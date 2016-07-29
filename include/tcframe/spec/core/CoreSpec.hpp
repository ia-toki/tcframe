#pragma once

#include "ProblemConfig.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/raw_testcase.hpp"

namespace tcframe {

struct CoreSpec {
private:
    ProblemConfig problemConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    RawTestSuite rawTestSuite_;

public:
    CoreSpec(
            const ProblemConfig& problemConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const RawTestSuite& rawTestSuite)
            : problemConfig_(problemConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , rawTestSuite_(rawTestSuite) {}

    const ProblemConfig& problemConfig() const {
        return problemConfig_;
    }

    const IOFormat& ioFormat() const {
        return ioFormat_;
    }

    const ConstraintSuite& constraintSuite() const {
        return constraintSuite_;
    }

    const RawTestSuite& rawTestSuite() const {
        return rawTestSuite_;
    }
};

}
