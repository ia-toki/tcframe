#pragma once

#include "tcframe/format/IOSegment.hpp"

namespace tcframe {

struct FakeIOSegment : public IOSegment {
private:
    IOSegmentType type_;

public:
    FakeIOSegment()
            : FakeIOSegment(IOSegmentType::LINE)
    {}

    FakeIOSegment(IOSegmentType type)
            : type_(type)
    {}

    IOSegmentType type() {
        return type_;
    }
};

}
