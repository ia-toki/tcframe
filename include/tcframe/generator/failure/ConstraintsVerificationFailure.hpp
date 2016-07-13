#pragma once

#include <tuple>

#include "Failure.hpp"
#include "tcframe/verifier.hpp"

using std::tie;

namespace tcframe {

class ConstraintsVerificationFailure : public Failure {
private:
    ConstraintsVerificationResult verificationResult_;

public:
    ConstraintsVerificationFailure(const ConstraintsVerificationResult& verificationResult)
            : verificationResult_(verificationResult)
    {}

    FailureType type() const {
        return FailureType::CONSTRAINTS_VERIFICATION;
    }

    const ConstraintsVerificationResult& verificationResult() const {
        return verificationResult_;
    }

    bool operator==(const ConstraintsVerificationFailure& o) const {
        return tie(verificationResult_) == tie(o.verificationResult_);
    }

    bool equals(Failure* o) const {
        return type() == o->type() && *this == *((ConstraintsVerificationFailure*) o);
    }
};

}
