#pragma once

#include <map>
#include <iostream>
#include <string>
#include <utility>

#include "TestCaseDriver.hpp"
#include "tcframe/spec.hpp"

using std::istream;
using std::map;
using std::move;
using std::ostream;
using std::string;

namespace tcframe {

// TODO (fushar): In 2.0:
// - replace all stream parameters with fields, which are then injected with stdin/stdout/stderr.
// - everything should only write to the stream, and return exit codes
class SpecDriver {
private:
    TestCaseDriver* testCaseDriver_;
    SeedSetter* seedSetter_;
    MultipleTestCasesConfig multipleTestCasesConfig_;
    TestSuite testSuite_;

    map<string, TestCase> testCasesByName_;

public:
    virtual ~SpecDriver() = default;

    SpecDriver(
            TestCaseDriver* testCaseDriver,
            SeedSetter* seedSetter,
            MultipleTestCasesConfig multipleTestCasesConfig,
            TestSuite testSuite)
            : testCaseDriver_(testCaseDriver)
            , seedSetter_(seedSetter)
            , multipleTestCasesConfig_(move(multipleTestCasesConfig))
            , testSuite_(move(testSuite)) {

        for (auto& testGroup : testSuite_.testGroups()) {
            for (auto& testCase: testGroup.testCases()) {
                testCasesByName_[testCase.name()] = testCase;
            }
        }
    }

    virtual TestSuite getTestSuite() {
        return testSuite_;
    }

    virtual void setSeed(unsigned seed) {
        seedSetter_->setSeed(seed);
    }

    virtual bool hasMultipleTestCases() {
        return multipleTestCasesConfig_.counter();
    }

    virtual optional<string> getMultipleTestCasesOutputPrefix() {
        return multipleTestCasesConfig_.outputPrefix();
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
