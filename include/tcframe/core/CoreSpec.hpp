#pragma once

#include "CoreConfig.hpp"
#include "tcframe/constraint.hpp"
#include "tcframe/io.hpp"
#include "tcframe/testcase.hpp"

namespace tcframe {

struct CoreSpec {
private:
    CoreConfig coreConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    CoreSpec() = default;

    CoreSpec(
            const CoreConfig& coreConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : coreConfig_(coreConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite)
    {}


    const CoreConfig& coreConfig() const {
        return coreConfig_;
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
