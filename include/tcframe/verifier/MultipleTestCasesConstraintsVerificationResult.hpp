#pragma once

#include <set>
#include <string>
#include <tuple>

using std::set;
using std::string;
using std::tie;

namespace tcframe {

struct MultipleTestCasesConstraintsVerificationResult {
private:
    set<string> unsatisfiedConstraintDescriptions_;

public:
    MultipleTestCasesConstraintsVerificationResult(const set<string>& unsatisfiedConstraintDescriptions)
            : unsatisfiedConstraintDescriptions_(unsatisfiedConstraintDescriptions) {}

    const set<string>& unsatisfiedConstraintDescriptions() const {
        return unsatisfiedConstraintDescriptions_;
    }

    bool isValid() const {
        return unsatisfiedConstraintDescriptions_.empty();
    }

    bool operator==(const MultipleTestCasesConstraintsVerificationResult& o) const {
        return tie(unsatisfiedConstraintDescriptions_) == tie(o.unsatisfiedConstraintDescriptions_);
    }
};

}
