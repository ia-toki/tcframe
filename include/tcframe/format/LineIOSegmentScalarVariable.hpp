#pragma once

#include <tuple>

#include "LineIOSegmentVariable.hpp"
#include "tcframe/type.hpp"

using std::tie;

namespace tcframe {

struct LineIOSegmentScalarVariable : public LineIOSegmentVariable {
private:
    Scalar* variable_;

public:
    LineIOSegmentScalarVariable(Scalar* variable)
            : variable_(variable)
    {}

    LineIOSegmentVariableType type() const {
        return LineIOSegmentVariableType::SCALAR;
    }

    Scalar* variable() const {
        return variable_;
    }

    bool operator==(const LineIOSegmentScalarVariable& o) const {
        return tie(*variable_) == tie(*o.variable_);
    }

protected:
    bool equals(const LineIOSegmentVariable& o) const {
        return *this == (LineIOSegmentScalarVariable&) o;
    }
};

}
