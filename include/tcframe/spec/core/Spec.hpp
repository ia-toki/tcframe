#pragma once

#include "SeedSetter.hpp"
#include "tcframe/spec/config.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/testcase.hpp"

namespace tcframe {

struct Spec {
private:
    SeedSetter* seedSetter_;
    IOFormat ioFormat_;
    StyleConfig styleConfig_;
    GradingConfig gradingConfig_;
    MultipleTestCasesConfig multipleTestCasesConfig_;
    ConstraintSuite constraintSuite_;
    TestSuite testSuite_;

public:
    Spec(
            SeedSetter* seedSetter,
            const IOFormat& ioFormat,
            const StyleConfig& styleConfig,
            const GradingConfig& gradingConfig,
            const MultipleTestCasesConfig& multipleTestCasesConfig,
            const ConstraintSuite& constraintSuite,
            const TestSuite& testSuite)
            : seedSetter_(seedSetter)
            , ioFormat_(ioFormat)
            , styleConfig_(styleConfig)
            , gradingConfig_(gradingConfig)
            , multipleTestCasesConfig_(multipleTestCasesConfig)
            , constraintSuite_(constraintSuite)
            , testSuite_(testSuite) {}

    SeedSetter* seedSetter() const {
        return seedSetter_;
    }

    const IOFormat& ioFormat() const {
        return ioFormat_;
    }

    const StyleConfig& styleConfig() const {
        return styleConfig_;
    }

    const GradingConfig& gradingConfig() const {
        return gradingConfig_;
    }

    const MultipleTestCasesConfig& multipleTestCasesConfig() const {
        return multipleTestCasesConfig_;
    }

    const ConstraintSuite& constraintSuite() const {
        return constraintSuite_;
    }

    const TestSuite& testSuite() const {
        return testSuite_;
    }
};

}
