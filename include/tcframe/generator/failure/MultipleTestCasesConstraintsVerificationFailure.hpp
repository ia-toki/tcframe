#pragma once

#include <tuple>

#include "Failure.hpp"
#include "tcframe/verifier.hpp"

using std::tie;

namespace tcframe {

class MultipleTestCasesConstraintsVerificationFailure : public Failure {
private:
    MultipleTestCasesConstraintsVerificationResult verificationResult_;

public:
    MultipleTestCasesConstraintsVerificationFailure(
            const MultipleTestCasesConstraintsVerificationResult& verificationResult)
            : verificationResult_(verificationResult)
    {}

    FailureType type() const {
        return FailureType::MULTIPLE_TEST_CASES_CONSTRAINTS_VERIFICATION;
    }

    const MultipleTestCasesConstraintsVerificationResult& verificationResult() const {
        return verificationResult_;
    }

    bool operator==(const MultipleTestCasesConstraintsVerificationFailure& o) const {
        return tie(verificationResult_) == tie(o.verificationResult_);
    }

    bool equals(Failure* o) const {
        return type() == o->type() && *this == *((MultipleTestCasesConstraintsVerificationFailure*) o);
    }
};

}
