#ifndef TCFRAME_IO_H
#define TCFRAME_IO_H

#include "exception.hpp"
#include "type.hpp"

#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::make_pair;
using std::ostream;
using std::pair;
using std::string;
using std::vector;

namespace tcframe {

class IOSegment {
public:
    virtual ~IOSegment() { }

    virtual void printTo(ostream& out) = 0;
};

class LineIOSegment : public IOSegment {
public:
    LineIOSegment(string names) {
        this->names = Util::split(names);
    }

    template<typename T>
    LineIOSegment& operator,(T& variable) {
        addVariable(variable);
        return *this;
    }

    void printTo(ostream& out) override {
        bool first = true;
        for (auto variable : variables) {
            if (!first) {
                out << " ";
            }
            first = false;
            variable->printTo(out);
        }
        out << "\n";
    }

private:
    vector<string> names;
    vector<HorizontalVariable*> variables;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(T& x) {
        variables.push_back(new Scalar<T>(x, names[variables.size()]));
    }

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>& x) {
        variables.push_back(new HorizontalVector<T>(x, names[variables.size()]));
    }

    template<typename... T>
    void addVariable(T...) {
        throw IOFormatException("Line segment is only supported for basic scalar and vector of basic scalars types");
    }
};

class LinesIOSegment : public IOSegment {
public:
    LinesIOSegment(string names) {
        this->names = Util::split(names);
    }

    template<typename T>
    LinesIOSegment& operator,(T& variable) {
        addVariable(variable);
        return *this;
    }

    void printTo(ostream& out) override {
        if (variables.empty()) {
            throw IOFormatException("Lines segment must have at least one variable");
        }

        if (!isValidSegment()) {
            throw IOFormatException("All vectors participating in a lines segment must have equal sizes");
        }

        int linesSize = variables[0]->size();
        for (int i = 0; i < linesSize; i++) {
            bool first = true;
            for (auto variable : variables) {
                if (!first) {
                    out << " ";
                }
                first = false;
                variable->printElementTo(i, out);
            }
            out << "\n";
        }
    }

private:
    vector<string> names;
    vector<VerticalVariable*> variables;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>& x) {
        variables.push_back(new VerticalVector<T>(x, names[variables.size()]));
    }

    template<typename... T>
    void addVariable(T...) {
        throw IOFormatException("Lines segment is only supported for vector of basic scalars types");
    }

    bool isValidSegment() {
        int linesSize = variables[0]->size();
        for (int i = 1; i < variables.size(); i++) {
            if (variables[i]->size() != linesSize) {
                return false;
            }
        }

        return true;
    }
};

class GridIOSegment : public IOSegment {
public:
    GridIOSegment(string name)
            : name(name), variable(nullptr), hasSpaces(false) { }

    template<typename T>
    GridIOSegment& operator,(T& x) {
        if (variable != nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        setVariable(x, name);
        return *this;
    }

    void printTo(ostream& out) override {
        if (variable == nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        if (!isValidSegment()) {
            throw IOFormatException("Each row of the matrix in a grid segment must have equal number of columns");
        }

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

private:
    string name;
    MatrixVariable* variable;
    bool hasSpaces;

    template<typename T, typename = RequiresScalar<T>>
    void setVariable(vector<vector<T>>& x, string name) {
        variable = new Matrix<T>(x, name);
        hasSpaces = true;
    }

    void setVariable(vector<vector<char>>& x, string name) {
        variable = new Matrix<char>(x, name);
    }

    template<typename... T>
    void setVariable(T...) {
        throw IOFormatException("Grid segment is only supported for matrix of basic scalars types");
    }

    bool isValidSegment() {
        if (variable->rowsSize() == 0) {
            return true;
        }

        int columnsSize = variable->columnsSize(0);
        for (int i = 0; i < variable->rowsSize(); i++) {
            if (variable->columnsSize(i) != columnsSize) {
                return false;
            }
        }

        return true;
    }
};

class IOFormat {
public:
    void addSegment(IOSegment* segment) {
        segments.push_back(segment);
    }

    void printTo(ostream& out) {
        for (IOSegment* segment : segments) {
            segment->printTo(out);
        }
    }

private:
    vector<IOSegment*> segments;
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
    }

    void setMode(IOMode mode) {
        this->mode = mode;
    }

    LineIOSegment& addLineSegment(string names) {
        LineIOSegment* segment = new LineIOSegment(names);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    LinesIOSegment& addLinesSegment(string names) {
        LinesIOSegment* segment = new LinesIOSegment(names);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    GridIOSegment& addGridSegment(string name) {
        GridIOSegment* segment = new GridIOSegment(name);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    IOFormat* collectFormat(IOMode mode) {
        return formats[mode];
    }

private:
    IOFormat* formats[2];
    IOMode mode;
};

}

#endif
