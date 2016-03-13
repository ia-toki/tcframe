#pragma once

#include "tcframe/core/Util.hpp"
#include "tcframe/iovariable/MatrixVariable.hpp"

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
class Matrix : public MatrixVariable {
public:
    Matrix(vector<vector<T>>& value, string name)
            : MatrixVariable(name), value(&value) { }

    int rowsSize() {
        return (int)(value->size());
    }

    int columnsSize(int rowIndex) {
        return (int)((*value)[rowIndex].size());
    }

    void printElementTo(int rowIndex, int columnIndex, ostream& out) {
        out << (*value)[rowIndex][columnIndex];
    }

    void addRow() {
        value->push_back(vector<T>());
    }

    void parseAndAddColumnElementFrom(istream& in) {
        int rowIndex = (int)value->size() - 1;
        int columnIndex = (int)value->back().size();

        T element;
        parseVariableFrom(in, &element, getName() + "[" + Util::toString(rowIndex) + "]" + "[" + Util::toString(columnIndex) + "]");
        value->back().push_back(element);
    }

    void clear() {
        *value = vector<vector<T>>();
    }

private:
    vector<vector<T>>* value;
};

}
