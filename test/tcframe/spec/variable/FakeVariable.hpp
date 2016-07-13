#pragma once

#include "tcframe/spec/variable/Variable.hpp"

namespace tcframe {

class FakeVariable : public Variable {
public:
    FakeVariable(const string& name, VariableType type)
            : Variable(name, type)
    {}

    template<typename T>
    void parseValue(istream* in, T& val, const string& context) {
        Variable::parseValue(in, val, context);
    }
};

}
