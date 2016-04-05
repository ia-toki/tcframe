#pragma once

#include "tcframe/experimental/io/IOFormat.hpp"
#include "tcframe/experimental/runner/ProblemConfig.hpp"

namespace tcframe { namespace experimental {

class BaseProblem : protected ProblemConfigBuilder, protected IOFormatBuilder {
public:
    virtual ~BaseProblem() {}

    ProblemConfig buildProblemConfig() {
        Config();
        return this->ProblemConfigBuilder::build();
    }

    IOFormat buildIOFormat() {
        prepareForInputFormat();
        InputFormat();
        return this->IOFormatBuilder::build();
    }

protected:
    virtual void Config() {}
    virtual void InputFormat() = 0;
};

}}
