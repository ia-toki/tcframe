#pragma once

#include "gmock/gmock.h"

#include "tcframe/spec/verifier/Verifier.hpp"

namespace tcframe {

class MockVerifier : public Verifier {
public:
    MockVerifier()
            : Verifier(ConstraintSuite()) {}

    MOCK_METHOD1(verifyConstraints, ConstraintsVerificationResult(const set<int>&));
    MOCK_METHOD0(verifyMultipleTestCasesConstraints, MultipleTestCasesConstraintsVerificationResult());
};

}
