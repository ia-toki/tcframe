#pragma once

#include "gmock/gmock.h"

#include "tcframe/verification/ConstraintSuiteVerifier.hpp"

namespace tcframe {

class MockConstraintSuiteVerifier : public ConstraintSuiteVerifier {
public:
    MockConstraintSuiteVerifier()
            : ConstraintSuiteVerifier(ConstraintSuite())
    {}

    MOCK_METHOD1(verify, ConstraintSuiteVerificationResult(const set<int>&));
};

}
