#include "gtest/gtest.h"

#include "../commons/Commons.hpp"
#include "tcframe/deprecated/core/Verdict.hpp"
#include "tcframe/deprecated/logger/SubmitterLogger.hpp"
#include "tcframe/deprecated/os/ExecutionResult.hpp"
#include "tcframe/deprecated/os/OperatingSystem.hpp"
#include "tcframe/deprecated/submitter/Submitter.hpp"

#include <map>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::istream;
using std::istringstream;
using std::map;
using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

using tcframe::ExecutionResult;
using tcframe::Submitter;
using tcframe::SubmitterLogger;
using tcframe::Verdict;

class FakeSubmitterLogger : public SubmitterLogger {
public:
    void logTestGroupIntroduction(int) { }

    void logTestCaseIntroduction(string testCaseName) {
        currentKey = testCaseName;
    }

    void logFailures(vector<Failure>) { }
    void logIntroduction() { }

    void logTestCaseVerdict(Verdict verdict) {
        testCaseVerdictsMap[currentKey] = verdict;
    }

    void logSubmissionResult(map<int, Verdict> subtaskVerdicts) {
        subtaskVerdictsMap = subtaskVerdicts;
    }

    void logBriefSubmissionResult(map<int, Verdict> subtaskVerdicts) {
        subtaskVerdictsMap = subtaskVerdicts;
    }

    Verdict getTestCaseVerdict(string testCaseName) {
        return testCaseVerdictsMap[testCaseName];
    }

    Verdict getSubtaskVerdict(int subtaskId) {
        return subtaskVerdictsMap[subtaskId];
    }

private:
    string currentKey;
    map<string, Verdict> testCaseVerdictsMap;
    map<int, Verdict> subtaskVerdictsMap;
};

class FakeSubmitterOperatingSystem : public OperatingSystem {
public:
    FakeSubmitterOperatingSystem() { }

    FakeSubmitterOperatingSystem(map<string, ExecutionResult> arrangedResultsMap)
            : arrangedResultsMap(arrangedResultsMap) { }

    istream* openForReading(string) { }
    ostream* openForWriting(string) { }
    void closeOpenedWritingStream(ostream*) { }
    void forceMakeDir(string) { }
    void removeFile(string) { }
    void limitExecutionTime(int) { }
    void limitExecutionMemory(int) { }

    ExecutionResult execute(string id, string command, string inputFilename, string outputFilename, string errorFilename) {
        if (arrangedResultsMap.count(id)) {
            return arrangedResultsMap[id];
        } else {
            return (ExecutionResult){0, new istringstream(), new istringstream()};
        }
    }

    void combineMultipleTestCases(string testCaseBaseFilename, int testCasesCount) { }

private:
    map<string, ExecutionResult> arrangedResultsMap;
};

TEST(DeprecatedSubmitterTest, DefaultOptions) {
    Submitter<DefaultProblem> submitter(new DefaultGenerator());

    EXPECT_FALSE(submitter.isBrief());
}

TEST(DeprecatedSubmitterTest, CommandLineOptions) {
    char* argv[] = {(char*)"submit", (char*)"--brief"};

    Submitter<DefaultProblem> submitter(new DefaultGenerator());
    submitter.applySubmitterCommandLineOptions(2, argv);

    EXPECT_TRUE(submitter.isBrief());
}

TEST(DeprecatedSubmitterTest, DefaultSubmissionCommand) {
    Submitter<DefaultProblem> submitter(new DefaultGenerator());

    EXPECT_EQ("./solution", submitter.getSubmissionCommand());
}

TEST(DeprecatedSubmitterTest, MySubmissionCommand) {
    char* argv[] = {(char*)"submit", (char*)"./other_solution"};

    Submitter<DefaultProblem> submitter(new DefaultGenerator());
    submitter.applySubmitterCommandLineOptions(2, argv);

    EXPECT_EQ("./other_solution", submitter.getSubmissionCommand());
}

TEST(DeprecatedSubmitterTest, AcceptedSubmissionWithSubtasksAndTestGroups) {
    FakeSubmitterLogger logger;
    Submitter<ProblemWithSubtasks> submitter(new GeneratorWithTestGroups<ProblemWithSubtasks>(), &logger, new FakeSubmitterOperatingSystem());
    int exitCode = submitter.submit();

    EXPECT_EQ(0, exitCode);

    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_sample_1").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_1_1").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_1_2").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_2_1").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_2_2").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_3_1").getCode());

    EXPECT_EQ("AC", logger.getSubtaskVerdict(1).getCode());
    EXPECT_EQ("AC", logger.getSubtaskVerdict(2).getCode());
    EXPECT_EQ("AC", logger.getSubtaskVerdict(3).getCode());
}

TEST(DeprecatedSubmitterTest, FailedSubmissionWithSubtasksAndTestGroups) {
    FakeSubmitterLogger logger;

    map<string, ExecutionResult> arrangedResultsMap = {
            {"problem_1_1-submission-scoring-brief", (ExecutionResult){1, new istringstream(), new istringstream()}},
            {"problem_1_2-submission-evaluation", (ExecutionResult){1, new istringstream(), new istringstream()}},
            {"problem_2_1-submission-evaluation", (ExecutionResult){1, new istringstream(), new istringstream()}},
            {"problem_2_2-submission-evaluation", (ExecutionResult){SIGXCPU | (1<<7), new istringstream(), new istringstream()}},
            {"problem_3_1-submission-scoring-brief", (ExecutionResult){1, new istringstream(), new istringstream()}}
    };

    Submitter<ProblemWithSubtasks> submitter(new GeneratorWithTestGroups<ProblemWithSubtasks>(), &logger, new FakeSubmitterOperatingSystem(arrangedResultsMap));
    int exitCode = submitter.submit();

    EXPECT_EQ(0, exitCode);

    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_sample_1").getCode());
    EXPECT_EQ("WA", logger.getTestCaseVerdict("problem_1_1").getCode());
    EXPECT_EQ("RTE", logger.getTestCaseVerdict("problem_1_2").getCode());
    EXPECT_EQ("RTE", logger.getTestCaseVerdict("problem_2_1").getCode());
    EXPECT_EQ("TLE", logger.getTestCaseVerdict("problem_2_2").getCode());
    EXPECT_EQ("WA", logger.getTestCaseVerdict("problem_3_1").getCode());

    EXPECT_EQ("RTE", logger.getSubtaskVerdict(1).getCode());
    EXPECT_EQ("TLE", logger.getSubtaskVerdict(2).getCode());
    EXPECT_EQ("WA", logger.getSubtaskVerdict(3).getCode());
}

TEST(DeprecatedSubmitterTest, AcceptedSubmissionWithoutSubtasksAndTestGroups) {
    FakeSubmitterLogger logger;
    Submitter<ProblemWithoutSubtasks> submitter(new GeneratorWithoutTestGroups<ProblemWithoutSubtasks>(), &logger, new FakeSubmitterOperatingSystem());
    int exitCode = submitter.submit();

    EXPECT_EQ(0, exitCode);

    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_sample_1").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_sample_2").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_1").getCode());
    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_2").getCode());

    EXPECT_EQ("AC", logger.getSubtaskVerdict(-1).getCode());
}

TEST(DeprecatedSubmitterTest, FailedSubmissionWithoutSubtasksAndTestGroups) {
    FakeSubmitterLogger logger;

    map<string, ExecutionResult> arrangedResultsMap = {
            {"problem_sample_2-submission-scoring-brief", (ExecutionResult){1, new istringstream(), new istringstream()}},
            {"problem_1-submission-evaluation", (ExecutionResult){1, new istringstream(), new istringstream()}},
            {"problem_2-submission-evaluation", (ExecutionResult){SIGXCPU | (1<<7), new istringstream(), new istringstream()}},
    };

    Submitter<ProblemWithoutSubtasks> submitter(new GeneratorWithoutTestGroups<ProblemWithoutSubtasks>(), &logger, new FakeSubmitterOperatingSystem(arrangedResultsMap));
    int exitCode = submitter.submit();

    EXPECT_EQ(0, exitCode);

    EXPECT_EQ("AC", logger.getTestCaseVerdict("problem_sample_1").getCode());
    EXPECT_EQ("WA", logger.getTestCaseVerdict("problem_sample_2").getCode());
    EXPECT_EQ("RTE", logger.getTestCaseVerdict("problem_1").getCode());
    EXPECT_EQ("TLE", logger.getTestCaseVerdict("problem_2").getCode());

    EXPECT_EQ("TLE", logger.getSubtaskVerdict(-1).getCode());
}
