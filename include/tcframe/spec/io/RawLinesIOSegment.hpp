#pragma once

#include <functional>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "IOSegment.hpp"
#include "tcframe/spec/variable.hpp"

using std::function;
using std::move;
using std::runtime_error;
using std::tie;
using std::vector;

namespace tcframe {

struct RawLinesIOSegment : public IOSegment {
    friend class RawLinesIOSegmentBuilder;

private:
    Vector* variable_;
    function<int()> size_;

public:
    virtual ~RawLinesIOSegment() = default;

    IOSegmentType type() const {
        return IOSegmentType::RAW_LINES;
    }

    Vector* variable() const {
        return variable_;
    }

    const function<int()>& size() const {
        return size_;
    }

    bool operator==(const RawLinesIOSegment& o) const {
        return size_() == o.size_() && variable_->equals(o.variable_);
    }

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::RAW_LINES && *this == *((RawLinesIOSegment*) o);
    }
};

class RawLinesIOSegmentBuilder : public IOSegmentBuilder {
private:
    RawLinesIOSegment* subject_ = new RawLinesIOSegment();

public:
    RawLinesIOSegmentBuilder() {
        subject_->size_ = [] {return NO_SIZE;};
    }

    RawLinesIOSegmentBuilder& addVectorVariable(Vector* variable) {
        checkVariable();
        subject_->variable_ = variable;
        return *this;
    }

    RawLinesIOSegmentBuilder& setSize(function<int()> size) {
        subject_->size_ = move(size);
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
