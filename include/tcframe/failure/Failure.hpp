#pragma once

namespace tcframe {

enum FailureType {
    CONSTRAINTS_VERIFICATION,
    MULTIPLE_TEST_CASES_CONSTRAINTS_VERIFICATION,
    SIMPLE,
    SOLUTION_EXECUTION
};

struct Failure {
public:
    virtual ~Failure() {}

    virtual FailureType type() const = 0;

    virtual bool equals(Failure* o) const = 0;
};

}
