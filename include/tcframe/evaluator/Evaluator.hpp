#pragma once

#include <string>

#include "EvaluationResult.hpp"
#include "EvaluatorConfig.hpp"

using std::string;

namespace tcframe {

class Evaluator {
public:
    virtual ~Evaluator() {}

    virtual EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) = 0;
};

}
