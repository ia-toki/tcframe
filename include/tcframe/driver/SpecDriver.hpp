#pragma once

#include <map>
#include <iostream>
#include <string>

#include "TestCaseDriver.hpp"
#include "tcframe/spec.hpp"

using std::istream;
using std::map;
using std::ostream;
using std::string;

namespace tcframe {

// TODO (fushar): In 2.0:
// - replace all stream parameters with fields, which are then injected with stdin/stdout/stderr.
// - everything should only write to the stream, and return exit codes
class SpecDriver {
private:
    TestCaseDriver* testCaseDriver_;
    TestSuite testSuite_;

    map<string, TestCase> testCasesByName_;

public:
    virtual ~SpecDriver() {}

    SpecDriver(
            TestCaseDriver* testCaseDriver,
            const TestSuite& testSuite)
            : testCaseDriver_(testCaseDriver)
            , testSuite_(testSuite) {

        for (auto& testGroup : testSuite_.testGroups()) {
            for (auto& testCase: testGroup.testCases()) {
                testCasesByName_[testCase.name()] = testCase;
            }
        }
    }

    virtual TestSuite getTestSuite() {
        return testSuite_;
    }

    virtual void generateTestCaseInput(const string& testCaseName, ostream* out) {
        testCaseDriver_->generateInput(testCasesByName_[testCaseName], out);
    }

    virtual void generateSampleTestCaseOutput(const string& testCaseName, ostream* out) {
        testCaseDriver_->generateSampleOutput(testCasesByName_[testCaseName], out);
    }

    virtual void validateTestCaseOutput(istream* in) {
        testCaseDriver_->validateOutput(in);
    }

    virtual void validateMultipleTestCasesInput(int testCaseCount) {
        testCaseDriver_->validateMultipleTestCasesInput(testCaseCount);
    }
};

}
