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
    virtual void checkState() = 0;

protected:
    IOSegment(FailuresCollector* formatFailuresCollector)
            : formatFailuresCollector(formatFailuresCollector) { }

    void addFormatFailure(string message) {
        formatFailuresCollector->addFailure(Failure(message, 0));
    }

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

private:
    FailuresCollector* formatFailuresCollector;
};

class VectorSize {
public:
    VectorSize(int& size)
            : size(&size) { }

    VectorSize(int&& size)
            : size(new int(size)) { }

    int* getSize() {
        return size;
    }

private:
    int* size;
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
    LineIOSegment(string names, FailuresCollector* formatFailuresCollector)
            : IOSegment(formatFailuresCollector) {
        this->names = Util::split(names);
    }

    template<typename T>
    LineIOSegment& operator,(T& variable) {
        addVariable(variable);
        return *this;
    }

    template<typename T>
    LineIOSegment& operator,(T&& variable) {
        addVariable(variable);
        return *this;
    }

    void printTo(ostream& out) override {
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
                printVectorTo((VectorHorizontalVariable*)variable, *vectorSizes[i], out);
            }
        }
        out << "\n";
    }

    void parseFrom(istream& in) override {
        for (int i = 0; i < variables.size(); i++) {
            HorizontalVariable* variable = variables[i];

            if (i > 0) {
                parseSpaceFrom(in, variables[i - 1]->getName());
            }

            if (dynamic_cast<ScalarHorizontalVariable*>(variable) != nullptr) {
                parseScalarFrom((ScalarHorizontalVariable*)variable, in);
            } else {
                parseVectorFrom((VectorHorizontalVariable*)variable, *vectorSizes[i], in);
            }
        }

        if (variables.empty()) {
            parseNewLineFrom(in, "");
        } else {
            parseNewLineFrom(in, variables.back()->getName());
        }
    }

    void checkState() override {}

private:
    vector<string> names;
    vector<HorizontalVariable*> variables;
    vector<int*> vectorSizes;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(T& scalar) {
        variables.push_back(new Scalar<T>(scalar, names[variables.size()]));
        vectorSizes.push_back(nullptr);
    }

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(T&& scalar) {
        T* newScalar = new T(scalar);
        variables.push_back(new Scalar<T>(*newScalar, names[variables.size()]));
        vectorSizes.push_back(nullptr);
    }

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(VectorWithSize<T> vektorWithSize) {
        variables.push_back(new HorizontalVector<T>(*vektorWithSize.vektor, names[variables.size()]));
        vectorSizes.push_back(vektorWithSize.size.getSize());
    }

    template<typename... T>
    void addVariable(T...) {
        addFormatFailure("Variable type of `" + names[variables.size()] + "` unsatisfied. Expected: basic scalar or string type, or vector of those types");
        variables.push_back(nullptr);
    }

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
    LinesIOSegment(string names, FailuresCollector* formatFailuresCollector)
            : IOSegment(formatFailuresCollector), size(nullptr) {
        this->names = Util::split(names);
    }

    template<typename T>
    LinesIOSegment& operator,(T& variable) {
        addVariable(variable);
        return *this;
    }

    template<typename T>
    LinesIOSegment& operator,(T&& variable) {
        addVariable(variable);
        return *this;
    }

    LinesIOSegment& operator%(VectorSize vektorSize) {
        setSize(vektorSize.getSize());
        return *this;
    }

    void printTo(ostream& out) override {
        checkVectorSizes();

        for (int i = 0; i < *size; i++) {
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

    void parseFrom(istream& in) override {
        for (VerticalVariable* variable : variables) {
            variable->clear();
        }

        for (int i = 0; i < *size; i++) {
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

    void checkState() override {
        if (variables.empty()) {
            addFormatFailure("Lines segment must have at least one variable");
        }

        if (size == nullptr) {
            addFormatFailure("Lines segment must define vector sizes");
        }
    }

private:
    vector<string> names;
    vector<VerticalVariable*> variables;
    int* size;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>& vektor) {
        variables.push_back(new VerticalVector<T>(vektor, names[variables.size()]));
    }

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>&& vektor) {
        vector<T>* newVektor = new vector<T>(vektor);
        variables.push_back(new VerticalVector<T>(*newVektor, names[variables.size()]));
    }

    template<typename... T>
    void addVariable(T...) {
        addFormatFailure("Variable type of `" + names[variables.size()] + "` unsatisfied. Expected: vector of basic scalar or string type");
        variables.push_back(nullptr);
    }

    void setSize(int* size) {
        this->size = size;
    }

    void checkVectorSizes() {
        for (VerticalVariable* variable : variables) {
            if (variable->size() != *size) {
                throw PrintingException("Number of elements of vector `" + variable->getName() + "` unsatisfied. Expected: " + Util::toString(*size) + ", actual: " + Util::toString(variable->size()));
            }
        }
    }
};

class MatrixSizes {
public:
    MatrixSizes(int& rowsSize, int& columnsSize)
            : rowsSize(&rowsSize), columnsSize(&columnsSize) { }

    MatrixSizes(int&& rowsSize, int& columnsSize)
            : rowsSize(new int(rowsSize)), columnsSize(&columnsSize) { }

    MatrixSizes(int& rowsSize, int&& columnsSize)
            : rowsSize(&rowsSize), columnsSize(new int(columnsSize)) { }

    MatrixSizes(int&& rowsSize, int&& columnsSize)
            : rowsSize(new int(rowsSize)), columnsSize(new int(columnsSize)) { }

    int* getRowsSize() {
        return rowsSize;
    }

    int* getColumnsize() {
        return columnsSize;
    }

private:
    int* rowsSize;
    int* columnsSize;
};

class GridIOSegment : public IOSegment {
public:
    GridIOSegment(string name, FailuresCollector* formatFailuresCollector)
            : IOSegment(formatFailuresCollector), name(name), variable(nullptr), hasSpaces(false), rowsSize(nullptr), columnsSize(nullptr) { }

    template<typename T>
    GridIOSegment& operator,(T& variable) {
        setVariable(variable, name);
        return *this;
    }

    template<typename T>
    GridIOSegment& operator,(T&& variable) {
        setVariable(variable, name);
        return *this;
    }

    GridIOSegment& operator%(MatrixSizes sizes) {
        rowsSize = sizes.getRowsSize();
        columnsSize = sizes.getColumnsize();
        return *this;
    }

    void printTo(ostream& out) override {
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

    void parseFrom(istream& in) override {
        variable->clear();

        for (int i = 0; i < *rowsSize; i++) {
            variable->addRow();
            for (int j = 0; j < *columnsSize; j++) {
                if (j > 0 && hasSpaces) {
                    parseSpaceFrom(in, variable->getName() + "[" + Util::toString(i) + "][" + Util::toString(j - 1) + "]");
                }
                variable->parseAndAddColumnElementFrom(in);
            }
            parseNewLineFrom(in, variable->getName() + "[" + Util::toString(i) + "][" + Util::toString(*rowsSize - 1) + "]");
        }
    }

    void checkState() override {
        if (variable == nullptr) {
            addFormatFailure("Grid segment must have exactly one variable");
        }

        if (rowsSize == nullptr || columnsSize == nullptr) {
            addFormatFailure("Grid segment must define matrix sizes");
        }
    }

private:
    string name;
    MatrixVariable* variable;
    bool hasSpaces;
    int* rowsSize;
    int* columnsSize;

    template<typename T, typename = RequiresScalar<T>>
    void setVariable(vector<vector<T>>& matrix, string name) {
        if (variable != nullptr) {
            addFormatFailure("Grid segment must have exactly one variable");
        }

        variable = new Matrix<T>(matrix, name);
        hasSpaces = !is_same<T, char>::value;
    }

    template<typename T, typename = RequiresScalar<T>>
    void setVariable(vector<vector<T>>&& matrix, string name) {
        if (variable != nullptr) {
            addFormatFailure("Grid segment must have exactly one variable");
        }

        vector<vector<T>>* newMatrix = new vector<vector<T>>(matrix);

        variable = new Matrix<T>(*newMatrix, name);
        hasSpaces = !is_same<T, char>::value;
    }

    template<typename... T>
    void setVariable(T...) {
        addFormatFailure("Variable type of `" + name + "` unsatisfied. Expected: matrix of basic scalar or string type");
    }

    void checkMatrixSizes() {
        if (variable->rowsSize() != *rowsSize) {
            throw PrintingException("Number of rows of matrix `" + variable->getName() + "` unsatisfied. Expected: " + Util::toString(*rowsSize) + ", actual: " + Util::toString(variable->rowsSize()));
        }

        for (int i = 0; i < *rowsSize; i++) {
            if (variable->columnsSize(i) != *columnsSize) {
                throw PrintingException("Number of columns row " + Util::toString(i) + " of matrix `" + variable->getName() + "` (0-based) unsatisfied. Expected: " + Util::toString(*columnsSize) + ", actual: " + Util::toString(variable->columnsSize(i)));
            }
        }
    }
};

class IOFormat {
public:
    void addSegment(IOSegment* segment) {
        segments.push_back(segment);
    }

    vector<IOSegment*> getSegments() {
        return segments;
    }

    void printTo(ostream& out) {
        for (IOSegment* segment : segments) {
            segment->printTo(out);
        }
    }

    void parseFrom(istream& in) {
        for (IOSegment* segment : segments) {
            segment->parseFrom(in);
        }
        parseEofFrom(in);
    }

private:
    vector<IOSegment*> segments;

    static void parseEofFrom(istream& in) {
        if (in.get() != char_traits<char>::eof()) {
            throw ParsingException("Expected: <EOF>");
        }
    }
};

enum IOMode {
    INPUT,
    OUTPUT
};

class IOFormatsCollector {
public:
    IOFormatsCollector() : mode(IOMode::INPUT) {
        this->formats[IOMode::INPUT] = new IOFormat();
        this->formats[IOMode::OUTPUT] = new IOFormat();

        this->formatFailuresCollectors[IOMode::INPUT] = new FailuresCollector();
        this->formatFailuresCollectors[IOMode::OUTPUT] = new FailuresCollector();
    }

    void setMode(IOMode mode) {
        this->mode = mode;
    }

    LineIOSegment& addLineSegment(string names) {
        LineIOSegment* segment = new LineIOSegment(names, formatFailuresCollectors[mode]);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    LinesIOSegment& addLinesSegment(string names) {
        LinesIOSegment* segment = new LinesIOSegment(names, formatFailuresCollectors[mode]);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    GridIOSegment& addGridSegment(string name) {
        GridIOSegment* segment = new GridIOSegment(name, formatFailuresCollectors[mode]);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    IOFormat* collectFormat(IOMode mode) {
        IOFormat* format = formats[mode];

        for (IOSegment* segment : format->getSegments()) {
            segment->checkState();
        }

        vector<Failure> failures = formatFailuresCollectors[mode]->collectFailures();

        if (failures.empty()) {
            return format;
        } else {
            throw IOFormatException(failures);
        }
    }

private:
    IOFormat* formats[2];
    FailuresCollector* formatFailuresCollectors[2];
    IOMode mode;
};

}

#endif
