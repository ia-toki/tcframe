#pragma once

#include <vector>
#include <utility>

#include "IOSegment.hpp"
#include "LineIOSegment.hpp"

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
};

class IOFormatBuilder {
private:
    IOFormat subject_;
    vector<IOSegment*>* currentFormat_;
    IOSegmentBuilder* lastBuilder_;

public:
    IOFormatBuilder()
            : lastBuilder_(nullptr)
    {}

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
