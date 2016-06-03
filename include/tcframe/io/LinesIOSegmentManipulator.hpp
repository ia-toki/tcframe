#pragma once

#include <iostream>
#include <stdexcept>

#include "LinesIOSegment.hpp"
#include "tcframe/logger.hpp"
#include "tcframe/util.hpp"
#include "tcframe/variable.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class LinesIOSegmentManipulator {
public:
    static void parse(LinesIOSegment* segment, istream* in) {
        for (Variable* variable : segment->variables()) {
            if (variable->type() == VariableType::VECTOR) {
                ((Vector*) variable)->clear();
            } else {
                ((Matrix*) variable)->clear();
            }
        }

        for (int j = 0; j < segment->size(); j++) {
            string lastVariableName;
            for (Variable* variable : segment->variables()) {
                if (variable->type() == VariableType::VECTOR) {
                    if (!lastVariableName.empty()) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                    ((Vector*) variable)->parseAndAddElementFrom(in);
                } else {
                    if (!lastVariableName.empty() && !WhitespaceManipulator::canParseNewline(in)) {
                        WhitespaceManipulator::parseSpace(in, lastVariableName);
                    }
                    ((Matrix*) variable)->parseAndAddRowFrom(in, j);
                }
                lastVariableName = TokenFormatter::formatVectorElement(variable->name(), j);
            }
            WhitespaceManipulator::parseNewline(in, lastVariableName);
        }
    }

    static void print(LinesIOSegment* segment, ostream* out) {
        checkVectorSizes(segment);

        for (int j = 0; j < segment->size(); j++) {
            for (int i = 0; i < segment->variables().size(); i++) {
                Variable *variable = segment->variables()[i];
                if (variable->type() == VariableType::VECTOR) {
                    if (i > 0) {
                        *out << ' ';
                    }
                    ((Vector*) variable)->printElementTo(j, out);
                } else {
                    if (((Matrix*) variable)->columns(j) > 0) {
                        *out << ' ';
                    }
                    ((Matrix*) variable)->printRowTo(j, out);
                }
            }
            *out << endl;
        }
    }

private:
    static void checkVectorSizes(LinesIOSegment* segment) {
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
            if (size != segment->size()) {
                throw runtime_error(
                        "Number of elements of " + type + " " + TokenFormatter::formatVariable(variable->name())
                      + " unsatisfied. Expected: " + StringUtils::toString(segment->size())
                      + ", actual: " + StringUtils::toString(size));
            }
        }
    }
};

}
