#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "SubmitterConfig.hpp"
#include "SubmitterLogger.hpp"
#include "TestCaseSubmitter.hpp"
#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::max;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

class Submitter {
private:
    TestCaseSubmitter* testCaseSubmitter_;
    SubmitterLogger* logger_;

public:
    virtual ~Submitter() {}

    Submitter(TestCaseSubmitter* testCaseSubmitter, SubmitterLogger* logger)
            : testCaseSubmitter_(testCaseSubmitter)
            , logger_(logger) {}

    virtual void submit(const TestSuite& testSuite, const set<int>& subtaskIds, const SubmitterConfig& config) {
        logger_->logIntroduction();

        map<int, Verdict> subtaskVerdicts;
        for (int subtaskId : subtaskIds) {
            subtaskVerdicts[subtaskId] = Verdict::ac();
        }

        for (const TestGroup& testGroup : testSuite.testGroups()) {
            submitOnTestGroup(testGroup, config, subtaskVerdicts);
        }

        logger_->logResult(subtaskVerdicts);
    }

private:
    void submitOnTestGroup(
            const TestGroup& testGroup,
            const SubmitterConfig& config,
            map<int, Verdict>& subtaskVerdicts) {

        logger_->logTestGroupIntroduction(testGroup.id());

        if (config.hasMultipleTestCasesCount()) {
            TestCase testCase = TestCaseBuilder()
                    .setId(TestCaseIdCreator::createBaseId(config.slug(), testGroup.id()))
                    .setSubtaskIds(testGroup.testCases()[0].subtaskIds())
                    .build();
            submitOnTestCase(testCase, config, subtaskVerdicts);
        } else {
            for (const TestCase& testCase : testGroup.testCases()) {
                submitOnTestCase(testCase, config, subtaskVerdicts);
            }
        }
    }

    void submitOnTestCase(const TestCase& testCase, const SubmitterConfig& config, map<int, Verdict>& subtaskVerdicts) {
        Verdict verdict = testCaseSubmitter_->submit(testCase, config);
        for (int subtaskId : testCase.subtaskIds()) {
            subtaskVerdicts[subtaskId] = max(subtaskVerdicts[subtaskId], verdict);
        }
    }
};

class SubmitterFactory {
public:
    virtual Submitter* create(TestCaseSubmitter* testCaseSubmitter, SubmitterLogger* logger) {
        return new Submitter(testCaseSubmitter, logger);
    }
};

}
