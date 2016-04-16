#pragma once

#include "tcframe/deprecated/iovariable/Variable.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class MatrixVariable : public Variable {
public:
    virtual ~MatrixVariable() { }

    virtual int rowsSize() = 0;
    virtual int columnsSize(int rowIndex) = 0;
    virtual void printElementTo(int rowIndex, int columnIndex, ostream& out) = 0;
    virtual void addRow() = 0;
    virtual void parseAndAddColumnElementFrom(istream& in) = 0;

protected:
    MatrixVariable(string name)
            : Variable(name) { }
};

}
