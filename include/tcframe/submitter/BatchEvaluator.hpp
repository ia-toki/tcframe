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

        ExecutionRequestBuilder request = ExecutionRequestBuilder()
                .setCommand(config.solutionCommand())
                .setInputFilename(inputFilename)
                .setOutputFilename("_evaluation.out")
                .setErrorFilename("_error.out");

        if (config.timeLimit()) {
            request.setTimeLimit(config.timeLimit().value());
        }
        if (config.memoryLimit()) {
            request.setMemoryLimit(config.memoryLimit().value());
        }

        ExecutionResult result = os_->execute(request.build());

        if (result.info().isSuccessful()) {
            return optional<Verdict>();
        }

        if (result.info().exceededCpuLimits()) {
            logger_->logTestCaseVerdict(Verdict::tle());
            return optional<Verdict>(Verdict::tle());
        }

        logger_->logTestCaseVerdict(Verdict::rte());
        logger_->logSolutionExecutionFailure(result);
        return optional<Verdict>(Verdict::rte());
    }
};

}
