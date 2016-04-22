#pragma once

#include <tuple>
#include <vector>

#include "IOSegment.hpp"
#include "LineIOSegmentScalarVariable.hpp"
#include "LineIOSegmentVariable.hpp"
#include "tcframe/type.hpp"
#include "tcframe/util.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct LineIOSegment : public IOSegment {
    friend class LineIOSegmentBuilder;

private:
    vector<LineIOSegmentVariable*> variables_;

public:
    IOSegmentType type() {
        return IOSegmentType::LINE;
    }

    const vector<LineIOSegmentVariable*>& variables() const {
        return variables_;
    }

    bool operator==(const LineIOSegment& o) const {
        return tie(variables_) == tie(o.variables_);
    }
};

class LineIOSegmentBuilder {
private:
    LineIOSegment* subject_;

public:
    LineIOSegmentBuilder()
            : subject_(new LineIOSegment())
    {}

    template<typename T, typename = ScalarCompatible<T>>
    LineIOSegmentBuilder& addVariable(T& var, string name) {
        return addScalarVariable(Scalar::create(var, name));
    }

    LineIOSegmentBuilder& addScalarVariable(Scalar* variable) {
        subject_->variables_.push_back(new LineIOSegmentScalarVariable(variable));
        return *this;
    }

    LineIOSegment* build() {
        return subject_;
    }
};

}
