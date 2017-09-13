#pragma once

#include <iostream>
#include <stdexcept>

#include "LinesIOSegment.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class LinesIOSegmentManipulator {
public:
    string parse(LinesIOSegment* segment, istream* in) {
        for (Variable* variable : segment->variables()) {
            if (variable->type() == VariableType::VECTOR) {
                ((Vector*) variable)->clear();
            } else {
                ((Matrix*) variable)->clear();
            }
        }

        string lastVariableName;

        int size = segment->size()();
        for (int j = 0; j != size; j++) {
            if (size == NO_SIZE && WhitespaceManipulator::isEof(in)) {
                break;
            }

            bool isFirstColumn = true;
            for (Variable* variable : segment->variables()) {
                if (variable->type() == VariableType::VECTOR) {
                    if (!isFirstColumn) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                    ((Vector*) variable)->parseAndAddElementFrom(in);
                    lastVariableName = TokenFormatter::formatVectorElement(variable->name(), j);
                } else {
                    if (!isFirstColumn && !WhitespaceManipulator::canParseNewline(in)) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                    Matrix* matrixVariable = (Matrix*) variable;
                    matrixVariable->parseAndAddRowFrom(in, j);
                    lastVariableName = TokenFormatter::formatMatrixElement(
                            variable->name(),
                            j,
                            matrixVariable->columns(j) - 1);
                }
                isFirstColumn = false;
            }
            WhitespaceManipulator::parseNewline(in, lastVariableName);
        }

        return lastVariableName;
    }

    void print(LinesIOSegment* segment, ostream* out) {
        checkVectorSizes(segment);

        int size = getSize(segment);
        for (int j = 0; j < size; j++) {
            for (int i = 0; i < segment->variables().size(); i++) {
                Variable *variable = segment->variables()[i];
                if (variable->type() == VariableType::VECTOR) {
                    if (i > 0) {
                        *out << ' ';
                    }
                    ((Vector*) variable)->printElementTo(j, out);
                } else {
                    if (i > 0 && ((Matrix*) variable)->columns(j) > 0) {
                        *out << ' ';
                    }
                    ((Matrix*) variable)->printRowTo(j, out);
                }
            }
            *out << endl;
        }
    }

private:
    static int getSize(LinesIOSegment* segment) {
        int size = segment->size()();
        if (size != NO_SIZE) {
            return size;
        }

        Variable* firstVariable = segment->variables()[0];
        if (firstVariable->type() == VariableType::VECTOR) {
            return ((Vector*) firstVariable)->size();
        } else {
            return ((Matrix*) firstVariable)->rows();
        }
    }

    static void checkVectorSizes(LinesIOSegment* segment) {
        int expectedSize = getSize(segment);

        for (Variable* variable : segment->variables()) {
            int size;
            string type;
            if (variable->type() == VariableType::VECTOR) {
                size = ((Vector*) variable)->size();
                type = "vector";
            } else {
                size = ((Matrix*) variable)->rows();
                type = "jagged vector";
            }
            if (size != expectedSize) {
                string withoutSizeMessage;
                if (segment->size()() == NO_SIZE) {
                    string firstVariableName = TokenFormatter::formatVariable(segment->variables()[0]->name());
                    withoutSizeMessage = " (number of elements of " + firstVariableName + ")";
                }
                throw runtime_error(
                        "Number of elements of " + type + " " + TokenFormatter::formatVariable(variable->name())
                      + " unsatisfied. Expected: " + StringUtils::toString(expectedSize) + withoutSizeMessage
                      + ", actual: " + StringUtils::toString(size));
            }
        }
    }
};

}
