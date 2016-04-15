#pragma once

#include <utility>

#include "tcframe/experimental/generation/TestCaseGenerationFailure.hpp"
#include "tcframe/experimental/verification/ConstraintSuiteVerificationResult.hpp"

using std::move;

namespace tcframe { namespace experimental {

class VerificationFailure : public TestCaseGenerationFailure {
private:
    ConstraintSuiteVerificationResult verificationResult_;

public:
    VerificationFailure(ConstraintSuiteVerificationResult verificationResult)
            : verificationResult_(move(verificationResult))
    {}

    const ConstraintSuiteVerificationResult& verificationResult() const {
        return verificationResult_;
    }
};

}}
