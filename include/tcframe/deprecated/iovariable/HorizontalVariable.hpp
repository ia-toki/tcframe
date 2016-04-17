#pragma once

#include "tcframe/deprecated/iovariable/Variable.hpp"

#include <string>

using std::string;

namespace tcframe_old {

class HorizontalVariable : public Variable {
public:
    virtual ~HorizontalVariable() { }

protected:
    HorizontalVariable(string name)
            : Variable(name) { }
};

}
