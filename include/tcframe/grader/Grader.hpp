#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "TestCaseGrader.hpp"
#include "tcframe/grade.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
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

    virtual void grade(const TestSuite& testSuite, const ConstraintSuite& constraintSuite, const GraderConfig& config) {
        logger_->logIntroduction(config.solutionCommand());

        map<int, Verdict> subtaskVerdicts;
        for (int subtaskId : getSubtaskIdsToGrade(constraintSuite)) {
            subtaskVerdicts[subtaskId] = Verdict::ac();
        }

        for (const TestGroup& testGroup : testSuite.testGroups()) {
            gradeOnTestGroup(testGroup, config, subtaskVerdicts);
        }

        logger_->logResult(subtaskVerdicts);
    }

private:
    set<int> getSubtaskIdsToGrade(const ConstraintSuite& constraintSuite) {
        set<int> subtaskIds;
        for (const Subtask& subtask : constraintSuite.constraints()) {
            subtaskIds.insert(subtask.id());
        }

        // remove global constraints for problem with subtasks
        if (subtaskIds.size() > 1) {
            subtaskIds.erase(-1);
        }

        return subtaskIds;
    }

    void gradeOnTestGroup(
            const TestGroup& testGroup,
            const GraderConfig& config,
            map<int, Verdict>& subtaskVerdicts) {

        logger_->logTestGroupIntroduction(testGroup.id());

        if (config.hasMultipleTestCases()) {
            TestCase testCase = TestCaseBuilder()
                    .setName(TestCaseNameCreator::createBaseName(config.slug(), testGroup.id()))
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
        TestCaseGrade grade = testCaseGrader_->grade(testCase, config);
        for (int subtaskId : testCase.subtaskIds()) {
            subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], grade.verdict());
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
