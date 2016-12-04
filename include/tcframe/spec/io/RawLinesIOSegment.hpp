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

struct RawLinesIOSegment : public IOSegment {
    friend class RawLinesIOSegmentBuilder;

private:
    Vector* variable_;
    int* size_;

public:
    IOSegmentType type() const {
        return IOSegmentType::RAW_LINES;
    }

    Vector* variable() const {
        return variable_;
    }

    int* size() const {
        return size_;
    }

    bool operator==(const RawLinesIOSegment& o) const {
        return *size_ == *o.size_ && variable_->equals(o.variable_);
    }

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::RAW_LINES && *this == *((RawLinesIOSegment*) o);
    }
};

class RawLinesIOSegmentBuilder : public IOSegmentBuilder {
private:
    RawLinesIOSegment* subject_;

public:
    RawLinesIOSegmentBuilder()
            : subject_(new RawLinesIOSegment()) {
        subject_->size_ = new int(-1);
    }

    RawLinesIOSegmentBuilder& addVectorVariable(Vector* variable) {
        checkVariable();
        subject_->variable_ = variable;
        return *this;
    }

    RawLinesIOSegmentBuilder& setSize(int* size) {
        subject_->size_ = size;
        return *this;
    }

    RawLinesIOSegment* build() {
        checkState();
        return subject_;
    }

private:
    void checkVariable() {
        if (subject_->variable_ != nullptr) {
            throw runtime_error("Raw lines segment must have exactly one variable");
        }
    }

    void checkState() {
        if (subject_->variable_ == nullptr) {
            throw runtime_error("Raw lines segment must have exactly one variable");
        }
    }
};

}
