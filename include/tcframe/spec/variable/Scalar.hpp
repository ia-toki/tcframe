#pragma once

#include <exception>
#include <iostream>
#include <string>

#include "TokenFormatter.hpp"
#include "Variable.hpp"
#include "WhitespaceManipulator.hpp"

using std::istream;
using std::ostream;
using std::ref;
using std::string;

namespace tcframe {

class Scalar : public Variable {
public:
    virtual ~Scalar() {}

    Scalar(const string& name)
            : Variable(name, VariableType::SCALAR) {}

    virtual void printTo(ostream* out) = 0;
    virtual void parseFrom(istream* in) = 0;

    template<typename T, typename = ScalarCompatible<T>>
    static Scalar* create(T& var, const string& name);
};

template<typename T, typename = ScalarCompatible<T>>
class ScalarImpl : public Scalar {
private:
    T* var_;

public:
    ScalarImpl(T& var, const string& name)
            : Scalar(name)
            , var_(&var) {}

    void printTo(ostream* out) {
        *out << *var_;
    }

    void parseFrom(istream* in) {
        Variable::parseValue(in, *var_, TokenFormatter::formatVariable(name()));
    }
};

template<typename T, typename>
Scalar* Scalar::create(T& var, const string& name) {
    return new ScalarImpl<T>(var, name);
}

}
