#pragma once

#include "tcframe/deprecated/iovariable/ScalarHorizontalVariable.hpp"

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
using std::string;

namespace tcframe {

template<typename T>
using RequiresScalar = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using RequiresNotScalar = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

template<typename T>
class Scalar : public ScalarHorizontalVariable {
public:
    Scalar(T& value, string name)
            : ScalarHorizontalVariable(name), value(&value) { }

    void printTo(ostream& out) {
        out << *value;
    }

    void parseFrom(istream& in) {
        parseVariableFrom(in, value, getName());
    }

    void clear() {
        *value = T();
    }

private:
    T* value;
};

}
