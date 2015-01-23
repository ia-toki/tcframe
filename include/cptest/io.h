#ifndef CPTEST_IO_H
#define CPTEST_IO_H

#include "type.h"

#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

namespace org { namespace iatoki { namespace cptest {

class IOSegment {
public:
    virtual void printTo(ostream& out) = 0;
};

class LineIOSegment : public IOSegment {
private:
    vector<Variable*> variables;

public:
    LineIOSegment& operator%(int& x) {
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

}}}

#endif