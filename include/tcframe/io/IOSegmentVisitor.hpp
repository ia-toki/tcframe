#pragma once

namespace tcframe {

struct LineIOSegment;

class IOSegmentVisitor {
public:
    virtual ~IOSegmentVisitor() {}

    virtual void visit(LineIOSegment* segment) const = 0;
};

}
