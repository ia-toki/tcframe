#pragma once

#include <vector>
#include <utility>

#include "GridIOSegment.hpp"
#include "IOSegment.hpp"
#include "LineIOSegment.hpp"
#include "LinesIOSegment.hpp"

using std::move;
using std::vector;

namespace tcframe {

struct IOFormat {
    friend class IOFormatBuilder;

private:
    vector<IOSegment*> inputFormat_;
    vector<IOSegment*> outputFormat_;

public:
    const vector<IOSegment*>& inputFormat() const {
        return inputFormat_;
    }

    const vector<IOSegment*>& outputFormat() const {
        return outputFormat_;
    }

    bool operator==(const IOFormat& o) const {
        return equals(inputFormat_, o.inputFormat_) && equals(outputFormat_, o.outputFormat_);
    }

private:
    bool equals(const vector<IOSegment*>& a, const vector<IOSegment*>& b) const {
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
    vector<IOSegment*>* currentFormat_;
    IOSegmentBuilder* lastBuilder_;

public:
    IOFormatBuilder()
            : lastBuilder_(nullptr) {}

    virtual ~IOFormatBuilder() {}

    void prepareForInputFormat() {
        addLastSegment();
        currentFormat_ = &subject_.inputFormat_;
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
                currentFormat_->push_back(lastBuilder_->build());
            }
            lastBuilder_ = nullptr;
        }
    }
};

}
