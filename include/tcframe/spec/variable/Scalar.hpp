#pragma once

#include <exception>
#include <iostream>
#include <string>

#include "TokenFormatter.hpp"
#include "Variable.hpp"
#include "WhitespaceManipulator.hpp"

using std::istream;
using std::ostream;
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

    template<typename = void>
    static Scalar* createRaw(string& var, const string& name);
};

template<typename T, typename = ScalarCompatible<T>>
class ScalarImpl : public Scalar {
private:
    T* var_;

public:
    virtual ~ScalarImpl() {}

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

class RawScalarImpl : public ScalarImpl<string> {
private:
    string* var_;

public:
    virtual ~RawScalarImpl() {}

    RawScalarImpl(string& var, const string& name)
            : ScalarImpl(var, name)
            , var_(&var) {}

    void parseFrom(istream* in) {
        Variable::parseRawLine(in, *var_);
    }
};

template<typename T, typename>
Scalar* Scalar::create(T& var, const string& name) {
    return new ScalarImpl<T>(var, name);
}

template<typename>
Scalar* Scalar::createRaw(string& var, const string& name) {
    return new RawScalarImpl(var, name);
}

}
