#pragma once

#include <iostream>
#include <stdexcept>

#include "LineIOSegment.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class LineIOSegmentManipulator {
public:
    string parse(LineIOSegment* segment, istream* in) {
        string lastVariableName;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size()();

            if (variable->type() == VariableType::SCALAR) {
                if (!lastVariableName.empty()) {
                    WhitespaceManipulator::parseSpace(in, lastVariableName);
                }
                parseScalar((Scalar*) variable, in);
                lastVariableName = TokenFormatter::formatVariable(variable->name());
            } else {
                if (!lastVariableName.empty()) {
                    if (size != NO_SIZE && size > 0) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                    if (size == NO_SIZE && !WhitespaceManipulator::canParseNewline(in)) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                }
                Vector* vectorVariable = (Vector*) variable;
                parseVector(vectorVariable, size, in);
                lastVariableName = TokenFormatter::formatVectorElement(variable->name(), vectorVariable->size() - 1);
            }
        }
        WhitespaceManipulator::parseNewline(in, lastVariableName);

        return lastVariableName;
    }

    void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size()();

            if (variable->type() == VariableType::SCALAR) {
                if (!first) {
                    *out << ' ';
                }
                printScalar((Scalar*) variable, out);
            } else if (variable->type() == VariableType::VECTOR) {
                if (!first && ((Vector*) variable)->size() != 0) {
                    *out << ' ';
                }
                printVector((Vector*) variable, size, out);
            }
            first = false;
        }
        *out << endl;
    }

private:
    static void checkVectorSize(Vector* vektor, int size) {
        if (size != NO_SIZE && vektor->size() != size) {
            throw runtime_error(
                    "Number of elements of vector " + TokenFormatter::formatVariable(vektor->name())
                    + " unsatisfied. Expected: " + StringUtils::toString(size) + ", actual: "
                    + StringUtils::toString(vektor->size()));
        }
    }

    static void parseScalar(Scalar* scalar, istream* in) {
        scalar->parseFrom(in);
    }

    static void parseVector(Vector* vektor, int size, istream* in) {
        vektor->clear();
        if (size == NO_SIZE) {
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
