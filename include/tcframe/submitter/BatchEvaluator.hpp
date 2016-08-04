#pragma once

#include "Evaluator.hpp"
#include "SubmitterConfig.hpp"
#include "SubmitterLogger.hpp"
#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"
#include "tcframe/util.hpp"

namespace tcframe {

class BatchEvaluator : public Evaluator {
private:
    OperatingSystem* os_;
    SubmitterLogger* logger_;

public:
    BatchEvaluator(OperatingSystem* os, SubmitterLogger* logger)
            : os_(os)
            , logger_(logger) {}

    optional<Verdict> evaluate(const TestCase& testCase, const SubmitterConfig& config) {
        string inputFilename = config.testCasesDir() + "/" + testCase.id() + ".in";

        ExecutionResult result = os_->execute(
                config.solutionCommand(),
                inputFilename,
                "_evaluation.out",
                "_error.out");

        if (result.info().exitStatus() == 0) {
            return optional<Verdict>();
        }

        logger_->logTestCaseVerdict(Verdict::rte());
        logger_->logSolutionExecutionFailure(result);
        return optional<Verdict>(Verdict::rte());
    }
};

}
