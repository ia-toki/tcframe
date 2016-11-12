#pragma once

#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/testcase.hpp"

namespace tcframe {

struct Spec {
private:
    SeedSetter* seedSetter_;
    MultipleTestCasesConfig multipleTestCasesConfig_;
    GradingConfig gradingConfig_;
    IOFormat ioFormat_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    Spec(
            SeedSetter* seedSetter,
            const MultipleTestCasesConfig& multipleTestCasesConfig,
            const GradingConfig& gradingConfig,
            const IOFormat& ioFormat,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : seedSetter_(seedSetter)
            , multipleTestCasesConfig_(multipleTestCasesConfig)
            , gradingConfig_(gradingConfig)
            , ioFormat_(ioFormat)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite) {}

    SeedSetter* seedSetter() const {
        return seedSetter_;
    }

    const MultipleTestCasesConfig& multipleTestCasesConfig() const {
        return multipleTestCasesConfig_;
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
