#pragma once

#include "tcframe/experimental/io/IOSegmentVisitor.hpp"

namespace tcframe { namespace experimental {

struct IOSegment {
public:
    virtual ~IOSegment(){}

    virtual void accept(IOSegmentVisitor* visitor) = 0;
};

}}
