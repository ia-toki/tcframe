#pragma once

#include "gmock/gmock.h"

#include "tcframe/submitter/Scorer.hpp"

namespace tcframe {

class MockScorer : public Scorer {
public:
    MOCK_METHOD2(score, Verdict(const TestCase&, const SubmitterConfig&));
};

}
