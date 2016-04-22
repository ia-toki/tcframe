#pragma once

#include "tcframe/config.hpp"
#include "tcframe/constraint.hpp"
#include "tcframe/format.hpp"

namespace tcframe {

class BaseProblem : protected ProblemConfigBuilder, protected IOFormatBuilder, protected ConstraintSuiteBuilder {
public:
    virtual ~BaseProblem() {}

    virtual ProblemConfig buildProblemConfig() {
        applyProblemConfig();
        return ProblemConfigBuilder::build();
    }

    IOFormat buildIOFormat() {
        prepareForInputFormat();
        InputFormat();
        return IOFormatBuilder::build();
    }

    ConstraintSuite buildConstraintSuite() {
        Constraints();
        return ConstraintSuiteBuilder::build();
    }

protected:
    virtual void applyProblemConfig() {
        Config();
    }

    virtual void Config() {};
    virtual void InputFormat() = 0;
    virtual void Constraints() {}
};

}
