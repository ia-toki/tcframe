#pragma once

#include <string>

#include "ScoringResult.hpp"

using std::string;

namespace tcframe {

class Scorer {
public:
    virtual ~Scorer() {}

    virtual ScoringResult score(
            const string& inputFilename,
            const string& outputFilename,
            const string& evaluationFilename) = 0;
};

}
