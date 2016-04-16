#pragma once

#include "tcframe/deprecated/core/Util.hpp"
#include "tcframe/deprecated/iovariable/HorizontalVariable.hpp"
#include "tcframe/deprecated/iovariable/HorizontalVector.hpp"
#include "tcframe/deprecated/iovariable/Matrix.hpp"
#include "tcframe/deprecated/iovariable/MatrixVariable.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"
#include "tcframe/deprecated/iovariable/Scalar.hpp"
#include "tcframe/deprecated/iovariable/VerticalVariable.hpp"
#include "tcframe/deprecated/iovariable/VerticalVector.hpp"
#include "tcframe/deprecated/ioformat/IOFormatException.hpp"
#include "tcframe/deprecated/ioformat/IOSegment.hpp"
#include "tcframe/deprecated/ioformat/VectorSize.hpp"
#include "tcframe/deprecated/ioformat/VectorWithSize.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe {

class LinesIOSegment : public IOSegment {
public:
    LinesIOSegment(string names) {
        this->names = Util::split(names);
    }

    template<typename T, typename = RequiresScalar<T>>
    LinesIOSegment& operator,(vector<T>& vektor) {
        checkJaggedVector();

        variables.push_back(new VerticalVector<T>(vektor, names[variables.size()]));
        return *this;
    }

    template<typename T, typename = RequiresScalar<T>>
    LinesIOSegment& operator,(vector<vector<T>>& matrix) {
        checkJaggedVector();

        jaggedVector = new Matrix<T>(matrix, names[variables.size()]);
        return *this;
    }

    template<typename T>
    LinesIOSegment& operator,(T) {
        throw IOFormatException("Variable type of `" + names[variables.size()] + "` unsatisfied. Expected: (jagged) vector of basic scalar or string type");
    }

    LinesIOSegment& operator%(VectorSize vektorSize) {
        setSize(vektorSize.getSize());
        return *this;
    }

    void checkState() {
        if (variables.empty() && jaggedVector == nullptr) {
            throw IOFormatException("Lines segment must have at least one variable");
        }

        if (size == -1) {
            throw IOFormatException("Lines segment must define vector sizes");
        }
    }

    void printTo(ostream& out) {
        checkState();
        checkVectorSizes();

        for (int i = 0; i < size; i++) {
            bool first = true;
            for (VerticalVariable* variable : variables) {
                if (!first) {
                    out << " ";
                }
                first = false;
                variable->printElementTo(i, out);
            }

            if (jaggedVector != nullptr) {
                for (int j = 0; j < jaggedVector->columnsSize(i); j++) {
                    if (!first) {
                        out << " ";
                    }
                    first= false;
                    jaggedVector->printElementTo(i, j, out);
                }
            }
            out << "\n";
        }
    }

    void parseFrom(istream& in) {
        checkState();

        for (VerticalVariable* variable : variables) {
            variable->clear();
        }
        if (jaggedVector != nullptr) {
            jaggedVector->clear();
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < variables.size(); j++) {
                VerticalVariable* variable = variables[j];
                if (j > 0) {
                    parseSpaceFrom(in, variables[j - 1]->getName() + "[" + Util::toString(i) + "]");
                }
                variable->parseAndAddElementFrom(in);
            }

            if (jaggedVector != nullptr) {
                if (!variables.empty()) {
                    parseSpaceFrom(in, variables.back()->getName() + "[" + Util::toString(i) + "]");
                }
                jaggedVector->addRow();
                for (int j = 0; ; j++) {
                    int c = in.peek();

                    if (j == 0) {
                        if (c == '\n') {
                            break;
                        }
                    } else {
                        if (c == '\n') {
                            break;
                        } else if (c == ' ') {
                            in.get();
                        } else {
                            throw ParsingException("Expected: <space> or <new line> after variable `" + jaggedVector->getName() + "[" + Util::toString(i) + "][" + Util::toString(j - 1) + "]`");
                        }
                    }

                    jaggedVector->parseAndAddColumnElementFrom(in);
                }
                parseNewLineFrom(in, jaggedVector->getName() + "[" + Util::toString(i) + "]");
            } else {
                parseNewLineFrom(in, variables.back()->getName() + "[" + Util::toString(i) + "]");
            }
        }
    }

private:
    vector<string> names;
    vector<VerticalVariable*> variables;
    MatrixVariable* jaggedVector = nullptr;
    int size = -1;

    void setSize(int size) {
        this->size = size;
    }

    void checkJaggedVector() {
        if (jaggedVector != nullptr) {
            throw IOFormatException("Jagged vector can only be the last variable in a lines segment");
        }
    }

    void checkVectorSizes() {
        for (VerticalVariable* variable : variables) {
            if (variable->size() != size) {
                throw PrintingException("Number of elements of vector `" + variable->getName() + "` unsatisfied. Expected: " + Util::toString(size) + ", actual: " + Util::toString(variable->size()));
            }
        }

        if (jaggedVector != nullptr) {
            if (jaggedVector->rowsSize() != size) {
                throw PrintingException("Number of rows of jagged vector `" + jaggedVector->getName() + "` unsatisfied. Expected: " + Util::toString(size) + ", actual: " + Util::toString(
                        jaggedVector->rowsSize()));
            }
        }
    }
};

}
