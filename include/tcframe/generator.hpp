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
#include <cstring>
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
class BaseGenerator : public TProblem, protected TestCasesCollector {
public:
    void applyProblemConfiguration() {
        // This method is a hack because Config() method is ambiguous
        // between BaseProblem and BaseGenerator.
        TProblem::Config();
    }

    void applyGeneratorConfiguration() {
        Config();
    }

    int applyGeneratorCommandLineOptions(int argc, char* argv[]) {
        option longopts[3] = {
                { "tc-dir", required_argument, nullptr, 'd'},
                { "solution-command", required_argument, nullptr, 'c'},
                { 0, 0, 0, 0 }
        };

        optind = 1;

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 'd':
                    testCasesDir = string(optarg);
                    break;
                case 'c':
                    solutionCommand = string(optarg);
                    break;
                default:
                    break;
            }
        }

        return 0;
    }

    int generate() {
        logger->logIntroduction();

        os->forceMakeDir(testCasesDir);

        subtasks = TProblem::getSubtasks();
        testData = getTestData();

        bool successful = true;
        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            logger->logTestGroupIntroduction(testGroupId);

            for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                if (!generateTestCase(testGroupId, testCaseId)) {
                    successful = false;
                }
            }
        }

        return successful ? 0 : 1;
    }

    string getSolutionCommand() {
        return solutionCommand;
    }

    string getTestCasesDir() {
        return testCasesDir;
    }

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

protected:
    BaseGenerator()
            : logger(new DefaultGeneratorLogger()),
              os(new UnixOperatingSystem()),
              solutionCommand("./solution"),
              testCasesDir("tc") { }

    BaseGenerator(GeneratorLogger* logger, OperatingSystem* os)
            : logger(logger),
              os(os),
              solutionCommand("./solution"),
              testCasesDir("tc") { }

    virtual ~BaseGenerator() { }

    virtual void Config() { }

    virtual void FinalizeInput() { }

    virtual void SampleTestCases() { }
    virtual void TestCases() { throw NotImplementedException(); }

    virtual void TestGroup1() { throw NotImplementedException(); }
    virtual void TestGroup2() { throw NotImplementedException(); }
    virtual void TestGroup3() { throw NotImplementedException(); }
    virtual void TestGroup4() { throw NotImplementedException(); }
    virtual void TestGroup5() { throw NotImplementedException(); }
    virtual void TestGroup6() { throw NotImplementedException(); }
    virtual void TestGroup7() { throw NotImplementedException(); }
    virtual void TestGroup8() { throw NotImplementedException(); }
    virtual void TestGroup9() { throw NotImplementedException(); }
    virtual void TestGroup10() { throw NotImplementedException(); }

    void setTestCasesDir(string testCasesDir) {
        this->testCasesDir = testCasesDir;
    }

    void setSolutionCommand(string solutionCommand) {
        this->solutionCommand = solutionCommand;
    }

private:
    GeneratorLogger* logger;
    OperatingSystem* os;

    string solutionCommand;
    string testCasesDir;

    vector<TestGroup*> testData;
    vector<Subtask*> subtasks;

    vector<void(BaseGenerator::*)()> testGroupBlocks = {
            &BaseGenerator::TestGroup1,
            &BaseGenerator::TestGroup2,
            &BaseGenerator::TestGroup3,
            &BaseGenerator::TestGroup4,
            &BaseGenerator::TestGroup5,
            &BaseGenerator::TestGroup6,
            &BaseGenerator::TestGroup7,
            &BaseGenerator::TestGroup8,
            &BaseGenerator::TestGroup9,
            &BaseGenerator::TestGroup10
    };

    bool generateTestCase(int testGroupId, int testCaseId) {
        string testCaseName = Util::constructTestCaseName(TProblem::getSlug(), testGroupId, testCaseId);
        logger->logTestCaseIntroduction(testCaseName);

        string testCaseInputFilename = testCasesDir + "/" + testCaseName + ".in";
        string testCaseOutputFilename = testCasesDir + "/" + testCaseName + ".out";

        TestCase* testCase = getTestCase(testGroupId, testCaseId);

        try {
            applyTestCase(testCase);
            checkConstraints(testCase);
            FinalizeInput();
            generateTestCaseInput(testCaseInputFilename);
            generateTestCaseOutput(testCaseName, testCaseInputFilename, testCaseOutputFilename);

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
        TProblem::beginParsingFormat(&sin);
        TProblem::InputFormat();
        TProblem::endParsingFormat();
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

    void generateTestCaseInput(string testCaseInputFilename) {
        ostream* testCaseInput = os->openForWriting(testCaseInputFilename);

        TProblem::beginPrintingFormat(testCaseInput);
        TProblem::InputFormat();
        TProblem::endPrintingFormat();

        os->closeOpenedWritingStream(testCaseInput);
    }

    void generateTestCaseOutput(string testCaseName, string testCaseInputFilename, string testCaseOutputFilename) {
        ExecutionResult result = os->execute(testCaseName + "-generation-evaluation", solutionCommand, testCaseInputFilename, testCaseOutputFilename, "_error.out");

        if (result.exitStatus != 0) {
            vector<Failure> failures;
            failures.push_back(Failure("Execution of solution failed:", 0));

            if (result.exitStatus <= 128) {
                failures.push_back(Failure("Exit code: " + Util::toString(result.exitStatus), 1));
                failures.push_back(Failure("Standard error: " + string(istreambuf_iterator<char>(*result.errorStream), istreambuf_iterator<char>()), 1));
            } else {
                failures.push_back(Failure(string(strsignal(result.exitStatus - 128)), 1));
            }

            throw ExecutionException(failures);
        }

        TProblem::beginParsingFormat(result.outputStream);
        TProblem::OutputFormat();
        TProblem::endParsingFormat();
    }
};

}

#endif
