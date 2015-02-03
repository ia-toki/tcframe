#ifndef TCFRAME_GENERATOR_H
#define TCFRAME_GENERATOR_H

#include "constraint.h"
#include "exception.h"
#include "logger.h"
#include "os.h"
#include "problem.h"
#include "testcase.h"
#include "util.h"

#include <vector>
#include <cstdio>
#include <ostream>
#include <set>
#include <string>

using std::initializer_list;
using std::ostream;
using std::set;
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
        string inputFilename = Util::constructTestCaseFilename(TProblem::getSlug(), testGroupId, testCaseId);
        Logger::logTestCaseIntroduction(inputFilename);

        TestCase* testCase = getTestCase(testGroupId, testCaseId);
        ostream* inputFile = nullptr;

        try {
            applyTestCase(testCase);
            checkConstraints(testCase);

            inputFile = os->createFile(TEST_CASES_DIR_NAME, inputFilename + ".in");
            inputFormat->printTo(*inputFile);

            Logger::logTestCaseOkResult();

        } catch (ConstraintsUnsatisfiedException e1) {
            Logger::logTestCaseFailedResult(testCase->getDescription());
            Logger::logConstraintsUnsatisfiedException(e1);
        }

        if (inputFile != nullptr) {
            delete inputFile;
        }
    }

    TestCase* getTestCase(int testGroupId, int testCaseId) {
        if (testGroupId == -1) {
            return testData[1]->getTestCase(testCaseId - 1);
        } else {
            return testData[testGroupId]->getTestCase(testCaseId - 1);
        }
    }

    void applyTestCase(TestCase* testCase) {
        if (testCase->getType() == TestCaseType::OFFICIAL) {
            static_cast<OfficialTestCase*>(testCase)->apply();
        } else {
            // TODO: sample test cases
        }
    }

    void checkConstraints(TestCase* testCase) {
        set<int> subtaskIds = testCase->getSubtaskIds();
        vector<SubtaskFailure*> subtaskFailures;

        for (Subtask* subtask : subtasks) {
            vector<Constraint*> unsatisfiedConstraints;
            for (Constraint* constraint : subtask->getConstraints()) {
                if (!constraint->isSatisfied()) {
                    unsatisfiedConstraints.push_back(constraint);
                }
            }

            if (subtaskIds.count(subtask->getId())) {
                if (!unsatisfiedConstraints.empty()) {
                    subtaskFailures.push_back(new UnsatisfiedSubtaskFailure(subtask->getId(), unsatisfiedConstraints));
                }
            } else {
                if (unsatisfiedConstraints.empty()) {
                    subtaskFailures.push_back(new SatisfiedSubtaskFailure(subtask->getId()));
                }
            }
        }

        if (!subtaskFailures.empty()) {
            throw ConstraintsUnsatisfiedException(subtaskFailures);
        }
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

        Logger::logIntroduction();

        os->createDirectory(TEST_CASES_DIR_NAME);

        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            Logger::logTestGroupIntroduction(testGroupId);

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
