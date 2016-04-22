#pragma once

#include "tcframe/config.hpp"
#include "tcframe/testcase.hpp"

namespace tcframe {

template<typename TProblem /* extends Problem */>
class BaseGenerator : public TProblem, protected GeneratorConfigBuilder, protected TestSuiteBuilder {
public:
    virtual ~BaseGenerator() {}

    void applyProblemConfig() {
        TProblem::Config();
    }

    TestSuite buildTestSuite() {
        TestCases();
        return this->TestSuiteBuilder::build();
    }

    GeneratorConfig buildGeneratorConfig() {
        Config();
        return this->GeneratorConfigBuilder::build();
    }

protected:
    virtual void Config() {}
    virtual void TestCases() {}
};

}
