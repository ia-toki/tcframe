#pragma once

#include <string>

#include "EvaluationOptions.hpp"
#include "EvaluationResult.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"

using std::string;

namespace tcframe {

class Evaluator {
public:
    static constexpr const char* EVALUATION_OUT_FILENAME = "__tcframe_evaluation.out";

    virtual ~Evaluator() {}

    virtual EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) = 0;

    virtual GenerationResult generate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) = 0;

    virtual ScoringResult score(const string& inputFilename, const string& outputFilename) = 0;
};

}
