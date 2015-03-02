#ifndef TCFRAME_GENERATOR_H
#define TCFRAME_GENERATOR_H

#include "constraint.hpp"
#include "exception.hpp"
#include "logger.hpp"
#include "os.hpp"
#include "problem.hpp"
#include "testcase.hpp"
#include "util.hpp"

#include <cstdio>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

using std::initializer_list;
using std::map;
using std::ostream;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

template<typename TProblem>
class BaseGenerator : protected TProblem, protected TestCasesCollector {
public:
    map<string, vector<Failure*>> generate() {
        subtasks = TProblem::getSubtasks();
        testData = getTestData();
        inputFormat = TProblem::getInputFormat();

        TProblem::Config();
        Config();

        os->setBaseDirectory(testCasesBaseDirectoryName);

        logger->logIntroduction();

        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            logger->logTestGroupIntroduction(testGroupId);

            for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                generateTestCase(testGroupId, testCaseId);
            }
        }

        return testCaseFailures;
    }

protected:
    BaseGenerator()
            : logger(new StandardLogger()),
              os(new UnixOperatingSystem()),
              solutionExecutionCommand("./solution"),
              testCasesBaseDirectoryName("tc") { }

    BaseGenerator(Logger* logger, OperatingSystem* os)
            : logger(logger),
              os(os),
              solutionExecutionCommand("./solution"),
              testCasesBaseDirectoryName("tc") { }

    virtual ~BaseGenerator() { }

    virtual void Config() = 0;

    void setBaseDir(string testCasesBaseDirectoryName) {
        this->testCasesBaseDirectoryName = testCasesBaseDirectoryName;
    }

    void setSolution(string solutionExecutionCommand) {
        this->solutionExecutionCommand = solutionExecutionCommand;
    }

    virtual void TestCases() { throw NotImplementedException(); }

    virtual void TestGroup1() { throw NotImplementedException(); }
    virtual void TestGroup2() { throw NotImplementedException(); }
    virtual void TestGroup3() { throw NotImplementedException(); }
    virtual void TestGroup4() { throw NotImplementedException(); }
    virtual void TestGroup5() { throw NotImplementedException(); }

private:
    Logger* logger;
    OperatingSystem* os;

    string solutionExecutionCommand;
    string testCasesBaseDirectoryName;

    vector<TestGroup*> testData;
    vector<Subtask*> subtasks;

    IOFormat* inputFormat;

    map<string, vector<Failure*>> testCaseFailures;

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
        } catch (NotImplementedException& e1){
            for (auto testGroupBlock : testGroupBlocks) {
                try {
                    TestCasesCollector::newTestGroup();
                    (this->*testGroupBlock)();
                } catch (NotImplementedException& e2) {
                    vector<TestGroup*> localTestData = TestCasesCollector::collectTestData();
                    localTestData.pop_back();
                    return localTestData;
                }
            }

            return TestCasesCollector::collectTestData();
        }
    }

    void generateTestCase(int testGroupId, int testCaseId) {
        string testCaseName = Util::constructTestCaseName(TProblem::getSlug(), testGroupId, testCaseId);
        logger->logTestCaseIntroduction(testCaseName);

        string testCaseInputName = testCaseName + ".in";
        string testCaseOutputName = testCaseName + ".out";

        TestCase* testCase = getTestCase(testGroupId, testCaseId);

        try {
            applyTestCase(testCase);
            checkConstraints(testCase);
            generateTestCaseInput(testCaseInputName);
            generateTestCaseOutput(testCaseInputName, testCaseOutputName);

            logger->logTestCaseOkResult();
            testCaseFailures[testCaseName] = vector<Failure*>();
        } catch (TestCaseException& e) {
            logger->logTestCaseFailedResult(testCase->getDescription(), e.getFailures());
            testCaseFailures[testCaseName] = e.getFailures();
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
        vector<Failure*> failures;

        set<int> subtaskIds = testCase->getSubtaskIds();

        for (Subtask* subtask : subtasks) {
            vector<Constraint*> unsatisfiedConstraints;
            for (Constraint* constraint : subtask->getConstraints()) {
                if (!constraint->isSatisfied()) {
                    unsatisfiedConstraints.push_back(constraint);
                }
            }

            if (subtaskIds.count(subtask->getId())) {
                if (!unsatisfiedConstraints.empty()) {
                    failures.push_back(new SubtaskUnsatisfiedButAssignedFailure(subtask->getId(), unsatisfiedConstraints));
                }
            } else {
                if (unsatisfiedConstraints.empty()) {
                    failures.push_back(new SubtaskSatisfiedButNotAssignedFailure(subtask->getId()));
                }
            }
        }

        if (!failures.empty()) {
            throw TestCaseException(failures);
        }
    }

    void generateTestCaseInput(string testCaseInputName) {
        ostream* testCaseInput = os->openForWriting(testCaseInputName);
        inputFormat->printTo(*testCaseInput);
        delete testCaseInput;
    }

    void generateTestCaseOutput(string testCaseInputName, string testCaseOutputName) {
        os->execute(solutionExecutionCommand, testCaseInputName, testCaseOutputName);
    }
};

}

#endif
