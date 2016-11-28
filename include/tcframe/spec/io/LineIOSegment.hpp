#pragma once

#include <stdexcept>
#include <tuple>
#include <vector>

#include "IOSegment.hpp"
#include "tcframe/spec/variable.hpp"

using std::runtime_error;
using std::tie;
using std::vector;

namespace tcframe {

struct LineIOSegmentVariable {
private:
    Variable* variable_;
    int* size_;

public:
    LineIOSegmentVariable(Variable* variable, int* size)
            : variable_(variable)
            , size_(size) {}

    LineIOSegmentVariable(Variable* variable)
            : variable_(variable)
            , size_(new int(-1)) {
    }

    Variable* variable() const {
        return variable_;
    }

    int* size() const {
        return size_;
    }

    bool operator==(const LineIOSegmentVariable& o) const {
        return variable_->equals(o.variable_) && *size_ == *o.size_;
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

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::LINE && *this == *((LineIOSegment*) o);
    }
};

class LineIOSegmentBuilder : public IOSegmentBuilder {
private:
    LineIOSegment* subject_;
    bool hasVectorWithoutSize_;

public:
    LineIOSegmentBuilder()
            : subject_(new LineIOSegment())
            , hasVectorWithoutSize_(false) {}

    LineIOSegmentBuilder& addScalarVariable(Scalar* variable) {
        checkVectorWithoutSize();
        subject_->variables_.push_back(LineIOSegmentVariable(variable));
        return *this;
    }

    LineIOSegmentBuilder& addVectorVariable(Vector* variable) {
        checkVectorWithoutSize();
        hasVectorWithoutSize_ = true;
        subject_->variables_.push_back(LineIOSegmentVariable(variable));
        return *this;
    }

    LineIOSegmentBuilder& addVectorVariable(Vector* variable, int* size) {
        checkVectorWithoutSize();
        subject_->variables_.push_back(LineIOSegmentVariable(variable, size));
        return *this;
    }

    LineIOSegment* build() {
        return subject_;
    }

private:
    void checkVectorWithoutSize() {
        if (hasVectorWithoutSize_) {
            throw runtime_error("Vector without size can only be the last variable in a line segment");
        }
    }
};

}
