#pragma once

#include "tcframe/iovariable/Variable.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class VerticalVariable : public Variable {
public:
    virtual ~VerticalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
    virtual void parseAndAddElementFrom(istream& in) = 0;

protected:
    VerticalVariable(string name)
            : Variable(name) { }
};

}
