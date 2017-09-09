#pragma once

#include <string>

#include "tcframe/driver.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec/testcase.hpp"

using std::string;

namespace tcframe {

// TODO (fushar): In 2.0, replace all specDriver_ invocations with actual driver executable calls
class SpecClient {
private:
    SpecDriver* specDriver_;
    OperatingSystem* os_;

public:
    virtual ~SpecClient() {}

    SpecClient(SpecDriver* specDriver, OperatingSystem* os)
            : specDriver_(specDriver)
            , os_(os) {}

    virtual TestSuite getTestSuite() {
        return specDriver_->getTestSuite();
    }

    virtual void generateTestCaseInput(const string& testCaseName, const string& filename) {
        ostream* out = os_->openForWriting(filename);
        specDriver_->generateTestCaseInput(testCaseName, out);
        os_->closeOpenedStream(out);
    }

    virtual void generateSampleTestCaseOutput(const string& testCaseName, const string& filename) {
        ostream* out = os_->openForWriting(filename);
        specDriver_->generateSampleTestCaseOutput(testCaseName, out);
        os_->closeOpenedStream(out);
    }

    virtual void validateTestCaseOutput(const string& filename) {
        istream* in = os_->openForReading(filename);
        specDriver_->validateTestCaseOutput(in);
        os_->closeOpenedStream(in);
    }

    virtual void validateMultipleTestCasesInput(int testCaseCount) {
        specDriver_->validateMultipleTestCasesInput(testCaseCount);
    }
};

}
