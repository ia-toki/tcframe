#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "GradingOptions.hpp"
#include "GraderLogger.hpp"
#include "TestCaseGrader.hpp"
#include "tcframe/runner/os.hpp"
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
    TestCaseAggregator* testCaseAggregator_;
    SubtaskAggregator* subtaskAggregator_;
    GraderLogger* logger_;

public:
    virtual ~Grader() = default;

    Grader(
            SpecClient* specClient,
            TestCaseGrader* testCaseGrader,
            TestCaseAggregator* testCaseAggregator,
            SubtaskAggregator* subtaskAggregator,
            GraderLogger* logger)
            : specClient_(specClient)
            , testCaseGrader_(testCaseGrader)
            , testCaseAggregator_(testCaseAggregator)
            , subtaskAggregator_(subtaskAggregator)
            , logger_(logger) {}

    virtual void grade(const GradingOptions& options) {
        logger_->logIntroduction(options.solutionCommand());

        TestSuite testSuite = specClient_->getTestSuite();
        bool hasMultipleTestCases = specClient_->hasMultipleTestCases();

        map<int, vector<TestCaseVerdict>> verdictsBySubtaskId;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            gradeTestGroup(testGroup, options, hasMultipleTestCases, verdictsBySubtaskId);
        }

        map<int, double> subtaskPointsById = getSubtaskPoints(options);
        map<int, SubtaskVerdict> subtaskVerdictsById;
        vector<SubtaskVerdict> subtaskVerdicts;

        for (const auto& entry : verdictsBySubtaskId) {
            int subtaskId = entry.first;
            const vector<TestCaseVerdict>& verdicts = entry.second;

            if (subtaskPointsById.count(subtaskId)) {
                SubtaskVerdict subtaskVerdict = testCaseAggregator_->aggregate(verdicts, subtaskPointsById[subtaskId]);
                subtaskVerdictsById[subtaskId] = subtaskVerdict;
                subtaskVerdicts.push_back(subtaskVerdict);
            }
        }
        SubtaskVerdict verdict = subtaskAggregator_->aggregate(subtaskVerdicts);

        logger_->logResult(subtaskVerdictsById, verdict);
    }

private:
    map<int, double> getSubtaskPoints(const GradingOptions& options) {
        map<int, double> subtaskPointsByIds;
        for (unsigned id = 1; id <= options.subtaskPoints().size(); id++) {
            subtaskPointsByIds[id] = options.subtaskPoints()[id - 1];
        }
        if (subtaskPointsByIds.empty()) {
            subtaskPointsByIds[Subtask::MAIN_ID] = Subtask::MAIN_POINTS;
        }

        return subtaskPointsByIds;
    }

    void gradeTestGroup(
            const TestGroup& testGroup,
            const GradingOptions& options,
            bool hasMultipleTestCases,
            map<int, vector<TestCaseVerdict>>& verdictsBySubtaskId) {

        if (hasMultipleTestCases && testGroup.testCases().empty()) {
            return;
        }

        logger_->logTestGroupIntroduction(testGroup.id());

        if (hasMultipleTestCases) {
            auto testCase = TestCaseBuilder()
                    .setName(TestGroup::createName(options.slug(), testGroup.id()))
                    .setSubtaskIds(testGroup.testCases()[0].subtaskIds())
                    .build();
            gradeTestCase(testCase, options, verdictsBySubtaskId);
        } else {
            bool skipping = 0;
            for (const TestCase& testCase : testGroup.testCases()) {
                TestCaseVerdict verdict = gradeTestCase(testCase, options, verdictsBySubtaskId, skipping);
                if(verdict.verdict().code() != "OK" && verdict.verdict().code() != "AC") {
                    // comment this code if you dont want to skip
                    skipping = 1;
                }
            }
        }
    }

    TestCaseVerdict gradeTestCase(
            const TestCase& testCase,
            const GradingOptions& options,
            map<int, vector<TestCaseVerdict>>& verdictsBySubtaskId,
            bool skipTestCase = 0) {
        TestCaseVerdict verdict;
        if(skipTestCase) verdict = TestCaseVerdict(Verdict::skip());
        else verdict = testCaseGrader_->grade(testCase, options);
        for (int subtaskId : testCase.subtaskIds()) {
            verdictsBySubtaskId[subtaskId].push_back(verdict);
        }
        return verdict;
    }
};

class GraderFactory {
public:
    virtual ~GraderFactory() = default;

    virtual Grader* create(
            SpecClient* specClient,
            TestCaseGrader* testCaseGrader,
            TestCaseAggregator* aggregator,
            SubtaskAggregator* subtaskAggregator,
            GraderLogger* logger) {
        return new Grader(specClient, testCaseGrader, aggregator, subtaskAggregator, logger);
    }
};

}
