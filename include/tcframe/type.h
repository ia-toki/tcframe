#ifndef TCFRAME_TYPE_H
#define TCFRAME_TYPE_H

#include <ostream>
#include <type_traits>

using std::enable_if;
using std::integral_constant;
using std::is_arithmetic;
using std::is_same;
using std::ostream;
using std::string;

namespace tcframe {

class Variable {
public:
    virtual void printTo(ostream& out) = 0;
    virtual ~Variable() { };
};

template<typename T>
using RequiresScalar = typename enable_if<integral_constant<bool, is_arithmetic<T>::value || is_same<string, T>::value>::value>::type;

template<typename T>
class Scalar : public Variable {
private:
    T* value;

public:
    explicit Scalar(T& value) {
        this->value = &value;
    }

    void printTo(ostream& out) {
        out << *value;
    }
};

}

#endif
