#pragma once

#include "gmock/gmock.h"

#include "tcframe/verifier/Verifier.hpp"

namespace tcframe {

class MockVerifier : public Verifier {
public:
    MockVerifier()
            : Verifier(ConstraintSuite())
    {}

    MOCK_METHOD1(verify, VerificationResult(const set<int>&));
};

}
