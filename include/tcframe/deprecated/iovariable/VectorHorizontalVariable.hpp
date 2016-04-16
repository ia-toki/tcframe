#pragma once

#include "tcframe/deprecated/iovariable/HorizontalVariable.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class VectorHorizontalVariable : public HorizontalVariable {
public:
    virtual ~VectorHorizontalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
    virtual void parseAndAddElementFrom(istream& in) = 0;

protected:
    VectorHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

}
