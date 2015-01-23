#ifndef CPTEST_TYPE_H
#define CPTEST_TYPE_H

#include <ostream>

using std::ostream;

namespace org { namespace iatoki { namespace cptest {

class Variable {
public:
    virtual void printTo(ostream& out) = 0;
    virtual ~Variable() {};
};

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


}}}

#endif