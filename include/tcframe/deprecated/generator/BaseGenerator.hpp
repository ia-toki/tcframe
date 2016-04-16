#pragma once

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/core/NotImplementedException.hpp"
#include "tcframe/deprecated/core/Util.hpp"
#include "tcframe/deprecated/generator/OfficialTestCase.hpp"
#include "tcframe/deprecated/generator/SampleTestCase.hpp"
#include "tcframe/deprecated/generator/TestCase.hpp"
#include "tcframe/deprecated/generator/TestCasesCollector.hpp"
#include "tcframe/deprecated/generator/TestGroup.hpp"
#include "tcframe/deprecated/logger/GeneratorLogger.hpp"
#include "tcframe/deprecated/logger/DefaultGeneratorLogger.hpp"
#include "tcframe/deprecated/os/ExecutionException.hpp"
#include "tcframe/deprecated/os/ExecutionResult.hpp"
#include "tcframe/deprecated/os/OperatingSystem.hpp"
#include "tcframe/deprecated/os/UnixOperatingSystem.hpp"
#include "tcframe/deprecated/problem/Constraint.hpp"
#include "tcframe/deprecated/problem/MultipleTestCasesConstraintsSatisfiabilityException.hpp"
#include "tcframe/deprecated/problem/Subtask.hpp"
#include "tcframe/deprecated/problem/SubtaskSatisfiabilityException.hpp"
#include "tcframe/deprecated/random/Random.hpp"

#include <cstring>
#include <getopt.h>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

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
        option longopts[4] = {
                { "tc-dir", required_argument, nullptr, 'd'},
                { "solution-command", required_argument, nullptr, 'c'},
                { "seed", required_argument, nullptr, 'e'},
                { 0, 0, 0, 0 }
        };

        optind = 1;
        opterr = 0;

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 'd':
                    testCasesDir = string(optarg);
                    break;
                case 'c':
                    solutionCommand = string(optarg);
                    break;
                case 'e':
                    rnd.setSeed((unsigned int) Util::toInt(optarg));
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
        multipleTestCasesConstraints = TProblem::getMultipleTestCasesConstraints();
        testData = getTestData();

        bool successful = true;
        for (TestGroup* testGroup : testData) {
            int testGroupId = testGroup->getId();
            logger->logTestGroupIntroduction(testGroupId);

            bool testGroupSuccessful = true;

            for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                if (!generateTestCase(testGroupId, testCaseId)) {
                    testGroupSuccessful = false;
                }
            }

            if (!testGroupSuccessful) {
                successful = false;
            }

            if (testGroupSuccessful && TProblem::isMultipleTestCasesPerFile()) {
                string testCaseBaseName = Util::constructTestCaseBaseName(TProblem::getSlug(), testGroupId);
                logger->logMultipleTestCasesCombinationIntroduction(testCaseBaseName);

                *(TProblem::getMultipleTestCasesCountPointer()) = testGroup->getTestCasesCount();

                try {
                    checkMultipleTestCasesConstraints();
                } catch (MultipleTestCasesConstraintsSatisfiabilityException& e) {
                    logger->logMultipleTestCasesCombinationFailedResult();
                    logger->logFailures(e.getFailures());
                    successful = false;
                    continue;
                }

                string testCaseBaseFilename = getTestCasesDir() + "/" + testCaseBaseName;
                os->combineMultipleTestCases(testCaseBaseFilename, testGroup->getTestCasesCount());

                logger->logMultipleTestCasesCombinationOkResult();
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
              os(new UnixOperatingSystem()) { }

    BaseGenerator(GeneratorLogger* logger, OperatingSystem* os)
            : logger(logger),
              os(os) { }

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

    Random rnd;

private:
    GeneratorLogger* logger;
    OperatingSystem* os;

    string solutionCommand = "./solution";
    string testCasesDir = "tc";

    vector<TestGroup*> testData;
    vector<Subtask*> subtasks;
    vector<Constraint*> multipleTestCasesConstraints;

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
            applyOfficialTestCase((OfficialTestCase*)testCase);
        } else {
            applySampleTestCase((SampleTestCase*)testCase);
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

    void checkMultipleTestCasesConstraints() {
        vector<Constraint*> unsatisfiedConstraints;
        for (Constraint* constraint : multipleTestCasesConstraints) {
            if (!constraint->isSatisfied()) {
                unsatisfiedConstraints.push_back(constraint);
            }
        }

        if (!unsatisfiedConstraints.empty()) {
            vector<Failure> failures;
            failures.push_back(Failure("Does not satisfy multiple test cases constraints, on:", 0));
            for (Constraint* constraint : unsatisfiedConstraints) {
                failures.push_back(Failure(constraint->getDescription(), 1));
            }

            throw MultipleTestCasesConstraintsSatisfiabilityException(failures);
        }
    }

    void generateTestCaseInput(string testCaseInputFilename) {
        ostream* testCaseInput = os->openForWriting(testCaseInputFilename);

        if (TProblem::isMultipleTestCasesPerFile()) {
            *testCaseInput << "1\n";
        }

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

        try {
            TProblem::OutputFormat();
            TProblem::endParsingFormat();
        } catch (NotImplementedException& e) {
            // skip
        }
    }
};

}
