#ifndef TCFRAME_GENERATOR_H
#define TCFRAME_GENERATOR_H

#include "exception.h"
#include "logger.h"
#include "os.h"
#include "problem.h"
#include "testcase.h"
#include "unsatisfiability.h"
#include "util.h"

#include <vector>
#include <cstdio>
#include <string>

using std::initializer_list;
using std::string;
using std::vector;

namespace tcframe {

template<typename TProblem>
class BaseGenerator : protected TProblem, protected TestCasesCollector {
private:
    static const string TEST_CASES_DIR_NAME;

    OperatingSystem* os;
    vector<TestGroup*> testData;
    vector<Subtask*> subtasks;

    IOFormat* inputFormat;

    vector<void(BaseGenerator::*)()> testGroupBlocks = {
        &BaseGenerator::TestGroup1,
        &BaseGenerator::TestGroup2,
        &BaseGenerator::TestGroup3,
        &BaseGenerator::TestGroup4,
        &BaseGenerator::TestGroup5
    };

    vector<TestGroup*> getTestData() {
        try {
            TestCases();
            return TestCasesCollector::collectTestData();
        } catch (NotImplementedException e1){
            for (auto testGroupBlock : testGroupBlocks) {
                try {
                    TestCasesCollector::newTestGroup();
                    (this->*testGroupBlock)();
                } catch (NotImplementedException e2) {
                    vector<TestGroup*> localTestData = TestCasesCollector::collectTestData();
                    localTestData.pop_back();
                    return localTestData;
                }
            }
        }
    }

    void generateTestCase(int testGroupId, int testCaseId) {
        string inputFilename = util::constructTestCaseFilename(TProblem::getSlug(), testGroupId, testCaseId);
        logger::logTestCaseHeader(inputFilename);

        TestCase* testCase = getTestCase(testGroupId, testCaseId);
        UnsatisfiabilitiesCollector* unsatisfiabilitiesCollector = new UnsatisfiabilitiesCollector();

        applyTestCase(testCase, unsatisfiabilitiesCollector);
        applyConstraints(testCase, unsatisfiabilitiesCollector);

        if (unsatisfiabilitiesCollector->hasUnsatisfiabilities()) {
            logger::logTestCaseUnsatisfiedResult();
            return;
        }

        ostream* inputFile = os->createFile(TEST_CASES_DIR_NAME, inputFilename);
        inputFormat->printTo(*inputFile);
        delete inputFile;

        logger::logTestCaseSatisfiedResult();
    }

    TestCase* getTestCase(int testGroupId, int testCaseId) {
        if (testGroupId == -1) {
            return testData[1]->getTestCase(testCaseId - 1);
        } else {
            return testData[testGroupId]->getTestCase(testCaseId - 1);
        }
    }

    void applyTestCase(TestCase* testCase, UnsatisfiabilitiesCollector* unsatisfiabilitiesCollector) {
        if (testCase->getType() == TestCaseType::OFFICIAL) {
            static_cast<OfficialTestCase*>(testCase)->apply();
        } else {
            // TODO: sample test cases
        }
    }

    void applyConstraints(TestCase* testCase, UnsatisfiabilitiesCollector* unsatisfiabilitiesCollector) {
        if (unsatisfiabilitiesCollector->hasUnsatisfiabilities()) {
            return;
        }

        // TODO: check constraint satisfiabilities
    }

protected:
    BaseGenerator()
        : os(new Unix()) { }

    BaseGenerator(OperatingSystem* os)
        : os(os) { }

    virtual ~BaseGenerator() { }

    virtual void TestCases() { throw NotImplementedException(); }

    virtual void TestGroup1() { throw NotImplementedException(); }
    virtual void TestGroup2() { throw NotImplementedException(); }
    virtual void TestGroup3() { throw NotImplementedException(); }
    virtual void TestGroup4() { throw NotImplementedException(); }
    virtual void TestGroup5() { throw NotImplementedException(); }

public:
    void generate() {
        subtasks = TProblem::getSubtasks();
        testData = getTestData();
        inputFormat = TProblem::getInputFormat();

        logger::logIntroduction();

        os->createDirectory(TEST_CASES_DIR_NAME);

        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            logger::logTestGroupHeader(testGroupId);

            for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                generateTestCase(testGroupId, testCaseId);
            }
        }
    }
};

template<typename TProblem>
const string BaseGenerator<TProblem>::TEST_CASES_DIR_NAME = "tc";

}

#endif
