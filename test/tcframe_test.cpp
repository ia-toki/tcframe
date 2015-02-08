#include "gtest/gtest.h"

#include "tcframe/tcframe.h"
#include "tcframe/util.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::map;
using std::ostringstream;
using std::string;
using std::vector;
using tcframe::BaseGenerator;
using tcframe::BaseProblem;
using tcframe::ExecutionResult;
using tcframe::Failure;
using tcframe::FailureType;
using tcframe::Logger;
using tcframe::OperatingSystem;
using tcframe::SubtaskUnsatisfiedButAssignedFailure;
using tcframe::Util;

class FakeLogger : public Logger {
public:
    void logIntroduction() override { }
    void logTestGroupIntroduction(int) override { }
    void logTestCaseIntroduction(string) override { }
    void logTestCaseOkResult() override { }
    void logTestCaseFailedResult(string, vector<Failure*>) override { }
};

class FakeOperatingSystem : public OperatingSystem {
private:
    map<string, ostringstream*> testCaseInputs;

public:
    void setBaseDirectory(string) override { }

    istream* openForReading(string) override {
        return new istringstream();
    }

    ostream* openForWriting(string name) override {
        ostringstream* testCaseInput = new ostringstream();
        testCaseInputs[name] = testCaseInput;
        return testCaseInput;
    }

    void remove(string) override { }

    ExecutionResult execute(string, string inputName, string) override {
        istringstream input(testCaseInputs[inputName]->str());

        int A, B, K;
        input >> A >> B >> K;

        string output = Util::toString(A + B * K);

        ExecutionResult result;
        result.exitCode = 0;
        result.errorStream = new istringstream();
        result.outputStream = new istringstream(output);

        return result;
    }
};

class ProblemWithSubtasks : public BaseProblem {
public:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        line() % A, B;
        line() % K;
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

    void TestGroup1() {
        assignToSubtasks({1, 2});

        addTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addTestCase([this] { A = 100, B = 2000, K = 0; }, "A = 100, B = 2000, K = 0");
    }

    void TestGroup2() {
        assignToSubtasks({2});

        addTestCase([this] { A = 1, B = 2, K = 1; }, "A = 1, B = 2, K = 1");
        addTestCase([this] { A = 0, B = 0, K = 100; }, "A = 2000000000, B = 2000000000, K = 100");
    }

public:
    GeneratorWithTestGroups() :
        BaseGenerator(new FakeLogger(), new FakeOperatingSystem()) { }
};

TEST(TcframeTest, GenerationWithSubtasksAndTestGroups) {
    GeneratorWithTestGroups gen;
    map<string, vector<Failure*>> failures = gen.generate();

    ASSERT_EQ(0, failures["problem_1_1"].size());

    ASSERT_EQ(1, failures["problem_1_2"].size());
    ASSERT_EQ(FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED, failures["problem_1_2"][0]->getType());

    SubtaskUnsatisfiedButAssignedFailure* failure_1_2_0 = static_cast<SubtaskUnsatisfiedButAssignedFailure*>(failures["problem_1_2"][0]);
    EXPECT_EQ(1, failure_1_2_0->getSubtaskId());
    ASSERT_EQ(1, failure_1_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= B && B <= 1000", failure_1_2_0->getUnsatisfiedConstraints()[0]->getDescription());

    ASSERT_EQ(1, failures["problem_2_1"].size());
    ASSERT_EQ(FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED, failures["problem_2_1"][0]->getType());

    SubtaskSatisfiedButNotAssignedFailure* failure_2_1_0 = static_cast<SubtaskSatisfiedButNotAssignedFailure*>(failures["problem_2_1"][0]);
    EXPECT_EQ(1, failure_2_1_0->getSubtaskId());

    ASSERT_EQ(2, failures["problem_2_2"].size());

    ASSERT_EQ(FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED, failures["problem_2_2"][0]->getType());

    SubtaskUnsatisfiedButAssignedFailure* failure_2_2_0 = static_cast<SubtaskUnsatisfiedButAssignedFailure*>(failures["problem_2_2"][0]);
    EXPECT_EQ(2, failure_2_2_0->getSubtaskId());
    ASSERT_EQ(2, failure_2_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= A && A <= 2000000000", failure_2_2_0->getUnsatisfiedConstraints()[0]->getDescription());
    EXPECT_EQ("1 <= B && B <= 2000000000", failure_2_2_0->getUnsatisfiedConstraints()[1]->getDescription());

    ASSERT_EQ(FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED, failures["problem_2_2"][1]->getType());

    SubtaskSatisfiedButNotAssignedFailure* failure_2_2_1 = static_cast<SubtaskSatisfiedButNotAssignedFailure*>(failures["problem_2_2"][1]);
    EXPECT_EQ(3, failure_2_2_1->getSubtaskId());
}

