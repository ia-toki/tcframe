#pragma once

#include <string>

#include "EvaluationResult.hpp"
#include "EvaluatorConfig.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"

using std::string;

namespace tcframe {

class Evaluator {
public:
    static constexpr const char* EVALUATION_OUT_FILENAME = "_evaluation.out";

    virtual ~Evaluator() {}

    virtual EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) = 0;

    virtual GenerationResult generate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) = 0;

    virtual ScoringResult score(const string& inputFilename, const string& outputFilename) = 0;
};

}
