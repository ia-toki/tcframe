#pragma once

#include "TestCaseGenerationFailure.hpp"
#include "tcframe/verification.hpp"

namespace tcframe {

class VerificationFailure : public TestCaseGenerationFailure {
private:
    ConstraintSuiteVerificationResult verificationResult_;

public:
    VerificationFailure(const ConstraintSuiteVerificationResult& verificationResult)
            : verificationResult_(verificationResult)
    {}

    const ConstraintSuiteVerificationResult& verificationResult() const {
        return verificationResult_;
    }
};

}
