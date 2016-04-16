#pragma once

#include "tcframe/deprecated/core/Util.hpp"
#include "tcframe/deprecated/core/Verdict.hpp"
#include "tcframe/deprecated/generator/BaseGenerator.hpp"
#include "tcframe/deprecated/generator/TestCase.hpp"
#include "tcframe/deprecated/generator/TestGroup.hpp"
#include "tcframe/deprecated/logger/DefaultSubmitterLogger.hpp"
#include "tcframe/deprecated/logger/SubmitterLogger.hpp"
#include "tcframe/deprecated/os/ExecutionResult.hpp"
#include "tcframe/deprecated/os/OperatingSystem.hpp"
#include "tcframe/deprecated/os/UnixOperatingSystem.hpp"
#include "tcframe/deprecated/problem/Subtask.hpp"

#include <algorithm>
#include <csignal>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::max;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

template<typename TProblem>
class Submitter {
public:
    Submitter(BaseGenerator<TProblem>* generator)
            : logger(new DefaultSubmitterLogger()),
              os(new UnixOperatingSystem()),
              generator(generator),
              submissionCommand(generator->getSolutionCommand()) { }

    Submitter(BaseGenerator<TProblem>* generator, SubmitterLogger* logger, OperatingSystem* os)
            : logger(logger),
              os(os),
              generator(generator),
              submissionCommand(generator->getSolutionCommand()) { }

    string applySubmitterCommandLineOptions(int argc, char** argv) {
        int brief_opt = 0;

        option longopts[2] = {
                { "brief", no_argument, &brief_opt, 1},
                { 0, 0, 0, 0 }
        };

        optind = 1;
        opterr = 0;

        while (getopt_long(argc, argv, "", longopts, nullptr) != -1) {
            ;
        }

        brief = (bool) brief_opt;

        if (optind + 1 == argc) {
            submissionCommand = string(argv[optind]);
        } else if (optind + 1 < argc) {
            return "Usage: <runner> submit [ <submissionCommand> ]";
        }

        return "";
    }

    int submit() {
        if (!brief) {
            logger->logIntroduction();
        }

        map<int, Verdict> subtaskVerdicts;
        for (Subtask* subtask : generator->getSubtasks()) {
            subtaskVerdicts[subtask->getId()] = Verdict::accepted();
        }

        for (TestGroup* testGroup : generator->getTestData()) {
            int testGroupId = testGroup->getId();

            if (!brief) {
                logger->logTestGroupIntroduction(testGroupId);
            }

            if (generator->isMultipleTestCasesPerFile()) {
                string testCaseName = Util::constructTestCaseBaseName(generator->getSlug(), testGroupId);
                Verdict verdict = submitOnTestCase(testCaseName);

                TestCase* testCase = testGroup->getTestCase(0);
                for (int subtaskId : testCase->getSubtaskIds()) {
                    subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], verdict);
                }
            } else {
                for (int testCaseId = 1; testCaseId <= testGroup->getTestCasesCount(); testCaseId++) {
                    TestCase *testCase = testGroup->getTestCase(testCaseId - 1);
                    string testCaseName = Util::constructTestCaseName(generator->getSlug(), testGroup->getId(),
                                                                      testCaseId);
                    Verdict verdict = submitOnTestCase(testCaseName);
                    for (int subtaskId : testCase->getSubtaskIds()) {
                        subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], verdict);
                    }
                }
            }
        }

        if (!brief) {
            logger->logSubmissionResult(subtaskVerdicts);
        } else {
            logger->logBriefSubmissionResult(subtaskVerdicts);
        }

        return 0;
    }

    string getSubmissionCommand() {
        return submissionCommand;
    }

    bool isBrief() {
        return brief;
    }

private:
    SubmitterLogger* logger;
    OperatingSystem* os;
    BaseGenerator<TProblem>* generator;

    bool brief = false;

    string submissionCommand;

    Verdict submitOnTestCase(string testCaseName) {

        if (!brief) {
            logger->logTestCaseIntroduction(testCaseName);
        }

        Verdict verdict = gradeOnTestCase(testCaseName);
        os->removeFile("_submission.out");
        os->removeFile("_diff.out");

        if (!brief) {
            logger->logTestCaseVerdict(verdict);
        }

        if (!verdict.isAccepted()) {
            if (!brief) {
                logger->logFailures(verdict.getFailures());
            }
        }

        return verdict;
    }

    Verdict gradeOnTestCase(string testCaseName) {
        Verdict verdict = executeOnTestCase(testCaseName);
        if (verdict.isUnknown()) {
            return scoreOnTestCase(testCaseName);
        }
        return verdict;
    }

    Verdict executeOnTestCase(string testCaseName) {
        string testCaseInputFilename = generator->getTestCasesDir() + "/" + testCaseName + ".in";

        os->limitExecutionTime(generator->getTimeLimit());
        os->limitExecutionMemory(generator->getMemoryLimit());
        ExecutionResult result = os->execute(testCaseName + "-submission-evaluation", submissionCommand, testCaseInputFilename, "_submission.out", "_error.out");
        os->limitExecutionTime(0);
        os->limitExecutionMemory(0);

        if (result.exitStatus == 0) {
            return Verdict::unknown();
        }

        vector<Failure> failures;

        if (result.exitStatus & (1<<7)) {
            int signal = WTERMSIG(result.exitStatus);

            if (signal == SIGXCPU) {
                return Verdict::timeLimitExceeded();
            }

            failures.push_back(Failure("Execution of submission failed:", 0));
            failures.push_back(Failure(string(strsignal(signal)), 1));
        } else {
            failures.push_back(Failure("Execution of submission failed:", 0));
            failures.push_back(Failure("Exit code: " + Util::toString(result.exitStatus), 1));
            failures.push_back(Failure("Standard error: " + string(istreambuf_iterator<char>(*result.errorStream), istreambuf_iterator<char>()), 1));
        }
        return Verdict::runtimeError(failures);
    }

    Verdict scoreOnTestCase(string testCaseName) {
        string testCaseOutputFilename = generator->getTestCasesDir() + "/" + testCaseName + ".out";

        string diffCommand = "diff --unchanged-line-format=' %.2dn    %L' --old-line-format='(expected) [line %.2dn]    %L' --new-line-format='(received) [line %.2dn]    %L' " + testCaseOutputFilename + " _submission.out | head -n 10";
        ExecutionResult result = os->execute(testCaseName + "-submission-scoring", diffCommand, "", "_diff.out", "");

        string briefDiffCommand = "diff --brief _submission.out " + testCaseOutputFilename;
        ExecutionResult briefResult = os->execute(testCaseName + "-submission-scoring-brief", briefDiffCommand, "", "", "");

        if (briefResult.exitStatus == 0) {
            return Verdict::accepted();
        } else {
            string diff = string(istreambuf_iterator<char>(*result.outputStream), istreambuf_iterator<char>());
            return Verdict::wrongAnswer({
                Failure("Diff:\n" + diff, 0)
            });
        }
    }
};

}
