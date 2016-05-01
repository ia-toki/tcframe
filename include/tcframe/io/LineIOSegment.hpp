#pragma once

#include <tuple>
#include <vector>

#include "IOSegment.hpp"
#include "tcframe/variable.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct LineIOSegmentVariable {
private:
    Variable* variable_;
    int size_;

public:
    LineIOSegmentVariable(Variable* variable, int size)
            : variable_(variable)
            , size_(size)
    {}

    LineIOSegmentVariable(Variable* variable)
            : variable_(variable)
            , size_(-1)
    {}

    Variable* variable() const {
        return variable_;
    }

    int size() const {
        return size_;
    }

    bool operator==(const LineIOSegmentVariable& o) const {
        return variable_->equals(o.variable_) && size_ == o.size_;
    }
};

struct LineIOSegment : public IOSegment {
    friend class LineIOSegmentBuilder;

private:
    vector<LineIOSegmentVariable> variables_;

public:
    IOSegmentType type() const {
        return IOSegmentType::LINE;
    }

    const vector<LineIOSegmentVariable>& variables() const {
        return variables_;
    }

    bool operator==(const LineIOSegment& o) const {
        return tie(variables_) == tie(o.variables_);
    }

protected:
    virtual bool equals(const IOSegment& o) const {
        return *this == (LineIOSegment&) o;
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
        subject_->variables_.push_back(LineIOSegmentVariable(variable));
        return *this;
    }

    LineIOSegment* build() {
        return subject_;
    }
};

}
