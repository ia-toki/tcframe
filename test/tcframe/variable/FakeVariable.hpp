#pragma once

#include "tcframe/variable/Variable.hpp"

namespace tcframe {

class FakeVariable : public Variable {
public:
    FakeVariable(const string& name, VariableType type)
            : Variable(name, type)
    {}
};

}
