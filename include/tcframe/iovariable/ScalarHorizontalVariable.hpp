#pragma once

#include "tcframe/iovariable/HorizontalVariable.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class ScalarHorizontalVariable : public HorizontalVariable {
public:
    virtual ~ScalarHorizontalVariable() { }

    virtual void printTo(ostream& out) = 0;
    virtual void parseFrom(istream& in) = 0;

protected:
    ScalarHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

}
