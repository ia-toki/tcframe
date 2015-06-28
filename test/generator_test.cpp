#include "gtest/gtest.h"

#include "tcframe_test_commons.cpp"

class FakeGeneratorLogger : public GeneratorLogger {
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

class MyGenerator : public BaseGenerator<DefaultProblem> {
    void Config() {
        setTestCasesDir("testdata");
        setSolutionCommand("java Solution");
    }
};

TEST(GeneratorTest, DefaultOptions) {
    BaseGenerator<DefaultProblem>* generator = new DefaultGenerator();
    generator->applyGeneratorConfiguration();

    EXPECT_EQ("tc", generator->getTestCasesDir());
    EXPECT_EQ("./solution", generator->getSolutionCommand());
}

TEST(GeneratorTest, MyOptions) {
    BaseGenerator<DefaultProblem>* generator = new MyGenerator();
    generator->applyGeneratorConfiguration();

    EXPECT_EQ("testdata", generator->getTestCasesDir());
    EXPECT_EQ("java Solution", generator->getSolutionCommand());
}

TEST(GeneratorTest, CommandLineOptions) {
    char* argv[] = {(char*) "<runner>", (char*)"--tc-dir=testcases", (char*)"--solution-command=\"python sol.py\""};

    BaseGenerator<DefaultProblem>* generator = new MyGenerator();
    generator->applyGeneratorConfiguration();
    generator->applyGeneratorCommandLineOptions(3, argv);

    EXPECT_EQ("testcases", generator->getTestCasesDir());
    EXPECT_EQ("\"python sol.py\"", generator->getSolutionCommand());
}

TEST(GeneratorTest, GenerationWithSubtasksAndTestGroups) {
    FakeGeneratorLogger logger;
    GeneratorWithTestGroups gen(&logger, new FakeOperatingSystem());
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
    FakeGeneratorLogger logger;
    GeneratorWithoutTestGroups gen(&logger, new FakeOperatingSystem());
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
    FakeGeneratorLogger logger;
    GeneratorWithoutTestGroups gen(&logger, new FakeOperatingSystem({ {"problem_1-generation-evaluation", ExecutionResult{1, new istringstream(), new istringstream("Intentionally failed")} } }));
    int exitCode = gen.generate();

    EXPECT_NE(0, exitCode);

    auto failures = logger.getFailures("problem_1");
    ASSERT_EQ(3, failures.size());
    EXPECT_EQ(Failure("Execution of solution failed:", 0), failures[0]);
    EXPECT_EQ(Failure("Exit code: 1", 1), failures[1]);
    EXPECT_EQ(Failure("Standard error: Intentionally failed", 1), failures[2]);
}
