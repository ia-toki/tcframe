#pragma once

namespace tcframe {

enum FailureType {
    SIMPLE,
    VERIFICATION
};

struct Failure {
public:
    virtual ~Failure() {}

    virtual FailureType type() const = 0;

    virtual bool equals(Failure* o) const = 0;
};

}
