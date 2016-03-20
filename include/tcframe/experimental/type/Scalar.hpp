#pragma once

#include <exception>
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

using std::enable_if;
using std::exception;
using std::function;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::istream;
using std::ostream;
using std::string;

namespace tcframe { namespace experimental {

template<typename T>
using RequiresScalar = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using RequiresNotScalar = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

struct Scalar {
private:
    void* var_;
    string name_;
    function<void(istream&)> parseFunction_;
    function<void(ostream&)> printFunction_;

public:
    template<typename T, typename = RequiresScalar<T>>
    Scalar(T& var, string name)
            : var_(&var)
            , name_(name)
            , parseFunction_([this](istream& in) { in >> *reinterpret_cast<T*>(var_); })
            , printFunction_([this](ostream& out) { out << *reinterpret_cast<T*>(var_); })
    {}

    string name() const {
        return name_;
    }

    function<void(istream&)> parseFunction() const {
        return parseFunction_;
    }

    function<void(ostream&)> printFunction() const {
        return printFunction_;
    }
};

}}
