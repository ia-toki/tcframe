#pragma once

namespace tcframe {

enum FailureType {
    SIMPLE,
    CONSTRAINTS_VERIFICATION,
    MULTIPLE_TEST_CASES_CONSTRAINTS_VERIFICATION
};

struct Failure {
public:
    virtual ~Failure() {}

    virtual FailureType type() const = 0;

    virtual bool equals(Failure* o) const = 0;
};

}
