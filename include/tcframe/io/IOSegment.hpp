#pragma once

namespace tcframe {

enum IOSegmentType {
    LINE
};

struct IOSegment {
public:
    virtual ~IOSegment() {}

    virtual IOSegmentType type() = 0;
};

}
