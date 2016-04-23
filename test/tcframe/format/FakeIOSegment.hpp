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

    IOSegmentType type() const {
        return type_;
    }

protected:
    bool equals(const IOSegment& o) const {
        return this == &o;
    }
};

}
