#pragma once

#include <map>
#include <string>

#include "EvaluationOptions.hpp"
#include "EvaluationResult.hpp"
#include "Evaluator.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::string;

namespace tcframe {

class BatchEvaluator : public Evaluator {
private:
    OperatingSystem* os_;
    VerdictCreator* verdictCreator_;
    Scorer* scorer_;

public:
    virtual ~BatchEvaluator() = default;

    BatchEvaluator(OperatingSystem* os, VerdictCreator* verdictCreator, Scorer* scorer)
            : os_(os)
            , verdictCreator_(verdictCreator)
            , scorer_(scorer) {}

    EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) {

        map<string, ExecutionResult> executionResults;
        Verdict verdict;

        GenerationResult generationResult = generate(inputFilename, EVALUATION_OUT_FILENAME, options);
        executionResults["solution"] = generationResult.executionResult();

        if (generationResult.verdict()) {
            verdict = generationResult.verdict().value();
        } else {
            ScoringResult scoringResult = score(inputFilename, outputFilename);
            executionResults["scorer"] = scoringResult.executionResult();
            verdict = scoringResult.verdict();
        }

        return {verdict, executionResults};
    }

    GenerationResult generate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) {

        auto request = ExecutionRequestBuilder()
                .setCommand(options.solutionCommand())
                .setInputFilename(inputFilename)
                .setOutputFilename(outputFilename);

        if (options.timeLimit()) {
            request.setTimeLimit(options.timeLimit().value());
        }
        if (options.memoryLimit()) {
            request.setMemoryLimit(options.memoryLimit().value());
        }

        ExecutionResult executionResult = os_->execute(request.build());
        optional<Verdict> verdict = verdictCreator_->fromExecutionResult(executionResult);
        return {verdict, executionResult};
    }

    ScoringResult score(const string& inputFilename, const string& outputFilename) {
        return scorer_->score(inputFilename, outputFilename, EVALUATION_OUT_FILENAME);
    }
};

}
