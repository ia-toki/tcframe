#pragma once

#include "tcframe/deprecated/core/Util.hpp"
#include "tcframe/deprecated/iovariable/HorizontalVariable.hpp"
#include "tcframe/deprecated/iovariable/HorizontalVector.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"
#include "tcframe/deprecated/iovariable/Scalar.hpp"
#include "tcframe/deprecated/iovariable/ScalarHorizontalVariable.hpp"
#include "tcframe/deprecated/iovariable/VectorHorizontalVariable.hpp"
#include "tcframe/deprecated/ioformat/IOFormatException.hpp"
#include "tcframe/deprecated/ioformat/IOSegment.hpp"
#include "tcframe/deprecated/ioformat/VectorSize.hpp"
#include "tcframe/deprecated/ioformat/VectorWithSize.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe_old {

class LineIOSegmentOld : public IOSegment {
public:
    LineIOSegmentOld(string names) {
        this->names = Util::split(names);
    }

    template<typename T, typename = RequiresScalar<T>>
    LineIOSegmentOld& operator,(T& scalar) {
        checkVectorWithoutSize();

        variables.push_back(new Scalar<T>(scalar, names[variables.size()]));
        vectorSizes.push_back(0);
        return *this;
    }

    template<typename T, typename = RequiresScalar<T>>
    LineIOSegmentOld& operator,(vector<T>& vektor) {
        checkVectorWithoutSize();

        variables.push_back(new HorizontalVector<T>(vektor, names[variables.size()]));
        vectorSizes.push_back(-1);
        return *this;
    }

    template<typename T, typename = RequiresScalar<T>>
    LineIOSegmentOld& operator,(VectorWithSize<T> vektorWithSize) {
        checkVectorWithoutSize();

        variables.push_back(new HorizontalVector<T>(*vektorWithSize.vektor, names[variables.size()]));
        vectorSizes.push_back(vektorWithSize.size.getSize());
        return *this;
    }

    template<typename T, typename = RequiresNotScalar <T>>
    LineIOSegmentOld& operator,(T) {
        throw IOFormatException("Variable type of `" + names[variables.size()] +
                                "` unsatisfied. Expected: basic scalar or string type, or vector of those types");
    }

    void printTo(ostream& out) {
        bool first = true;
        for (int i = 0; i < variables.size(); i++) {
            HorizontalVariable* variable = variables[i];

            if (!first) {
                out << " ";
            }
            first = false;

            if (dynamic_cast<ScalarHorizontalVariable*>(variable) != nullptr) {
                printScalarTo((ScalarHorizontalVariable*) variable, out);
            } else {
                printVectorTo((VectorHorizontalVariable*) variable, vectorSizes[i], out);
            }
        }
        out << "\n";
    }

    void parseFrom(istream& in) {
        for (int i = 0; i < variables.size(); i++) {
            HorizontalVariable* variable = variables[i];

            if (i > 0) {
                parseSpaceFrom(in, variables[i - 1]->getName());
            }

            if (dynamic_cast<ScalarHorizontalVariable*>(variable) != nullptr) {
                parseScalarFrom((ScalarHorizontalVariable*) variable, in);
            } else {
                parseVectorFrom((VectorHorizontalVariable*) variable, vectorSizes[i], in);
            }
        }

        if (variables.empty()) {
            parseNewLineFrom(in, "");
        } else {
            parseNewLineFrom(in, variables.back()->getName());
        }
    }

private:
    vector<string> names;
    vector<HorizontalVariable*> variables;
    vector<int> vectorSizes;

    void checkVectorWithoutSize() {
        if (!vectorSizes.empty() && vectorSizes.back() == -1) {
            throw IOFormatException("Vector without size can only be the last variable in a line segment");
        }
    }

    void printScalarTo(ScalarHorizontalVariable* scalar, ostream& out) {
        scalar->printTo(out);
    }

    void checkVectorSize(VectorHorizontalVariable* vektor, int size) {
        if (size != -1 && vektor->size() != size) {
            throw PrintingException("Number of elements of vector `" + vektor->getName() + "` unsatisfied. Expected: " +
                                    Util::toString(size) + ", actual: " + Util::toString(vektor->size()));
        }
    }

    void printVectorTo(VectorHorizontalVariable* vektor, int size, ostream& out) {
        checkVectorSize(vektor, size);

        bool first = true;
        for (int i = 0; i < vektor->size(); i++) {
            if (!first) {
                out << " ";
            }
            first = false;

            vektor->printElementTo(i, out);
        }
    }

    void parseScalarFrom(ScalarHorizontalVariable* scalar, istream& in) {
        scalar->parseFrom(in);
    }

    void parseVectorWithSizeFrom(VectorHorizontalVariable* vektor, int size, istream& in) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                parseSpaceFrom(in, vektor->getName() + "[" + Util::toString(i - 1) + "]");
            }
            vektor->parseAndAddElementFrom(in);
        }
    }

    void parseVectorWithoutSizeFrom(VectorHorizontalVariable* vektor, istream& in) {
        for (int i = 0; ; i++) {
            int c = in.peek();

            if (i == 0) {
                if (c == '\n') {
                    break;
                }
            } else {
                if (c == '\n') {
                    break;
                } else if (c == ' ') {
                    in.get();
                } else {
                    throw ParsingException("Expected: <space> or <new line> after variable `" + vektor->getName() + "[" +
                            Util::toString(i - 1) + "]`");
                }
            }

            vektor->parseAndAddElementFrom(in);
        }
    }

    void parseVectorFrom(VectorHorizontalVariable* vektor, int size, istream& in) {
        vektor->clear();

        if (size == -1) {
            parseVectorWithoutSizeFrom(vektor, in);
        } else {
            parseVectorWithSizeFrom(vektor, size, in);
        }
    }
};

}
