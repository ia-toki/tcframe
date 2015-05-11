#ifndef TCFRAME_SUBMITTER_H
#define TCFRAME_SUBMITTER_H

#include "generator.hpp"
#include "logger.hpp"
#include "os.hpp"
#include "problem.hpp"
#include "util.hpp"
#include "verdict.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::max;
using std::set;
using std::string;
using std::vector;
using tcframe::BaseGenerator;
using tcframe::SubmitterLogger;
using tcframe::OperatingSystem;
using tcframe::DefaultSubmitterLogger;
using tcframe::UnixOperatingSystem;
using tcframe::Util;
using tcframe::Verdict;

namespace tcframe {

template<typename TProblem>
class Submitter {
public:
    Submitter(BaseGenerator<TProblem>* generator)
        : logger(new DefaultSubmitterLogger()),
          os(new UnixOperatingSystem()),
          generator(generator) { }

    int submit(string submissionCommand) {
        if (!isPorcelain) {
            logger->logIntroduction();
        }

        map<int, Verdict> subtaskVerdicts;
        for (Subtask* subtask : generator->getSubtasks()) {
            subtaskVerdicts[subtask->getId()] = Verdict::accepted();
        }

        for (TestGroup* testGroup : generator->getTestData()) {
            int testGroupId = testGroup->getId();

            if (!isPorcelain) {
                logger->logTestGroupIntroduction(testGroupId);
            }

            for (int i = 0; i < testGroup->getTestCasesCount(); i++) {
                TestCase* testCase = testGroup->getTestCase(i);
                Verdict verdict = submitOnTestCase(submissionCommand, testGroupId, i + 1);
                for (int subtaskId : testCase->getSubtaskIds()) {
                    subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], verdict);
                }
            }
        }

        if (!isPorcelain) {
            logger->logSubmissionResult(subtaskVerdicts);
        } else {
            logger->logPorcelainSubmissionResult(subtaskVerdicts);
        }

        return 0;
    }

    void setPorcelain(bool isPorcelain) {
        this->isPorcelain = isPorcelain;
    }

private:
    SubmitterLogger* logger;
    OperatingSystem* os;
    BaseGenerator<TProblem>* generator;

    bool isPorcelain;

    Verdict submitOnTestCase(string submissionCommand, int testGroupId, int testCaseId) {
        string testCaseName = Util::constructTestCaseName(generator->getSlug(), testGroupId, testCaseId);

        if (!isPorcelain) {
            logger->logTestCaseIntroduction(testCaseName);
        }

        string testCaseInputFilename = generator->getTestCasesDir() + "/" + testCaseName + ".in";
        string testCaseOutputFilename = generator->getTestCasesDir() + "/" + testCaseName + ".out";

        Verdict verdict = gradeOnTestCase(submissionCommand, testCaseInputFilename, testCaseOutputFilename);
        os->removeFile("_submission.out");
        os->removeFile("_diff.out");

        if (!isPorcelain) {
            logger->logTestCaseVerdict(verdict);
        }

        if (!verdict.isAccepted()) {
            if (!isPorcelain) {
                logger->logFailures(verdict.getFailures());
            }
        }

        return verdict;
    }

    Verdict gradeOnTestCase(string submissionCommand, string testCaseInputFilename, string testCaseOutputFilename) {
        try {
            executeOnTestCase(submissionCommand, testCaseInputFilename);
            return scoreOnTestCase(testCaseOutputFilename);
        } catch (ExecutionException& e) {
            return Verdict::runtimeError(e.getFailures());
        }
    }

    void executeOnTestCase(string submissionCommand, string testCaseInputFilename) {
        ExecutionResult result = os->execute(submissionCommand, testCaseInputFilename, "_submission.out", "_error.out");

        if (result.exitStatus != 0) {
            vector<Failure> failures;
            failures.push_back(Failure("Execution of submission failed:", 0));

            if (result.exitStatus & (1<<7)) {
                failures.push_back(Failure(string(strsignal(WTERMSIG(result.exitStatus))), 1));
            } else {
                failures.push_back(Failure("Exit code: " + Util::toString(result.exitStatus), 1));
                failures.push_back(Failure("Standard error: " + string(istreambuf_iterator<char>(*result.errorStream), istreambuf_iterator<char>()), 1));
            }

            throw ExecutionException(failures);
        }
    }

    Verdict scoreOnTestCase(string testCaseOutputFilename) {
        string diffCommand = "diff --unchanged-line-format=' %.2dn    %L' --old-line-format='(expected) [line %.2dn]    %L' --new-line-format='(received) [line %.2dn]    %L' " + testCaseOutputFilename + " _submission.out | head -n 10";
        ExecutionResult result = os->execute(diffCommand, "", "_diff.out", "");

        string briefDiffCommand = "diff --brief _submission.out " + testCaseOutputFilename;
        ExecutionResult briefResult = os->execute(briefDiffCommand, "", "", "");

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

#endif
