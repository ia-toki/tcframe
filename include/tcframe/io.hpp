#ifndef TCFRAME_IO_H
#define TCFRAME_IO_H

#include "exception.hpp"
#include "failure.hpp"
#include "type.hpp"

#include <cctype>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

using std::char_traits;
using std::is_same;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class IOSegment {
public:
    virtual ~IOSegment() { }

    virtual void printTo(ostream& out) = 0;
    virtual void parseFrom(istream& in) = 0;

protected:
    static void parseSpaceFrom(istream& in, string lastRealName) {
        if (in.get() != ' ') {
            throw ParsingException("Expected: <space> after variable `" + lastRealName + "`");
        }
    }

    static void parseNewLineFrom(istream& in, string lastRealName) {
        if (in.get() != '\n') {
            if (lastRealName == "") {
                throw ParsingException("Expected: <space> after empty line");
            } else {
                throw ParsingException("Expected: <new line> after variable `" + lastRealName + "`");
            }
        }
    }
};

class VectorSize {
public:
    VectorSize(int size)
            : size(size) { }

    int getSize() {
        return size;
    }

private:
    int size;
};

template<typename T>
struct VectorWithSize {
public:
    vector<T>* vektor;
    VectorSize size;
};

template<typename T>
VectorWithSize<T> operator%(vector<T>& v, VectorSize size) {
    return VectorWithSize<T>{&v, size};
}

class LineIOSegment : public IOSegment {
public:
    LineIOSegment(string names) {
        this->names = Util::split(names);
    }

    template<typename T, typename = RequiresScalar<T>>
    LineIOSegment& operator,(T& scalar) {
        variables.push_back(new Scalar<T>(scalar, names[variables.size()]));
        vectorSizes.push_back(0);
        return *this;
    }

    template<typename T, typename = RequiresScalar<T>>
    LineIOSegment& operator,(VectorWithSize<T> vektorWithSize) {
        variables.push_back(new HorizontalVector<T>(*vektorWithSize.vektor, names[variables.size()]));
        vectorSizes.push_back(vektorWithSize.size.getSize());
        return *this;
    }

    template<typename T, typename = RequiresNotScalar<T>>
    LineIOSegment& operator,(T) {
        throw IOFormatException("Variable type of `" + names[variables.size()] + "` unsatisfied. Expected: basic scalar or string type, or vector of those types");
    }

    void printTo(ostream& out) {
        bool first = true;
        for (int i = 0; i < variables.size(); i++) {
            HorizontalVariable* variable = variables[i];

            if (!first) {
                out << " ";
            }
            first = false;

            if (dynamic_cast<ScalarHorizontalVariable*>(variable) != nullptr) {
                printScalarTo((ScalarHorizontalVariable*)variable, out);
            } else {
                printVectorTo((VectorHorizontalVariable*)variable, vectorSizes[i], out);
            }
        }
        out << "\n";
    }

    void parseFrom(istream& in) {
        for (int i = 0; i < variables.size(); i++) {
            HorizontalVariable* variable = variables[i];

            if (i > 0) {
                parseSpaceFrom(in, variables[i - 1]->getName());
            }

            if (dynamic_cast<ScalarHorizontalVariable*>(variable) != nullptr) {
                parseScalarFrom((ScalarHorizontalVariable*)variable, in);
            } else {
                parseVectorFrom((VectorHorizontalVariable*)variable, vectorSizes[i], in);
            }
        }

        if (variables.empty()) {
            parseNewLineFrom(in, "");
        } else {
            parseNewLineFrom(in, variables.back()->getName());
        }
    }

private:
    vector<string> names;
    vector<HorizontalVariable*> variables;
    vector<int> vectorSizes;

    void printScalarTo(ScalarHorizontalVariable* scalar, ostream& out) {
        scalar->printTo(out);
    }

    void checkVectorSize(VectorHorizontalVariable* vektor, int size) {
        if (vektor->size() != size) {
            throw PrintingException("Number of elements of vector `" + vektor->getName() + "` unsatisfied. Expected: " + Util::toString(size) + ", actual: " + Util::toString(vektor->size()));
        }
    }

    void printVectorTo(VectorHorizontalVariable* vektor, int size, ostream& out) {
        checkVectorSize(vektor, size);

        bool first = true;
        for (int i = 0; i < size; i++) {
            if (!first) {
                out << " ";
            }
            first = false;

            vektor->printElementTo(i, out);
        }
    }

    void parseScalarFrom(ScalarHorizontalVariable* scalar, istream& in) {
        scalar->parseFrom(in);
    }

    void parseVectorFrom(VectorHorizontalVariable* vektor, int size, istream& in) {
        vektor->clear();

        for (int i = 0; i < size; i++) {
            if (i > 0) {
                parseSpaceFrom(in, vektor->getName() + "[" + Util::toString(i - 1) + "]");
            }
            vektor->parseAndAddElementFrom(in);
        }
    }
};

class LinesIOSegment : public IOSegment {
public:
    LinesIOSegment(string names) {
        this->names = Util::split(names);
    }

    template<typename T, typename = RequiresScalar<T>>
    LinesIOSegment& operator,(vector<T>& vektor) {
        variables.push_back(new VerticalVector<T>(vektor, names[variables.size()]));
        return *this;
    }

    template<typename T>
    LinesIOSegment& operator,(T) {
        throw IOFormatException("Variable type of `" + names[variables.size()] + "` unsatisfied. Expected: vector of basic scalar or string type");
    }

    LinesIOSegment& operator%(VectorSize vektorSize) {
        setSize(vektorSize.getSize());
        return *this;
    }

    void checkState() {
        if (variables.empty()) {
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
            out << "\n";
        }
    }

    void parseFrom(istream& in) {
        checkState();

        for (VerticalVariable* variable : variables) {
            variable->clear();
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < variables.size(); j++) {
                VerticalVariable* variable = variables[j];
                if (j > 0) {
                    parseSpaceFrom(in, variables[j - 1]->getName() + "[" + Util::toString(i) + "]");
                }
                variable->parseAndAddElementFrom(in);
            }
            parseNewLineFrom(in, variables.back()->getName() + "[" + Util::toString(i) + "]");
        }
    }

private:
    vector<string> names;
    vector<VerticalVariable*> variables;
    int size = -1;

    void setSize(int size) {
        this->size = size;
    }

    void checkVectorSizes() {
        for (VerticalVariable* variable : variables) {
            if (variable->size() != size) {
                throw PrintingException("Number of elements of vector `" + variable->getName() + "` unsatisfied. Expected: " + Util::toString(size) + ", actual: " + Util::toString(variable->size()));
            }
        }
    }
};

class MatrixSizes {
public:
    MatrixSizes(int rowsSize, int columnsSize)
            : rowsSize(rowsSize), columnsSize(columnsSize) { }

    int getRowsSize() {
        return rowsSize;
    }

    int getColumnsize() {
        return columnsSize;
    }

private:
    int rowsSize;
    int columnsSize;
};

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

class IOFormatProvider {
public:
    void beginParsingFormat(istream* in) {
        this->in = in;
        this->out = nullptr;
        this->lastSegment = nullptr;
    }

    void beginPrintingFormat(ostream* out) {
        this->in = nullptr;
        this->out = out;
        this->lastSegment = nullptr;
    }

    LineIOSegment& applyLineSegment(string names) {
        LineIOSegment* segment = new LineIOSegment(names);
        lastSegment = segment;
        return *segment;
    }

    LinesIOSegment& applyLinesSegment(string names) {
        LinesIOSegment* segment = new LinesIOSegment(names);
        lastSegment = segment;
        return *segment;
    }

    GridIOSegment& applyGridSegment(string name) {
        GridIOSegment* segment = new GridIOSegment(name);
        lastSegment = segment;
        return *segment;
    }

    void endParsingFormat() {
        applyLastSegment();

        if (in->get() != char_traits<char>::eof()) {
            throw ParsingException("Expected: <EOF>");
        }
    }

    void endPrintingFormat() {
        applyLastSegment();
    }

    void applyLastSegment() {
        if (lastSegment == nullptr) {
            return;
        }

        if (in != nullptr) {
            lastSegment->parseFrom(*in);
        } else {
            lastSegment->printTo(*out);
        }
    }

private:
    istream* in;
    ostream* out;
    IOSegment* lastSegment;
};

}

#endif
