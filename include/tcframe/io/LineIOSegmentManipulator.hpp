#pragma once

#include <iostream>

#include "LineIOSegment.hpp"
#include "WhitespaceManipulator.hpp"
#include "tcframe/variable.hpp"

using std::istream;
using std::ostream;

namespace tcframe {

class LineIOSegmentManipulator {
private:
    WhitespaceManipulator* whitespaceManipulator_;

public:
    virtual ~LineIOSegmentManipulator() {}

    LineIOSegmentManipulator(WhitespaceManipulator* whitespaceManipulator)
            : whitespaceManipulator_(whitespaceManipulator)
    {}

    virtual void parse(LineIOSegment* segment, istream* in) {
        string lastVariableName;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!lastVariableName.empty()) {
                whitespaceManipulator_->parseSpace(in, lastVariableName);
            }

            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                parseScalar((Scalar*) variable, in);
            }

            lastVariableName = variable->name();
        }
        whitespaceManipulator_->parseNewline(in, lastVariableName);
    }

    virtual void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!first) {
                whitespaceManipulator_->printSpace(out);
            }
            first = false;

            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                printScalar((Scalar*) variable, out);
            }
        }
        whitespaceManipulator_->printNewline(out);
    }

private:
    void parseScalar(Scalar* scalar, istream* in) {
        scalar->parseFrom(in);
    }

    void printScalar(Scalar* scalar, ostream* out) {
        scalar->printTo(out);
    }
};

}
