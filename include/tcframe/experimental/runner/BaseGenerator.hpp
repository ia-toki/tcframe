#pragma once

#include "tcframe/experimental/runner/GeneratorConfig.hpp"
#include "tcframe/experimental/testcase/TestSuite.hpp"

namespace tcframe { namespace experimental {

template<typename TProblem /* extends Problem */>
class BaseGenerator : public TProblem, protected GeneratorConfigBuilder, protected TestSuiteBuilder {
public:
    virtual ~BaseGenerator() {}

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

}}
