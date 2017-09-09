#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "GradingOptions.hpp"
#include "GraderLogger.hpp"
#include "TestCaseGrader.hpp"
#include "tcframe/os.hpp"
#include "tcframe/runner/aggregator.hpp"
#include "tcframe/runner/client.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::max;
using std::string;
using std::vector;

namespace tcframe {

class Grader {
private:
    SpecClient* specClient_;
    TestCaseGrader* testCaseGrader_;
    Aggregator* aggregator_;
    GraderLogger* logger_;

public:
    virtual ~Grader() {}

    Grader(SpecClient* specClient, TestCaseGrader* testCaseGrader, Aggregator* aggregator, GraderLogger* logger)
            : specClient_(specClient)
            , testCaseGrader_(testCaseGrader)
            , aggregator_(aggregator)
            , logger_(logger) {}

    virtual void grade(const ConstraintSuite& constraintSuite, const GradingOptions& options) {
        logger_->logIntroduction(options.solutionCommand());

        TestSuite testSuite = specClient_->getTestSuite();

        map<int, vector<Verdict>> verdictsBySubtaskId;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            gradeTestGroup(testGroup, options, verdictsBySubtaskId);
        }

        map<int, Subtask> subtasksById = getSubtasksToGrade(constraintSuite);
        map<int, Verdict> subtaskVerdictsById;
        vector<Verdict> subtaskVerdicts;

        for (auto& entry : verdictsBySubtaskId) {
            int subtaskId = entry.first;
            const vector<Verdict>& verdicts = entry.second;

            if (subtasksById.count(subtaskId)) {
                Verdict subtaskVerdict = aggregator_->aggregate(verdicts, subtasksById[subtaskId].points());
                subtaskVerdictsById[subtaskId] = subtaskVerdict;
                subtaskVerdicts.push_back(subtaskVerdict);
            }
        }
        Verdict verdict = aggregate(subtaskVerdicts);

        logger_->logResult(subtaskVerdictsById, verdict);
    }

private:
    map<int, Subtask> getSubtasksToGrade(const ConstraintSuite& constraintSuite) {
        map<int, Subtask> subtaskByIds;
        for (const Subtask& subtask : constraintSuite.constraints()) {
            subtaskByIds[subtask.id()] = subtask;
        }

        // remove global constraints for problem with subtasks
        if (subtaskByIds.size() > 1) {
            subtaskByIds.erase(Subtask::MAIN_ID);
        }

        return subtaskByIds;
    }

    void gradeTestGroup(
            const TestGroup& testGroup,
            const GradingOptions& options,
            map<int, vector<Verdict>>& verdictsBySubtaskId) {

        logger_->logTestGroupIntroduction(testGroup.id());

        if (options.hasMultipleTestCases()) {
            TestCase testCase = TestCaseBuilder()
                    .setName(TestGroup::createName(options.slug(), testGroup.id()))
                    .setSubtaskIds(testGroup.testCases()[0].subtaskIds())
                    .build();
            gradeTestCase(testCase, options, verdictsBySubtaskId);
        } else {
            for (const TestCase& testCase : testGroup.testCases()) {
                gradeTestCase(testCase, options, verdictsBySubtaskId);
            }
        }
    }

    void gradeTestCase(
            const TestCase& testCase,
            const GradingOptions& options,
            map<int, vector<Verdict>>& verdictsBySubtaskId) {

        Verdict verdict = testCaseGrader_->grade(testCase, options);
        for (int subtaskId : testCase.subtaskIds()) {
            verdictsBySubtaskId[subtaskId].push_back(verdict);
        }
    }

    Verdict aggregate(const vector<Verdict>& subtaskVerdicts) {
        VerdictStatus aggregatedStatus = VerdictStatus::ac();
        double aggregatedPoints = 0;
        for (const Verdict& verdict : subtaskVerdicts) {
            aggregatedStatus = max(aggregatedStatus, verdict.status());
            aggregatedPoints += verdict.points().value();
        }
        return Verdict(aggregatedStatus, aggregatedPoints);
    }
};

class GraderFactory {
public:
    virtual Grader* create(
            SpecClient* specClient,
            TestCaseGrader* testCaseGrader,
            Aggregator* aggregator,
            GraderLogger* logger) {
        return new Grader(specClient, testCaseGrader, aggregator, logger);
    }
};

}
