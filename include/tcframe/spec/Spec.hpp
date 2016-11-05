#pragma once

#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/testcase.hpp"

namespace tcframe {

struct Spec {
private:
    Metadata metadata_;
    ProblemConfig problemConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    Spec(
            const Metadata& metadata,
            const ProblemConfig& problemConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : metadata_(metadata)
            , problemConfig_(problemConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite) {}

    const Metadata& metadata() const {
        return metadata_;
    }

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
