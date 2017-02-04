#pragma once

#include "gmock/gmock.h"

namespace tcframe {

class Closure {
public:
    virtual ~Closure() {}

    virtual void call() {}
};

class MockClosure : public Closure {
public:
    MOCK_METHOD0(call, void());
};

}
