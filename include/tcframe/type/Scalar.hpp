#pragma once

#include <exception>
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

using std::enable_if;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::istream;
using std::ostream;
using std::ref;
using std::reference_wrapper;
using std::string;

namespace tcframe { namespace experimental {

template<typename T>
using ScalarCompatible = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using NotScalarCompatible = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

class Scalar {
public:
    virtual ~Scalar() {}

    virtual const string& getName() const = 0;
    virtual void printTo(ostream& out) const = 0;
    virtual void parseFrom(istream& in) const = 0;

    template<typename T>
    static Scalar* create(T& var, const string& name);
};

template<typename T>
class ScalarImpl : public Scalar {
private:
    reference_wrapper<T> var_;
    string name_;

public:
    ScalarImpl(T& var, const string& name)
            : var_(ref(var))
            , name_(name)
    {}

    const string& getName() const {
        return name_;
    }

    void printTo(ostream& out) const {
        out << var_;
    }

    void parseFrom(istream& in) const {
        in >> var_;
    }
};

template<typename T>
Scalar* Scalar::create(T& var, const string& name) {
    return new ScalarImpl<T>(var, name);
}

}}
