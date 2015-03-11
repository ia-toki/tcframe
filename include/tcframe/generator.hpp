#ifndef TCFRAME_GENERATOR_H
#define TCFRAME_GENERATOR_H

#include "constraint.hpp"
#include "exception.hpp"
#include "failure.hpp"
#include "logger.hpp"
#include "os.hpp"
#include "problem.hpp"
#include "testcase.hpp"
#include "util.hpp"

#include <cstdio>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using std::initializer_list;
using std::istreambuf_iterator;
using std::istringstream;
using std::ostream;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

template<typename TProblem>
class BaseGenerator : protected TProblem, protected TestCasesCollector {
public:
    int generate() {
        logger->logIntroduction();

        try {
            inputFormat = TProblem::getInputFormat();
        } catch (IOFormatException& e) {
            logger->logInputFormatFailedResult();
            logger->logFailures(e.getFailures());
            return 1;
        }

        subtasks = TProblem::getSubtasks();
        testData = getTestData();

        TProblem::Config();
        Config();

        os->setBaseDirectory(testCasesBaseDirectoryName);

        bool succesful = true;
        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            logger->logTestGroupIntroduction(testGroupId);

            for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                if (!generateTestCase(testGroupId, testCaseId)) {
                    succesful = false;
                }
            }
        }

        return succesful ? 0 : 1;
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

    virtual void Config() { }

    void setBaseDir(string testCasesBaseDirectoryName) {
        this->testCasesBaseDirectoryName = testCasesBaseDirectoryName;
    }

    void setSolution(string solutionExecutionCommand) {
        this->solutionExecutionCommand = solutionExecutionCommand;
    }

    virtual void FinalizeInput() { }

    virtual void SampleTestCases() { }
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

    vector<void(BaseGenerator::*)()> testGroupBlocks = {
            &BaseGenerator::TestGroup1,
            &BaseGenerator::TestGroup2,
            &BaseGenerator::TestGroup3,
            &BaseGenerator::TestGroup4,
            &BaseGenerator::TestGroup5
    };

    vector<TestGroup*> getTestData() {
        SampleTestCases();

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

    bool generateTestCase(int testGroupId, int testCaseId) {
        string testCaseName = Util::constructTestCaseName(TProblem::getSlug(), testGroupId, testCaseId);
        logger->logTestCaseIntroduction(testCaseName);

        string testCaseInputName = testCaseName + ".in";
        string testCaseOutputName = testCaseName + ".out";

        TestCase* testCase = getTestCase(testGroupId, testCaseId);

        try {
            applyTestCase(testCase);
            checkConstraints(testCase);
            FinalizeInput();
            generateTestCaseInput(testCaseInputName);
            generateTestCaseOutput(testCaseInputName, testCaseOutputName);

            logger->logTestCaseOkResult();
        } catch (TestCaseException& e) {
            logger->logTestCaseFailedResult(testCase->getDescription());
            logger->logFailures(e.getFailures());

            return false;
        }

        return true;
    }

    TestCase* getTestCase(int testGroupId, int testCaseId) {
        if (testGroupId == -1) {
            return testData[1]->getTestCase(testCaseId - 1);
        } else {
            return testData[testGroupId]->getTestCase(testCaseId - 1);
        }
    }

    void applyTestCase(TestCase* testCase) {
        if (dynamic_cast<OfficialTestCase*>(testCase) != nullptr) {
            applyOfficialTestCase(dynamic_cast<OfficialTestCase*>(testCase));
        } else {
            applySampleTestCase(dynamic_cast<SampleTestCase*>(testCase));
        }
    }

    void applyOfficialTestCase(OfficialTestCase* testCase) {
        testCase->apply();
    }

    void applySampleTestCase(SampleTestCase* testCase) {
        istringstream sin(testCase->getContent());
        inputFormat->parseFrom(sin);
    }

    void checkConstraints(TestCase* testCase) {
        vector<Failure> failures;

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
                    if (subtask->getId() == -1) {
                        failures.push_back(Failure("Does not satisfy constraints, on:", 0));
                    } else {
                        failures.push_back(Failure("Does not satisfy subtask " + Util::toString(subtask->getId()) + ", on constraints:", 0));
                    }

                    for (Constraint* constraint : unsatisfiedConstraints) {
                        failures.push_back(Failure(constraint->getDescription(), 1));
                    }
                }
            } else {
                if (unsatisfiedConstraints.empty()) {
                    failures.push_back(Failure("Satisfies subtask " + Util::toString(subtask->getId()) + " but is not assigned to it", 0));
                }
            }
        }

        if (!failures.empty()) {
            throw SubtaskSatisfiabilityException(failures);
        }
    }

    void generateTestCaseInput(string testCaseInputName) {
        ostream* testCaseInput = os->openForWriting(testCaseInputName);
        inputFormat->printTo(*testCaseInput);
        delete testCaseInput;
    }

    void generateTestCaseOutput(string testCaseInputName, string testCaseOutputName) {
        ExecutionResult result = os->execute(solutionExecutionCommand, testCaseInputName, testCaseOutputName);

        if (result.exitCode != 0) {
            throw ExecutionException({
                    Failure("Execution of solution failed:", 0),
                    Failure("Exit code: " + Util::toString(result.exitCode), 1),
                    Failure("Standard error: " + string(istreambuf_iterator<char>(*result.errorStream), istreambuf_iterator<char>()), 1)
            });
        }
    }
};

}

#endif
