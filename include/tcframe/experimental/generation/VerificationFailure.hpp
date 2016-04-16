#pragma once

#include "tcframe/experimental/generation/TestCaseGenerationFailure.hpp"
#include "tcframe/experimental/verification/ConstraintSuiteVerificationResult.hpp"

namespace tcframe { namespace experimental {

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

}}
