#pragma once

#include <algorithm>
#include <sstream>
#include <string>

#include "tcframe/experimental/generation/TestSuiteConfig.hpp"
#include "tcframe/experimental/os/OperatingSystem.hpp"
#include "tcframe/experimental/testcase/TestCase.hpp"
#include "tcframe/experimental/testcase/TestSuite.hpp"
#include "tcframe/experimental/testcase/TestGroup.hpp"
#include "tcframe/experimental/variable/IOVariablePrinter.hpp"

using std::move;
using std::ostringstream;
using std::string;

namespace tcframe { namespace experimental {

class TestSuiteGenerator {
private:
    IOVariablePrinter* ioVariablePrinter_;
    OperatingSystem* os_;
    TestSuiteConfig config_;

public:
    virtual ~TestSuiteGenerator() {}

    TestSuiteGenerator(IOVariablePrinter* ioVariablePrinter, OperatingSystem* os, TestSuiteConfig config)
            : ioVariablePrinter_(ioVariablePrinter)
            , os_(os)
            , config_(move(config))
    {}

    void generate(const TestSuite& testCaseUnit) {
        os_->forceMakeDir(config_.testCasesDir());

        generateTestCases(testCaseUnit.testCases());
    }

private:
    void generateTestCases(const vector<TestGroup>& testCases) {
        for (const TestGroup& testGroup : testCases) {
            generateTestGroup(testGroup);
        }
    }

    void generateTestGroup(const TestGroup& testGroup) {
        for (int testCaseId = 1; testCaseId <= testGroup.testCases().size(); testCaseId++) {
            TestCase testCase = testGroup.testCases()[testCaseId - 1];
            testCase.closure()();
            string testCaseName = getTestCaseName(testGroup.id(), testCaseId);
            generateTestCase(testCaseName);
        }
    }

    void generateTestCase(const string& testCaseName) {
        string testCaseInputFilename = config_.testCasesDir() + "/" + testCaseName + ".in";
        string testCaseOutputFilename = config_.testCasesDir() + "/" + testCaseName + ".out";
        generateTestCaseInput(testCaseInputFilename);
        generateTestCaseOutput(testCaseInputFilename, testCaseOutputFilename);
    }

    void generateTestCaseInput(const string& testCaseInputFilename) {
        ostream* testCaseInput = os_->openForWriting(testCaseInputFilename);
        ioVariablePrinter_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateTestCaseOutput(const string& testCaseInputFilename, const string& testCaseOutputFilename) {
        os_->execute(config_.solutionCommand(), testCaseInputFilename, testCaseOutputFilename, "_error.out");
    }

    string getTestCaseName(int testGroupId, int testCaseId) {
        ostringstream sout;
        sout << config_.slug() << "_";
        if (testGroupId != -1) {
            sout << testGroupId << "_";
        }
        sout << testCaseId;
        return sout.str();
    }
};

}}
