#ifndef TCFRAME_IO_H
#define TCFRAME_IO_H

#include "type.h"

#include <exception>
#include <ostream>
#include <vector>

#include <iostream>

using std::ostream;
using std::runtime_error;
using std::vector;

namespace tcframe {

class IOSegment {
public:
    virtual void printTo(ostream& out) = 0;
};

class LineIOSegment : public IOSegment {
private:
    vector<Variable*> variables;

public:
    LineIOSegment& operator%(int& x) {
        if (!variables.empty()) {
            throw runtime_error("Invalid syntax: use ',` here");
        }

        variables.push_back(new Scalar<int>(x));
        return *this;
    }

    LineIOSegment& operator,(int& x) {
        if (variables.empty()) {
            throw runtime_error("Invalid syntax: use '\%` here");
        }

        variables.push_back(new Scalar<int>(x));
        return *this;
    }

    void printTo(ostream& out) {
        bool first = true;
        for (auto variable : variables) {
            if (!first)
                out << " ";
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

protected:
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
