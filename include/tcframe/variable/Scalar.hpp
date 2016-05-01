#pragma once

#include <exception>
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

#include "Variable.hpp"

using std::enable_if;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::istream;
using std::ostream;
using std::ref;
using std::reference_wrapper;
using std::string;

namespace tcframe {

template<typename T>
using ScalarCompatible = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using NotScalarCompatible = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

class Scalar : public Variable {
public:
    virtual ~Scalar() {}

    Scalar(const string& name)
            : Variable(name, VariableType::SCALAR)
    {}

    virtual void printTo(ostream* out) = 0;
    virtual void parseFrom(istream* in) = 0;

    template<typename T>
    static Scalar* create(T& var, const string& name);
};

template<typename T>
class ScalarImpl : public Scalar {
private:
    reference_wrapper<T> var_;

public:
    ScalarImpl(T& var, const string& name)
            : Scalar(name)
            , var_(ref(var))
    {}

    void printTo(ostream* out) {
        *out << var_;
    }

    void parseFrom(istream* in) {
        *in >> var_;
    }
};

template<typename T>
Scalar* Scalar::create(T& var, const string& name) {
    return new ScalarImpl<T>(var, name);
}

}
