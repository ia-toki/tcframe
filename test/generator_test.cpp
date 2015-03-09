#include "gtest/gtest.h"

#include "tcframe/generator.hpp"

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
using tcframe::Logger;
using tcframe::OperatingSystem;
using tcframe::SubtaskFailure;
using tcframe::SubtaskSatisfiedButNotAssigned;
using tcframe::SubtaskUnsatisfiability;
using tcframe::SubtaskUnsatisfiedButAssigned;
using tcframe::TestCaseFailure;
using tcframe::Util;

class FakeLogger : public Logger {
public:
    void logIntroduction() override { }
    void logTestGroupIntroduction(int) override { }
    void logTestCaseIntroduction(string) override { }
    void logTestCaseOkResult() override { }
    void logTestCaseFailedResult(string) override { }
    void logTestCaseFailure(TestCaseFailure*) override { }
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
        addLineSegment("A, B"), A, B;
        addLineSegment("K"), K;
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
    GeneratorWithTestGroups()
            : BaseGenerator(new FakeLogger(), new FakeOperatingSystem()) { }
};

class ProblemWithoutSubtasks : public BaseProblem {
protected:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        addLineSegment("A, B"), A, B;
        addLineSegment("K"), K;
    }

    void Constraints() {
        addConstraint([this] { return 1 <= A && A <= 1000; }, "1 <= A && A <= 1000");
        addConstraint([this] { return 1 <= B && B <= 1000; }, "1 <= B && B <= 1000");
        addConstraint([this] { return 0 <= K && K <= 100; }, "0 <= K <= 100");
    }
};

class GeneratorWithoutTestGroups : public BaseGenerator<ProblemWithoutSubtasks> {
protected:
    void Config() { }

    void SampleTestCases() {
        addSampleTestCase({"1 1", "1"});
        addSampleTestCase({"1 1", "-1"});
    }

    void TestCases() {
        addOfficialTestCase([this] { A = 1, B = 100, K = 7; }, "A = 1, B = 100, K = 7");
        addOfficialTestCase([this] { A = 100, B = 2000, K = 0; }, "A = 100, B = 2000, K = 0");
    }

public:
    GeneratorWithoutTestGroups()
            : BaseGenerator(new FakeLogger(), new FakeOperatingSystem()) { }
};

TEST(GeneratorTest, GenerationWithSubtasksAndTestGroups) {
    GeneratorWithTestGroups gen;
    map<string, TestCaseFailure*> failures = gen.generate();

    SubtaskFailure* failure_sample_1 = dynamic_cast<SubtaskFailure*>(failures["problem_sample_1"]);
    ASSERT_NE(nullptr, failure_sample_1);

    ASSERT_EQ(1, failure_sample_1->getUnsatisfiabilities().size());

    SubtaskUnsatisfiedButAssigned* failure_sample_1_0 = dynamic_cast<SubtaskUnsatisfiedButAssigned*>(failure_sample_1->getUnsatisfiabilities()[0]);
    ASSERT_NE(nullptr, failure_sample_1_0);

    EXPECT_EQ(1, failure_sample_1_0->getSubtaskId());
    ASSERT_EQ(1, failure_sample_1_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= B && B <= 1000", failure_sample_1_0->getUnsatisfiedConstraints()[0]->getDescription());

    EXPECT_EQ(nullptr, failures["problem_1_1"]);

    SubtaskFailure* failure_1_2 = dynamic_cast<SubtaskFailure*>(failures["problem_1_2"]);
    ASSERT_NE(nullptr, failure_1_2);

    ASSERT_EQ(1, failure_1_2->getUnsatisfiabilities().size());

    SubtaskUnsatisfiedButAssigned* failure_1_2_0 = dynamic_cast<SubtaskUnsatisfiedButAssigned*>(failure_1_2->getUnsatisfiabilities()[0]);
    ASSERT_NE(nullptr, failure_1_2_0);

    EXPECT_EQ(1, failure_1_2_0->getSubtaskId());
    ASSERT_EQ(1, failure_1_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= B && B <= 1000", failure_1_2_0->getUnsatisfiedConstraints()[0]->getDescription());

    SubtaskFailure* failure_2_1 = dynamic_cast<SubtaskFailure*>(failures["problem_2_1"]);
    ASSERT_NE(nullptr, failure_2_1);

    ASSERT_EQ(1, failure_2_1->getUnsatisfiabilities().size());

    SubtaskSatisfiedButNotAssigned* failure_2_1_0 = dynamic_cast<SubtaskSatisfiedButNotAssigned*>(failure_2_1->getUnsatisfiabilities()[0]);
    ASSERT_NE(nullptr, failure_2_1_0);

    EXPECT_EQ(1, failure_2_1_0->getSubtaskId());

    SubtaskFailure* failure_2_2 = dynamic_cast<SubtaskFailure*>(failures["problem_2_2"]);
    ASSERT_NE(nullptr, failure_2_2);

    ASSERT_EQ(2, failure_2_2->getUnsatisfiabilities().size());

    SubtaskUnsatisfiedButAssigned* failure_2_2_0 = dynamic_cast<SubtaskUnsatisfiedButAssigned*>(failure_2_2->getUnsatisfiabilities()[0]);
    ASSERT_NE(failure_2_2_0, nullptr);

    EXPECT_EQ(2, failure_2_2_0->getSubtaskId());
    ASSERT_EQ(2, failure_2_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= A && A <= 2000000000", failure_2_2_0->getUnsatisfiedConstraints()[0]->getDescription());
    EXPECT_EQ("1 <= B && B <= 2000000000", failure_2_2_0->getUnsatisfiedConstraints()[1]->getDescription());

    SubtaskSatisfiedButNotAssigned* failure_2_2_1 = dynamic_cast<SubtaskSatisfiedButNotAssigned*>(failure_2_2->getUnsatisfiabilities()[1]);
    ASSERT_NE(nullptr, failure_2_2_1);

    EXPECT_EQ(3, failure_2_2_1->getSubtaskId());
}

TEST(GeneratorTest, GenerationWithoutSubtasksAndWithoutTestGroups) {
    GeneratorWithoutTestGroups gen;
    map<string, TestCaseFailure*> failures = gen.generate();

    EXPECT_EQ(nullptr, failures["problem_sample_1"]);

    SubtaskFailure* failure_sample_2 = dynamic_cast<SubtaskFailure*>(failures["problem_sample_2"]);
    ASSERT_NE(nullptr, failure_sample_2);

    ASSERT_EQ(1, failure_sample_2->getUnsatisfiabilities().size());

    SubtaskUnsatisfiedButAssigned* failure_sample_2_0 = dynamic_cast<SubtaskUnsatisfiedButAssigned*>(failure_sample_2->getUnsatisfiabilities()[0]);
    ASSERT_NE(nullptr, failure_sample_2_0);

    EXPECT_EQ(-1, failure_sample_2_0->getSubtaskId());
    ASSERT_EQ(1, failure_sample_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("0 <= K <= 100", failure_sample_2_0->getUnsatisfiedConstraints()[0]->getDescription());

    EXPECT_EQ(nullptr, failures["problem_1"]);

    SubtaskFailure* failure_2 = dynamic_cast<SubtaskFailure*>(failures["problem_2"]);
    ASSERT_NE(nullptr, failure_2);

    ASSERT_EQ(1, failure_2->getUnsatisfiabilities().size());

    SubtaskUnsatisfiedButAssigned* failure_2_0 = dynamic_cast<SubtaskUnsatisfiedButAssigned*>(failure_2->getUnsatisfiabilities()[0]);
    ASSERT_NE(nullptr, failure_2_0);

    EXPECT_EQ(-1, failure_2_0->getSubtaskId());
    ASSERT_EQ(1, failure_2_0->getUnsatisfiedConstraints().size());
    EXPECT_EQ("1 <= B && B <= 1000", failure_2_0->getUnsatisfiedConstraints()[0]->getDescription());
}
