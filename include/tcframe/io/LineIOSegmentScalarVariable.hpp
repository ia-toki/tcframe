#pragma once

#include <tuple>

#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/type/Scalar.hpp"

using std::tie;

namespace tcframe {

struct LineIOSegmentScalarVariable : public LineIOSegmentVariable {
private:
    Scalar* variable_;

public:
    LineIOSegmentScalarVariable(Scalar* variable)
            : variable_(variable)
    {}

    LineIOSegmentVariableType type() {
        return LineIOSegmentVariableType::SCALAR;
    }

    Scalar* variable() const {
        return variable_;
    }

    bool operator==(const LineIOSegmentScalarVariable& o) const {
        return tie(*variable_) == tie(*o.variable_);
    }
};

}
