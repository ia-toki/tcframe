#pragma once

namespace tcframe {

enum IOSegmentType {
    GRID,
    LINE,
    LINES
};

struct IOSegment {
public:
    virtual ~IOSegment() {}

    virtual IOSegmentType type() const = 0;

    virtual bool equals(IOSegment* o) const = 0;
};

class IOSegmentBuilder {
public:
    virtual ~IOSegmentBuilder() {}

    virtual IOSegment* build() = 0;
};

}
