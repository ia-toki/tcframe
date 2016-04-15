#pragma once

#include <vector>
#include <utility>

#include "tcframe/experimental/io/IOSegment.hpp"

using std::move;
using std::vector;

namespace tcframe { namespace experimental {

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
    void prepareForInputFormat() {
        currentFormat_ = &subject_.inputFormat_;
    }

    void prepareForOutputFormat() {
        currentFormat_ = &subject_.outputFormat_;
    }

    void addIOSegment(IOSegment* segment) {
        currentFormat_->push_back(segment);
    }

    IOFormat build() {
        return move(subject_);
    }
};

}}
