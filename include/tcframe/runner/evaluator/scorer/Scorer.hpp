#pragma once

#include <string>

#include "ScoringResult.hpp"

using std::string;

namespace tcframe {

class Scorer {
public:
    static constexpr const char* SCORING_OUT_FILENAME = "__tcframe_scoring.out";

    virtual ~Scorer() = default;

    virtual ScoringResult score(
            const string& inputFilename,
            const string& outputFilename,
            const string& evaluationFilename) = 0;
};

}
