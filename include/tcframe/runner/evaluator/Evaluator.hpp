#pragma once

#include <stdexcept>
#include <string>

#include "EvaluationOptions.hpp"
#include "EvaluationResult.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"

using std::logic_error;
using std::string;

namespace tcframe {

class Evaluator {
public:
    static constexpr const char* EVALUATION_OUT_FILENAME = "__tcframe_evaluation.out";

    virtual ~Evaluator() = default;

    virtual EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) = 0;

    virtual GenerationResult generate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) {

        throw logic_error("unsupported");
    }

    virtual ScoringResult score(const string& inputFilename, const string& outputFilename) {
        throw logic_error("unsupported");
    }
};

}
