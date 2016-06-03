#pragma once

#include "gmock/gmock.h"

#include "tcframe/verifier/ConstraintSuiteVerifier.hpp"

namespace tcframe {

class MockConstraintSuiteVerifier : public ConstraintSuiteVerifier {
public:
    MockConstraintSuiteVerifier()
            : ConstraintSuiteVerifier(ConstraintSuite())
    {}

    MOCK_METHOD1(verify, VerificationResult(const set<int>&));
};

}
