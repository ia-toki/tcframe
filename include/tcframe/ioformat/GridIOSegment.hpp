#pragma once

#include "tcframe/core/Util.hpp"
#include "tcframe/iovariable/Matrix.hpp"
#include "tcframe/iovariable/MatrixVariable.hpp"
#include "tcframe/iovariable/ParsingException.hpp"
#include "tcframe/iovariable/PrintingException.hpp"
#include "tcframe/iovariable/Scalar.hpp"
#include "tcframe/ioformat/MatrixSizes.hpp"
#include "tcframe/ioformat/IOFormatException.hpp"
#include "tcframe/ioformat/IOSegment.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe {

class GridIOSegment : public IOSegment {
public:
    GridIOSegment(string name)
            : name(name) { }

    template<typename T, typename = RequiresScalar<T>>
    GridIOSegment& operator,(vector<vector<T>>& matrix) {
        if (variable != nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        variable = new Matrix<T>(matrix, name);
        hasSpaces = !is_same<T, char>::value;
        return *this;
    }

    template<typename T>
    GridIOSegment& operator,(T) {
        throw IOFormatException("Variable type of `" + name + "` unsatisfied. Expected: matrix of basic scalar or string type");
    }

    GridIOSegment& operator%(MatrixSizes sizes) {
        rowsSize = sizes.getRowsSize();
        columnsSize = sizes.getColumnsize();
        return *this;
    }

    void checkState() {
        if (variable == nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        if (rowsSize == -1 || columnsSize == -1) {
            throw IOFormatException("Grid segment must define matrix sizes");
        }
    }

    void printTo(ostream& out) {
        checkState();
        checkMatrixSizes();

        for (int i = 0; i < variable->rowsSize(); i++) {
            for (int j = 0; j < variable->columnsSize(i); j++) {
                if (j > 0 && hasSpaces) {
                    out << " ";
                }
                variable->printElementTo(i, j, out);
            }
            out << "\n";
        }
    }

    void parseFrom(istream& in) {
        checkState();
        variable->clear();

        for (int i = 0; i < rowsSize; i++) {
            variable->addRow();
            for (int j = 0; j < columnsSize; j++) {
                if (j > 0 && hasSpaces) {
                    parseSpaceFrom(in, variable->getName() + "[" + Util::toString(i) + "][" + Util::toString(j - 1) + "]");
                }
                variable->parseAndAddColumnElementFrom(in);
            }
            parseNewLineFrom(in, variable->getName() + "[" + Util::toString(i) + "][" + Util::toString(rowsSize - 1) + "]");
        }
    }

private:
    string name;
    MatrixVariable* variable = nullptr;
    bool hasSpaces = false;
    int rowsSize = -1;
    int columnsSize = -1;

    void checkMatrixSizes() {
        if (variable->rowsSize() != rowsSize) {
            throw PrintingException("Number of rows of matrix `" + variable->getName() + "` unsatisfied. Expected: " + Util::toString(rowsSize) + ", actual: " + Util::toString(variable->rowsSize()));
        }

        for (int i = 0; i < rowsSize; i++) {
            if (variable->columnsSize(i) != columnsSize) {
                throw PrintingException("Number of columns row " + Util::toString(i) + " of matrix `" + variable->getName() + "` (0-based) unsatisfied. Expected: " + Util::toString(columnsSize) + ", actual: " + Util::toString(variable->columnsSize(i)));
            }
        }
    }
};

}
