#pragma once

#include <map>
#include <string>

#include "EvaluationResult.hpp"
#include "Evaluator.hpp"
#include "EvaluatorConfig.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;
using std::string;

namespace tcframe {

class BatchEvaluator : public Evaluator {
private:
    OperatingSystem* os_;
    VerdictCreator* verdictCreator_;
    Scorer* scorer_;

public:
    virtual ~BatchEvaluator() {}

    BatchEvaluator(OperatingSystem* os, VerdictCreator* verdictCreator, Scorer* scorer)
            : os_(os)
            , verdictCreator_(verdictCreator)
            , scorer_(scorer) {}

    EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) {

        map<string, ExecutionResult> executionResults;
        Verdict verdict;

        GenerationResult generationResult = generate(inputFilename, EVALUATION_OUT_FILENAME, config);
        executionResults["solution"] = generationResult.executionResult();

        if (generationResult.verdict()) {
            verdict = generationResult.verdict().value();
        } else {
            ScoringResult scoringResult = score(inputFilename, outputFilename);
            executionResults["scorer"] = scoringResult.executionResult();
            verdict = scoringResult.verdict();
        }

        return EvaluationResult(verdict, executionResults);
    }

    GenerationResult generate(
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
        optional<Verdict> verdict = verdictCreator_->fromExecutionResult(executionResult);
        return GenerationResult(verdict, executionResult);
    }

    ScoringResult score(const string& inputFilename, const string& outputFilename) {
        return scorer_->score(inputFilename, outputFilename, EVALUATION_OUT_FILENAME);
    }
};

}
