#pragma once

#include <tuple>
#include <vector>

#include "tcframe/io/LineIOSegmentScalarVariable.hpp"
#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/io/IOSegment.hpp"
#include "tcframe/io/IOSegmentVisitor.hpp"
#include "tcframe/type/Scalar.hpp"
#include "tcframe/util/StringUtils.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct LineIOSegment : public IOSegment {
    friend class LineIOSegmentBuilder;

private:
    vector<LineIOSegmentVariable*> variables_;

public:
    const vector<LineIOSegmentVariable*>& variables() const {
        return variables_;
    }

    void accept(IOSegmentVisitor* visitor) {
        return visitor->visit(this);
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

    LineIOSegment* build() {
        return subject_;
    }

private:
    LineIOSegmentBuilder& addScalarVariable(Scalar* variable) {
        subject_->variables_.push_back(new LineIOSegmentScalarVariable(variable));
        return *this;
    }
};

}
