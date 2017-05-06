#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "TestCaseGrader.hpp"
#include "tcframe/aggregator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;
using std::max;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

class Grader {
private:
    TestCaseGrader* testCaseGrader_;
    Aggregator* aggregator_;
    Aggregator* finalAggregator_;
    GraderLogger* logger_;

public:
    virtual ~Grader() {}

    Grader(TestCaseGrader* testCaseGrader, Aggregator* aggregator, Aggregator* finalAggregator, GraderLogger* logger)
            : testCaseGrader_(testCaseGrader)
            , aggregator_(aggregator)
            , finalAggregator_(finalAggregator)
            , logger_(logger) {}

    virtual void grade(const TestSuite& testSuite, const ConstraintSuite& constraintSuite, const GraderConfig& config) {
        logger_->logIntroduction(config.solutionCommand());

        map<int, vector<Verdict>> verdictsBySubtaskId;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            gradeTestGroup(testGroup, config, verdictsBySubtaskId);
        }

        map<int, Subtask> subtasksById = getSubtasksToGrade(constraintSuite);
        map<int, Verdict> subtaskVerdictsById;
        vector<Verdict> subtaskVerdicts;

        for (auto& entry : verdictsBySubtaskId) {
            int subtaskId = entry.first;
            const vector<Verdict>& verdicts = entry.second;

            if (subtasksById.count(subtaskId)) {
                Verdict subtaskVerdict = aggregator_->aggregate(verdicts);
                subtaskVerdictsById[subtaskId] = subtaskVerdict;
                subtaskVerdicts.push_back(subtaskVerdict);
            }
        }
        Verdict verdict = finalAggregator_->aggregate(subtaskVerdicts);

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
            const GraderConfig& config,
            map<int, vector<Verdict>>& verdictsBySubtaskId) {

        logger_->logTestGroupIntroduction(testGroup.id());

        if (config.hasMultipleTestCases()) {
            TestCase testCase = TestCaseBuilder()
                    .setName(TestGroup::createName(config.slug(), testGroup.id()))
                    .setSubtaskIds(testGroup.testCases()[0].subtaskIds())
                    .build();
            gradeTestCase(testCase, config, verdictsBySubtaskId);
        } else {
            for (const TestCase& testCase : testGroup.testCases()) {
                gradeTestCase(testCase, config, verdictsBySubtaskId);
            }
        }
    }

    void gradeTestCase(
            const TestCase& testCase,
            const GraderConfig& config,
            map<int, vector<Verdict>>& verdictsBySubtaskId) {

        Verdict verdict = testCaseGrader_->grade(testCase, config);
        for (int subtaskId : testCase.subtaskIds()) {
            verdictsBySubtaskId[subtaskId].push_back(verdict);
        }
    }
};

class GraderFactory {
public:
    virtual Grader* create(TestCaseGrader* testCaseGrader, Aggregator* aggregator, GraderLogger* logger) {
        return new Grader(testCaseGrader, aggregator, new SumAggregator(), logger);
    }
};

}
