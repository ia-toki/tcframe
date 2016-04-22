#pragma once

#include <vector>
#include <utility>

#include "IOSegment.hpp"

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

public:
    IOFormatBuilder& prepareForInputFormat() {
        currentFormat_ = &subject_.inputFormat_;
        return *this;
    }

    IOFormatBuilder& prepareForOutputFormat() {
        currentFormat_ = &subject_.outputFormat_;
        return *this;
    }

    IOFormatBuilder& addIOSegment(IOSegment* segment) {
        currentFormat_->push_back(segment);
        return *this;
    }

    IOFormat build() {
        return move(subject_);
    }
};

}
