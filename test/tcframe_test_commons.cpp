#include "tcframe/generator.hpp"

using tcframe::BaseGenerator;
using tcframe::BaseProblem;
using tcframe::ExecutionResult;
using tcframe::Failure;
using tcframe::GeneratorLogger;
using tcframe::OperatingSystem;
using tcframe::Util;
using tcframe::Verdict;

class FakeGeneratorLogger : public GeneratorLogger {
public:
    void logTestGroupIntroduction(int) { }

    void logTestCaseIntroduction(string testCaseName) {
        currentKey = testCaseName;
    }

    void logFailures(vector<Failure> failures) {
        this->failuresMap[currentKey] = failures;
    }

    void logIntroduction() { }
    void logTestCaseOkResult() { }
    void logTestCaseFailedResult(string) { }

    vector<Failure> getFailures(string key) {
        return failuresMap[key];
    }

private:
    string currentKey;
    map<string, vector<Failure>> failuresMap;
};

class FakeGeneratorOperatingSystem : public OperatingSystem {
public:
    FakeGeneratorOperatingSystem() { }

    FakeGeneratorOperatingSystem(map<string, ExecutionResult> arrangedResultsMap)
            : arrangedResultsMap(arrangedResultsMap) { }

    istream* openForReading(string) {
        return new istringstream();
    }

    ostream* openForWriting(string filename) {
        ostringstream* testCaseInput = new ostringstream();
        testCaseInputs[filename] = testCaseInput;
        return testCaseInput;
    }

    void closeOpenedWritingStream(ostream*) { }
    void forceMakeDir(string) { }
    void removeFile(string) { }
    void limitExecutionTime(int) { }
    void limitExecutionMemory(int) { }

    ExecutionResult execute(string id, string command, string inputFilename, string outputFilename, string errorFilename) {
        if (arrangedResultsMap.count(id)) {
            return arrangedResultsMap[id];
        }

        istringstream input(testCaseInputs[inputFilename]->str());

        int A, B, K;
        input >> A >> B >> K;

        string output = Util::toString(A + B * K) + "\n";

        ExecutionResult result;
        result.exitStatus = 0;
        result.outputStream = new istringstream(output);
        result.errorStream = new istringstream();

        return result;
    }

private:
    map<string, ostringstream*> testCaseInputs;
    map<string, ExecutionResult> arrangedResultsMap;
};

class DefaultProblem : public BaseProblem {
protected:
    void InputFormat() { }

    void OutputFormat() { }
};

class DefaultGenerator : public BaseGenerator<DefaultProblem> {

};

class ProblemWithSubtasks : public BaseProblem {
public:
    int A, B;
    int K;

    int result;

    void InputFormat() {
        applyLastSegment(), (applyLineSegment("A, B"), A, B);
        applyLastSegment(), (applyLineSegment("K"), K);
    }

    void OutputFormat() {
        applyLastSegment(), (applyLineSegment("result"), result);
    }

    void Subtask1() {
        addConstraint([this] { return 1 <= A && A <= 1000; }, "1 <= A && A <= 1000");
        addConstraint([this] { return 1 <= B && B <= 1000; }, "1 <= B && B <= 1000");
        addConstraint([this] { return 0 <= K && K <= 100; }, "0 <= K <= 100");
    }

    void Subtask2() {
        addConstraint([this] { return 1 <= A && A <= 2000000000; }, "1 <= A && A <= 2000000000");
        addConstraint([this] { return 1 <= B && B <= 2000000000; }, "1 <= B && B <= 2000000000");
        addConstraint([this] { return 0 <= K && K <= 10000; }, "0 <= K <= 10000");
    }

    void Subtask3() {
        addConstraint([this] { return A == 0; }, "A == 0");
        addConstraint([this] { return B == 0; }, "B == 0");
        addConstraint([this] { return 0 <= K && K <= 100; }, "0 <= K <= 100");
    }
};

class GeneratorWithTestGroups : public BaseGenerator<ProblemWithSubtasks> {
protected:
    void SampleTestCases() {
        addSampleTestCase({"1 1000", "1"}, {1, 2});
    }

    void TestGroup1() {
        assignToSubtasks({1, 2});

        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 1000, K = 0; }, "A = 100, B = 1000, K = 0");
    }

    void TestGroup2() {
        assignToSubtasks({2});

        addOfficialTestCase([this] { A = 1001, B = 2, K = 1; }, "A = 1001, B = 2, K = 1");
        addOfficialTestCase([this] { A = 2000, B = 3000, K = 100; }, "A = 2000, B = 3000, K = 100");
    }

    void TestGroup3() {
        assignToSubtasks({3});

        addOfficialTestCase([this] { A = 0, B = 0, K = 42; }, "A = 0, B = 0, K = 42");
    }

public:
    GeneratorWithTestGroups()
            : BaseGenerator(new FakeGeneratorLogger(), new FakeGeneratorOperatingSystem()) { }

    GeneratorWithTestGroups(GeneratorLogger* logger, OperatingSystem* os)
            : BaseGenerator(logger, os) { }
};

class InvalidGeneratorWithTestGroups : public BaseGenerator<ProblemWithSubtasks> {
protected:
    void SampleTestCases() {
        addSampleTestCase({"1 1001", "1"}, {1, 2});
    }

    void TestGroup1() {
        assignToSubtasks({1, 2});

        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 2000, K = 0; }, "A = 100, B = 2000, K = 0");
    }

    void TestGroup2() {
        assignToSubtasks({2});

        addOfficialTestCase([this] { A = 1, B = 2, K = 1; }, "A = 1, B = 2, K = 1");
        addOfficialTestCase([this] { A = 0, B = 0, K = 100; }, "A = 0, B = 0, K = 100");
    }

public:
    InvalidGeneratorWithTestGroups()
            : BaseGenerator(new FakeGeneratorLogger(), new FakeGeneratorOperatingSystem()) { }

    InvalidGeneratorWithTestGroups(GeneratorLogger* logger, OperatingSystem* os)
            : BaseGenerator(logger, os) { }
};

class ProblemWithoutSubtasks : public BaseProblem {
protected:
    int A, B;
    int K;

    int result;

    void InputFormat() {
        applyLastSegment(), (applyLineSegment("A, B"), A, B);
        applyLastSegment(), (applyLineSegment("K"), K);
    }

    void OutputFormat() {
        applyLastSegment(), (applyLineSegment("result"), result);
    }

    void Constraints() {
        addConstraint([this] { return 1 <= A && A <= 1000; }, "1 <= A && A <= 1000");
        addConstraint([this] { return 1 <= B && B <= 1000; }, "1 <= B && B <= 1000");
        addConstraint([this] { return 0 <= K && K <= 100; }, "0 <= K <= 100");
    }
};

class GeneratorWithoutTestGroups : public BaseGenerator<ProblemWithoutSubtasks> {
protected:
    void SampleTestCases() {
        addSampleTestCase({"1 1", "1"});
        addSampleTestCase({"1 1", "0"});
    }

    void TestCases() {
        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 1000, K = 0; }, "A = 100, B = 1000, K = 0");
    }

public:
    GeneratorWithoutTestGroups()
            : BaseGenerator(new FakeGeneratorLogger(), new FakeGeneratorOperatingSystem()) { }

    GeneratorWithoutTestGroups(GeneratorLogger* logger, OperatingSystem* os)
            : BaseGenerator(logger, os) { }
};

class InvalidGeneratorWithoutTestGroups : public BaseGenerator<ProblemWithoutSubtasks> {
protected:
    void SampleTestCases() {
        addSampleTestCase({"1 1", "1"});
        addSampleTestCase({"1 1", "-1"});
    }

    void TestCases() {
        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 2000, K = 0; }, "A = 100, B = 2000, K = 0");
    }

public:
    InvalidGeneratorWithoutTestGroups()
            : BaseGenerator(new FakeGeneratorLogger(), new FakeGeneratorOperatingSystem()) { }

    InvalidGeneratorWithoutTestGroups(GeneratorLogger* logger, OperatingSystem* os)
            : BaseGenerator(logger, os) { }
};
