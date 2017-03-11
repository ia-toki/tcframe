#pragma once

#include <string>

#include "EvaluatorConfig.hpp"
#include "tcframe/grade.hpp"

using std::string;

namespace tcframe {

class Evaluator {
public:
    static constexpr const char* EVALUATION_FILENAME = "_evaluation.out";

    virtual ~Evaluator() {}

    virtual EvaluationResult evaluate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluatorConfig& config) = 0;
};

}
