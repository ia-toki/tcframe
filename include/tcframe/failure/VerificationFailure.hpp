#pragma once

#include <tuple>

#include "Failure.hpp"
#include "tcframe/verifier.hpp"

using std::tie;

namespace tcframe {

class VerificationFailure : public Failure {
private:
    VerificationResult verificationResult_;

public:
    VerificationFailure(const VerificationResult& verificationResult)
            : verificationResult_(verificationResult)
    {}

    FailureType type() const {
        return FailureType::VERIFICATION;
    }

    const VerificationResult& verificationResult() const {
        return verificationResult_;
    }

    bool operator==(const VerificationFailure& o) const {
        return tie(verificationResult_) == tie(o.verificationResult_);
    }

    bool equals(Failure* o) const {
        return type() == o->type() && *this == *((VerificationFailure*) o);
    }
};

}
