#include "gtest/gtest.h"

#include "tcframe/generator.hpp"

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
using tcframe::Logger;
using tcframe::OperatingSystem;
using tcframe::Util;

class FakeLogger : public Logger {
public:
    FakeLogger()
            : currentKey("inputFormat") { }

    void logIntroduction() { }
    void logTestGroupIntroduction(int) { }

    void logTestCaseIntroduction(string testCaseName) {
        currentKey = testCaseName;
    }

    void logTestCaseOkResult() { }
    void logTestCaseFailedResult(string) { }
    void logInputFormatFailedResult() { }

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
            : arrangedFailedInputNames(arrangedFailedInputNames) { }

    void setBaseDirectory(string) { }

    istream* openForReading(string) {
        return new istringstream();
    }

    ostream* openForWriting(string name) {
        ostringstream* testCaseInput = new ostringstream();
        testCaseInputs[name] = testCaseInput;
        return testCaseInput;
    }

    void remove(string) { }

    ExecutionResult execute(string, string inputName, string) {
        if (arrangedFailedInputNames.count(inputName)) {
            return ExecutionResult{1, new istringstream(), new istringstream("Intentionally failed")};
        }

        istringstream input(testCaseInputs[inputName]->str());

        int A, B, K;
        input >> A >> B >> K;

        string output = Util::toString(A + B * K) + "\n";

        ExecutionResult result;
        result.exitCode = 0;
        result.outputStream = new istringstream(output);
        result.errorStream = new istringstream();

        return result;
    }

private:
    map<string, ostringstream*> testCaseInputs;
    set<string> arrangedFailedInputNames;
};

class ProblemWithSubtasks : public BaseProblem {
public:
    int A, B;
    int K;

    int result;

    void Config() {
        setSlug("problem");
    }

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
    void Config() { }

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

    void Config() {
        setSlug("problem");
    }

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

TEST(GeneratorTest, GenerationWithSubtasksAndTestGroups) {
    FakeLogger logger;
    GeneratorWithTestGroups gen(&logger);
    int exitCode = gen.generate();

    EXPECT_NE(0, exitCode);

    auto failures_sample_1 = logger.getFailures("problem_sample_1");
    ASSERT_EQ(2, failures_sample_1.size());
    EXPECT_EQ(Failure("Does not satisfy subtask 1, on constraints:", 0), failures_sample_1[0]);
    EXPECT_EQ(Failure("1 <= B && B <= 1000", 1), failures_sample_1[1]);

    auto failures_1_1 = logger.getFailures("problem_1_1");
    ASSERT_EQ(0, failures_1_1.size());

    auto failures_1_2 = logger.getFailures("problem_1_2");
    ASSERT_EQ(2, failures_1_2.size());
    EXPECT_EQ(Failure("Does not satisfy subtask 1, on constraints:", 0), failures_1_2[0]);
    EXPECT_EQ(Failure("1 <= B && B <= 1000", 1), failures_1_2[1]);

    auto failures_2_1 = logger.getFailures("problem_2_1");
    ASSERT_EQ(1, failures_2_1.size());
    EXPECT_EQ(Failure("Satisfies subtask 1 but is not assigned to it", 0), failures_2_1[0]);

    auto failures_2_2 = logger.getFailures("problem_2_2");
    ASSERT_EQ(4, failures_2_2.size());
    EXPECT_EQ(Failure("Does not satisfy subtask 2, on constraints:", 0), failures_2_2[0]);
    EXPECT_EQ(Failure("1 <= A && A <= 2000000000", 1), failures_2_2[1]);
    EXPECT_EQ(Failure("1 <= B && B <= 2000000000", 1), failures_2_2[2]);
    EXPECT_EQ(Failure("Satisfies subtask 3 but is not assigned to it", 0), failures_2_2[3]);
}

TEST(GeneratorTest, GenerationWithoutSubtasksAndWithoutTestGroups) {
    FakeLogger logger;
    GeneratorWithoutTestGroups gen(&logger);
    int exitCode = gen.generate();

    EXPECT_NE(0, exitCode);

    auto failures_sample_1 = logger.getFailures("problem_sample_1");
    ASSERT_EQ(0, failures_sample_1.size());

    auto failures_sample_2 = logger.getFailures("problem_sample_2");
    ASSERT_EQ(2, failures_sample_2.size());
    EXPECT_EQ(Failure("Does not satisfy constraints, on:", 0), failures_sample_2[0]);
    EXPECT_EQ(Failure("0 <= K <= 100", 1), failures_sample_2[1]);

    auto failures_1 = logger.getFailures("problem_1");
    ASSERT_EQ(0, failures_1.size());

    auto failures_2 = logger.getFailures("problem_2");
    ASSERT_EQ(2, failures_sample_2.size());
    EXPECT_EQ(Failure("Does not satisfy constraints, on:", 0), failures_2[0]);
    EXPECT_EQ(Failure("1 <= B && B <= 1000", 1), failures_2[1]);
}

TEST(GeneratorTest, GenerationWithFailedExecution) {
    FakeLogger logger;
    GeneratorWithArrangedExecutionFailure gen(&logger, {"problem_1.in"});
    int exitCode = gen.generate();

    EXPECT_NE(0, exitCode);

    auto failures = logger.getFailures("problem_1");
    ASSERT_EQ(3, failures.size());
    EXPECT_EQ(Failure("Execution of solution failed:", 0), failures[0]);
    EXPECT_EQ(Failure("Exit code: 1", 1), failures[1]);
    EXPECT_EQ(Failure("Standard error: Intentionally failed", 1), failures[2]);
}
