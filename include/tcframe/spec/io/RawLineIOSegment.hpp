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

struct RawLineIOSegment : public IOSegment {
    friend class RawLineIOSegmentBuilder;

private:
    Scalar* variable_;

public:
    virtual ~RawLineIOSegment() = default;

    IOSegmentType type() const {
        return IOSegmentType::RAW_LINE;
    }

    Scalar* variable() const {
        return variable_;
    }

    bool operator==(const RawLineIOSegment& o) const {
        return variable_->equals(o.variable_);
    }

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::RAW_LINE && *this == *((RawLineIOSegment*) o);
    }
};

class RawLineIOSegmentBuilder : public IOSegmentBuilder {
private:
    RawLineIOSegment* subject_ = new RawLineIOSegment();

public:
    RawLineIOSegmentBuilder& addScalarVariable(Scalar* variable) {
        checkVariable();
        subject_->variable_ = variable;
        return *this;
    }

    RawLineIOSegment* build() {
        checkState();
        return subject_;
    }

private:
    void checkVariable() {
        if (subject_->variable_ != nullptr) {
            throw runtime_error("Raw line segment must have exactly one variable");
        }
    }

    void checkState() {
        if (subject_->variable_ == nullptr) {
            throw runtime_error("Raw line segment must have exactly one variable");
        }
    }
};

}
