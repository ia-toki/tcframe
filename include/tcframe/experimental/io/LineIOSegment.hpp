#pragma once

#include <vector>

#include "tcframe/experimental/io/LineIOSegmentScalarVariable.hpp"
#include "tcframe/experimental/io/LineIOSegmentVariable.hpp"
#include "tcframe/experimental/io/IOSegment.hpp"
#include "tcframe/experimental/io/IOSegmentVisitor.hpp"
#include "tcframe/experimental/type/Scalar.hpp"
#include "tcframe/experimental/util/StringUtils.hpp"

using std::vector;

namespace tcframe { namespace experimental {

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

}}
