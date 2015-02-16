#ifndef TCFRAME_IO_H
#define TCFRAME_IO_H

#include "exception.h"
#include "type.h"

#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

namespace tcframe {

class IOSegment {
public:
    virtual void printTo(ostream& out) = 0;
};

class LineIOSegment : public IOSegment {
private:
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

public:
    template<typename T>
    LineIOSegment& operator%(T& x) {
        if (!variables.empty()) {
            throw IOFormatException("Invalid syntax: use ',` here");
        }

        addVariable(x);
        return *this;
    }

    template<typename T>
    LineIOSegment& operator,(T& x) {
        if (variables.empty()) {
            throw IOFormatException("Invalid syntax: use '%` here");
        }

        addVariable(x);
        return *this;
    }

    void printTo(ostream& out) {
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
};

class IOFormat {
private:
    vector<IOSegment*> segments;

public:
    void addSegment(IOSegment* segment) {
        segments.push_back(segment);
    }

    void printTo(ostream& out) {
        for (IOSegment* segment : segments) {
            segment->printTo(out);
        }
    }
};

enum IOMode {
    INPUT,
    OUTPUT
};

class IOFormatsCollector {
private:
    IOFormat* formats[2];
    IOMode mode;

public:
    IOFormatsCollector() : mode(IOMode::INPUT) {
        this->formats[IOMode::INPUT] = new IOFormat();
        this->formats[IOMode::OUTPUT] = new IOFormat();
    }

    void setMode(IOMode mode) {
        this->mode = mode;
    }

    LineIOSegment& line() {
        LineIOSegment* segment = new LineIOSegment();
        formats[mode]->addSegment(segment);
        return *segment;
    }

    IOFormat* collectFormat(IOMode mode) {
        return formats[mode];
    }
};

}

#endif
