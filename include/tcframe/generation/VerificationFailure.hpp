#pragma once

#include <tuple>

#include "TestCaseGenerationFailure.hpp"
#include "tcframe/verification.hpp"

using std::tie;

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

    bool operator==(const VerificationFailure& o) const {
        return tie(verificationResult_) == tie(o.verificationResult_);
    }
};

}
