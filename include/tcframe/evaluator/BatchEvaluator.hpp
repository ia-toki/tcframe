#pragma once

#include "Evaluator.hpp"
#include "EvaluatorConfig.hpp"
#include "tcframe/grade.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

namespace tcframe {

class BatchEvaluator : public Evaluator {
private:
    OperatingSystem* os_;

public:
    virtual ~BatchEvaluator() {}

    BatchEvaluator(OperatingSystem* os)
            : os_(os) {}

    EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) {

        ExecutionRequestBuilder request = ExecutionRequestBuilder()
                .setCommand(config.solutionCommand())
                .setInputFilename(inputFilename)
                .setOutputFilename(outputFilename);

        if (config.timeLimit()) {
            request.setTimeLimit(config.timeLimit().value());
        }
        if (config.memoryLimit()) {
            request.setMemoryLimit(config.memoryLimit().value());
        }

        ExecutionResult executionResult = os_->execute(request.build());
        return createResult(executionResult);
    }

private:
    static EvaluationResult createResult(const ExecutionResult& executionResult) {
        EvaluationResultBuilder result;
        result.setExecutionResult(executionResult);

        if (executionResult.exceededCpuLimits()) {
            result.setVerdict(Verdict::tle());
        } else if (!executionResult.isSuccessful()) {
            result.setVerdict(Verdict::rte());
        }

        return result.build();
    }
};

}
