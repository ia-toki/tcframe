#include "tcframe/generator.hpp"
#include "tcframe/problem.hpp"

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::map;
using std::ostringstream;
using std::set;
using std::string;
using std::vector;
using tcframe::BaseGenerator;
using tcframe::BaseProblem;
using tcframe::ExecutionResult;
using tcframe::Failure;
using tcframe::GeneratorLogger;
using tcframe::OperatingSystem;
using tcframe::Util;
using tcframe::Verdict;

class FakeLogger : public GeneratorLogger {
public:
    void logIntroduction() { }
    void logTestGroupIntroduction(int) { }

    void logTestCaseIntroduction(string testCaseName) {
        currentKey = testCaseName;
    }

    void logTestCaseOkResult() { }
    void logTestCaseFailedResult(string) { }

    void logSubmissionIntroduction() { }
    void logTestCaseVerdictName(string) { }
    void logSubmissionResult(map<int, Verdict>) { }
    void logPorcelainSubmissionResult(map<int, Verdict>) { }

    void logFailures(vector<Failure> failures) {
        this->failuresMap[currentKey] = failures;
    }

    vector<Failure> getFailures(string key) {
        return failuresMap[key];
    }

private:
    string currentKey;
    map<string, vector<Failure>> failuresMap;
};

class FakeOperatingSystem : public OperatingSystem {
public:
    FakeOperatingSystem() { }

    FakeOperatingSystem(set<string> arrangedFailedInputNames)
            : arrangedFailedInputFilenames(arrangedFailedInputNames) { }

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

    ExecutionResult execute(string, string inputFilename, string, string) {
        if (arrangedFailedInputFilenames.count(inputFilename)) {
            return ExecutionResult{1, new istringstream(), new istringstream("Intentionally failed")};
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
    set<string> arrangedFailedInputFilenames;
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
        addOfficialTestCase([this] { A = 0, B = 0, K = 100; }, "A = 2000000000, B = 2000000000, K = 100");
    }

public:
    GeneratorWithTestGroups(FakeLogger* logger)
            : BaseGenerator(logger, new FakeOperatingSystem()) { }
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
        addSampleTestCase({"1 1", "-1"});
    }

    void TestCases() {
        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 2000, K = 0; }, "A = 100, B = 2000, K = 0");
    }

public:
    GeneratorWithoutTestGroups(FakeLogger* logger)
            : BaseGenerator(logger, new FakeOperatingSystem()) { }
};

class GeneratorWithArrangedExecutionFailure : public BaseGenerator<ProblemWithoutSubtasks> {
protected:
    void TestCases() {
        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
    }

public:
    GeneratorWithArrangedExecutionFailure(FakeLogger* logger, set<string> arrangedFailedInputNames)
            : BaseGenerator(logger, new FakeOperatingSystem(arrangedFailedInputNames)) { }
};