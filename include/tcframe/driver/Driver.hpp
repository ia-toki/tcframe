#pragma once

#include <ostream>
#include <string>

#include "SlugParser.hpp"
#include "SpecDriver.hpp"
#include "TestCaseDriver.hpp"
#include "tcframe/spec.hpp"

using std::endl;
using std::ostream;
using std::string;

namespace tcframe {

template<typename TProblemSpec>
class Driver {
private:
    string specPath_;
    BaseTestSpec<TProblemSpec>* testSpec_;

public:
    virtual ~Driver() {}

    Driver(
            const string& specPath,
            BaseTestSpec<TProblemSpec>* testSpec)
            : specPath_(specPath)
            , testSpec_(testSpec) {}

    // TODO (fushar): In 2.0, replace this with entry point
    virtual SpecDriver* getSpecDriver(const Spec& spec) {
        auto testCaseDriver = new TestCaseDriver(
                new IOManipulator(spec.ioFormat()),
                new Verifier(spec.constraintSuite()),
                spec.multipleTestCasesConfig());

        return new SpecDriver(testCaseDriver, spec.testSuite());
    }

    // TODO (fushar): In 2.0, replace this with YAML file
    virtual Spec buildSpec() {
        string slug = SlugParser::parse(specPath_);
        return testSpec_->buildSpec(slug);
    }
};

}
