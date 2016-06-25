#pragma once

#include <tuple>
#include <vector>

#include "TestGroupGenerationResult.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct GenerationResult {
private:
    vector<TestGroupGenerationResult> testGroupResults_;

public:
    GenerationResult(const vector<TestGroupGenerationResult>& testGroupResults)
            : testGroupResults_(testGroupResults)
    {}

    const vector<TestGroupGenerationResult>& testGroupResults() const {
        return testGroupResults_;
    }

    bool isSuccessful() const {
        for (auto result : testGroupResults_) {
            if (!result.isSuccessful()) {
                return false;
            }
        }
        return true;
    }

    bool operator==(const GenerationResult& o) const {
        return tie(testGroupResults_) == tie(o.testGroupResults_);
    }
};

}
