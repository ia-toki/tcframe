#pragma once

#include <iostream>
#include <stdexcept>

#include "tcframe/logger.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class LineIOSegmentManipulator {
public:
    static string parse(LineIOSegment* segment, istream* in) {
        string lastVariableName;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!lastVariableName.empty()) {
                WhitespaceManipulator::parseSpace(in, lastVariableName);
            }

            Variable* variable = segmentVariable.variable();
            int size = *segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                parseScalar((Scalar*) variable, in);
                lastVariableName = TokenFormatter::formatVariable(variable->name());
            } else {
                parseVector((Vector*) variable, size, in);
                lastVariableName = TokenFormatter::formatVectorElement(variable->name(), size - 1);
            }
        }
        WhitespaceManipulator::parseNewline(in, lastVariableName);

        return lastVariableName;
    }

    static void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!first) {
                *out << ' ';
            }
            first = false;

            Variable* variable = segmentVariable.variable();
            int size = *segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                printScalar((Scalar*) variable, out);
            } else if (variable->type() == VariableType::VECTOR) {
                printVector((Vector*) variable, size, out);
            }
        }
        *out << endl;
    }

private:
    static void checkVectorSize(Vector* vektor, int size) {
        if (size != -1 && vektor->size() != size) {
            throw runtime_error("Number of elements of vector " + TokenFormatter::formatVariable(vektor->name())
                                + " unsatisfied. Expected: " + StringUtils::toString(size) + ", actual: "
                                + StringUtils::toString(vektor->size()));
        }
    }

    static void parseScalar(Scalar* scalar, istream* in) {
        scalar->parseFrom(in);
    }

    static void parseVector(Vector* vektor, int size, istream* in) {
        vektor->clear();
        if (size == -1) {
            vektor->parseFrom(in);
        } else {
            vektor->parseFrom(in, size);
        }
    }

    static void printScalar(Scalar* scalar, ostream* out) {
        scalar->printTo(out);
    }

    static void printVector(Vector* vektor, int size, ostream* out) {
        checkVectorSize(vektor, size);
        vektor->printTo(out);
    }
};

}
