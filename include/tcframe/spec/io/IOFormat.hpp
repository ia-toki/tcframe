#pragma once

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "GridIOSegment.hpp"
#include "IOSegment.hpp"
#include "LineIOSegment.hpp"
#include "LinesIOSegment.hpp"
#include "RawLineIOSegment.hpp"
#include "RawLinesIOSegment.hpp"

using std::function;
using std::move;
using std::runtime_error;
using std::vector;

namespace tcframe {

struct IOFormat {
    friend class IOFormatBuilder;

private:
    IOSegments inputFormat_;
    function<void()> beforeOutputFormat_;
    IOSegments outputFormat_;

public:
    const IOSegments& inputFormat() const {
        return inputFormat_;
    }

    const function<void()>& beforeOutputFormat() const {
        return beforeOutputFormat_;
    }

    const IOSegments& outputFormat() const {
        return outputFormat_;
    }

    bool operator==(const IOFormat& o) const {
        return equals(inputFormat_, o.inputFormat_) && equals(outputFormat_, o.outputFormat_);
    }

private:
    bool equals(const IOSegments& a, const IOSegments& b) const {
        if (a.size() != b.size()) {
            return false;
        }
        for (int i = 0; i < a.size(); i++) {
            if (!a[i]->equals(b[i])) {
                return false;
            }
        }
        return true;
    }
};

class IOFormatBuilder {
private:
    IOFormat subject_;
    IOSegments* currentFormat_;
    IOSegmentBuilder* lastBuilder_;

public:
    IOFormatBuilder()
            : lastBuilder_(nullptr) {}

    virtual ~IOFormatBuilder() {}

    void prepareForInputFormat() {
        addLastSegment();
        currentFormat_ = &subject_.inputFormat_;
    }

    void setBeforeOutputFormat(function<void()> beforeOutputFormat) {
        subject_.beforeOutputFormat_ = beforeOutputFormat;
    }

    void prepareForOutputFormat() {
        addLastSegment();
        currentFormat_ = &subject_.outputFormat_;
    }

    LineIOSegmentBuilder& newLineIOSegment() {
        addLastSegment();
        LineIOSegmentBuilder* builder = new LineIOSegmentBuilder();
        lastBuilder_ = builder;
        return *builder;
    }

    LinesIOSegmentBuilder& newLinesIOSegment() {
        addLastSegment();
        LinesIOSegmentBuilder* builder = new LinesIOSegmentBuilder();
        lastBuilder_ = builder;
        return *builder;
    }

    RawLineIOSegmentBuilder& newRawLineIOSegment() {
        addLastSegment();
        RawLineIOSegmentBuilder* builder = new RawLineIOSegmentBuilder();
        lastBuilder_ = builder;
        return *builder;
    }

    RawLinesIOSegmentBuilder& newRawLinesIOSegment() {
        addLastSegment();
        RawLinesIOSegmentBuilder* builder = new RawLinesIOSegmentBuilder();
        lastBuilder_ = builder;
        return *builder;
    }

    GridIOSegmentBuilder& newGridIOSegment() {
        addLastSegment();
        GridIOSegmentBuilder* builder = new GridIOSegmentBuilder();
        lastBuilder_ = builder;
        return *builder;
    }

    IOFormat build() {
        addLastSegment();
        return move(subject_);
    }

private:
    void addLastSegment() {
        if (lastBuilder_ != nullptr) {
            if (currentFormat_ != nullptr) {
                checkLastSegment();
                currentFormat_->push_back(lastBuilder_->build());
            }
            lastBuilder_ = nullptr;
        }
    }

    void checkLastSegment() {
        if (currentFormat_->empty()) {
            return;
        }
        IOSegment* lastSegment = currentFormat_->back();
        if (isLinesSegmentWithoutSize(lastSegment)) {
            throw runtime_error("Lines segment without size can only be the last segment");
        }
        if (isRawLinesSegmentWithoutSize(lastSegment)) {
            throw runtime_error("Raw lines segment without size can only be the last segment");
        }
    }

    bool isLinesSegmentWithoutSize(IOSegment* segment) {
        if (segment->type() != IOSegmentType::LINES) {
            return false;
        }
        return ((LinesIOSegment*) segment)->size()() == NO_SIZE;
    }

    bool isRawLinesSegmentWithoutSize(IOSegment* segment) {
        if (segment->type() != IOSegmentType::RAW_LINES) {
            return false;
        }
        return ((RawLinesIOSegment*) segment)->size()() == NO_SIZE;
    }
};

}
