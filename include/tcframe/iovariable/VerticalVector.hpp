#pragma once

#include "tcframe/core/Util.hpp"
#include "tcframe/iovariable/VerticalVariable.hpp"

#include <istream>
#include <ostream>
#include <string>
#include <vector>

using std::istream;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

template<typename T>
class VerticalVector : public VerticalVariable {
public:
    VerticalVector(vector<T>& value, string name)
            : VerticalVariable(name), value(&value) { }

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
