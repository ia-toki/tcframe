#pragma once

#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/type/Scalar.hpp"

namespace tcframe { namespace experimental {

struct LineIOSegmentScalarVariable : public LineIOSegmentVariable {
private:
    Scalar* variable_;

public:
    LineIOSegmentScalarVariable(Scalar* variable)
            : variable_(variable)
    {}

    Scalar* variable() const {
        return variable_;
    }
};

}}
