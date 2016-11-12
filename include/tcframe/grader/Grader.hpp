#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "TestCaseGrader.hpp"
#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::max;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

class Grader {
private:
    TestCaseGrader* testCaseGrader_;
    GraderLogger* logger_;

public:
    virtual ~Grader() {}

    Grader(TestCaseGrader* testCaseGrader, GraderLogger* logger)
            : testCaseGrader_(testCaseGrader)
            , logger_(logger) {}

    virtual void grade(const TestSuite& testSuite, const set<int>& subtaskIds, const GraderConfig& config) {
        logger_->logIntroduction();

        map<int, Verdict> subtaskVerdicts;
        for (int subtaskId : subtaskIds) {
            subtaskVerdicts[subtaskId] = Verdict::ac();
        }

        for (const TestGroup& testGroup : testSuite.testGroups()) {
            gradeOnTestGroup(testGroup, config, subtaskVerdicts);
        }

        logger_->logResult(subtaskVerdicts);
    }

private:
    void gradeOnTestGroup(
            const TestGroup& testGroup,
            const GraderConfig& config,
            map<int, Verdict>& subtaskVerdicts) {

        logger_->logTestGroupIntroduction(testGroup.id());

        if (config.hasMultipleTestCases()) {
            TestCase testCase = TestCaseBuilder()
                    .setId(TestCaseIdCreator::createBaseId(config.slug(), testGroup.id()))
                    .setSubtaskIds(testGroup.testCases()[0].subtaskIds())
                    .build();
            gradeOnTestCase(testCase, config, subtaskVerdicts);
        } else {
            for (const TestCase& testCase : testGroup.testCases()) {
                gradeOnTestCase(testCase, config, subtaskVerdicts);
            }
        }
    }

    void gradeOnTestCase(const TestCase& testCase, const GraderConfig& config, map<int, Verdict>& subtaskVerdicts) {
        Verdict verdict = testCaseGrader_->grade(testCase, config);
        for (int subtaskId : testCase.subtaskIds()) {
            subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], verdict);
        }
    }
};

class GraderFactory {
public:
    virtual Grader* create(TestCaseGrader* testCaseGrader, GraderLogger* logger) {
        return new Grader(testCaseGrader, logger);
    }
};

}
