#ifndef TCFRAME_IO_H
#define TCFRAME_IO_H

#include "exception.hpp"
#include "type.hpp"

#include <ostream>
#include <string>
#include <vector>

using std::ostream;
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
    LineIOSegment(string description)
            : description(description) { }

    template<typename T>
    LineIOSegment& operator,(T& x) {
        addVariable(x);
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
    string description;
    vector<HorizontalVariable*> variables;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(T& x) {
        variables.push_back(new Scalar<T>(x));
    }

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>& x) {
        variables.push_back(new HorizontalVector<T>(x));
    }

    template<typename... T>
    void addVariable(T...) {
        throw IOFormatException("Line segment is only supported for basic scalar and vector of basic scalars types");
    }
};

class LinesIOSegment : public IOSegment {
public:
    LinesIOSegment(string description)
            : description(description) { }

    template<typename T>
    LinesIOSegment& operator,(T& x) {
        addVariable(x);
        return *this;
    }

    void printTo(ostream& out) override {
        if (variables.empty()) {
            throw IOFormatException("Lines segment must have at least one variable");
        }

        int linesCount = variables[0]->size();

        for (int i = 1; i < variables.size(); i++) {
            if (variables[i]->size() != linesCount) {
                throw IOFormatException("All vectors participating in a lines segment must have equal sizes");
            }
        }

        for (int i = 0; i < linesCount; i++) {
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
    string description;
    vector<VerticalVariable*> variables;

    template<typename T, typename = RequiresScalar<T>>
    void addVariable(vector<T>& x) {
        variables.push_back(new VerticalVector<T>(x));
    }

    template<typename... T>
    void addVariable(T...) {
        throw IOFormatException("Lines segment is only supported for vector of basic scalars types");
    }
};

class GridIOSegment : public IOSegment {
public:
    GridIOSegment(string description)
            : description(description), variable(nullptr) { }

    template<typename T>
    GridIOSegment& operator,(T& x) {
        if (variable != nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        setVariable(x);
        return *this;
    }

    void printTo(ostream& out) override {
        if (variable == nullptr) {
            throw IOFormatException("Grid segment must have exactly one variable");
        }

        variable->printTo(out);
    }

private:
    string description;
    MatrixVariable* variable;

    template<typename T, typename = RequiresScalar<T>>
    void setVariable(vector<vector<T>>& x) {
        variable = new Matrix<T>(x);
    }

    template<typename... T>
    void setVariable(T...) {
        throw IOFormatException("Grid segment is only supported for matrix of basic scalars types");
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

    LineIOSegment& addLineSegment(string description) {
        LineIOSegment* segment = new LineIOSegment(description);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    LinesIOSegment& addLinesSegment(string description) {
        LinesIOSegment* segment = new LinesIOSegment(description);
        formats[mode]->addSegment(segment);
        return *segment;
    }

    GridIOSegment& addGridSegment(string description) {
        GridIOSegment* segment = new GridIOSegment(description);
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
