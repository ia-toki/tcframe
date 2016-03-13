#pragma once

#include "tcframe/iovariable/Variable.hpp"

#include <string>

using std::string;

namespace tcframe {

class HorizontalVariable : public Variable {
public:
    virtual ~HorizontalVariable() { }

protected:
    HorizontalVariable(string name)
            : Variable(name) { }
};

}
