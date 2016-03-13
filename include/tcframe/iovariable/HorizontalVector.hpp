#pragma once

#include "tcframe/core/Util.hpp"
#include "tcframe/iovariable/VectorHorizontalVariable.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

template<typename T>
class HorizontalVector : public VectorHorizontalVariable {
public:
    HorizontalVector(vector<T>& value, string name)
            : VectorHorizontalVariable(name), value(&value) { }

    int size() {
        return (int)(value->size());
    }

    void printElementTo(int index, ostream& out) {
        out << (*value)[index];
    }

    void parseAndAddElementFrom(istream& in) {
        int index = (int)value->size();
        T element;
        parseVariableFrom(in, &element, getName() + "[" + Util::toString(index) + "]");
        value->push_back(element);
    }

    void clear() {
        *value = vector<T>();
    }

private:
    vector<T>* value;
};

}
