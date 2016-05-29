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

    FailureType type() const {
        return FailureType::VERIFICATION;
    }

    const ConstraintSuiteVerificationResult& verificationResult() const {
        return verificationResult_;
    }

    bool operator==(const VerificationFailure& o) const {
        return tie(verificationResult_) == tie(o.verificationResult_);
    }

    bool equals(TestCaseGenerationFailure* o) const {
        return type() == o->type() && *this == *((VerificationFailure *) o);
    }
};

}
