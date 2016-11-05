#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/MetadataParser.hpp"

namespace tcframe {

class MockMetadataParser : public MetadataParser {
public:
    MockMetadataParser()
            : MetadataParser(nullptr) {}

    MOCK_METHOD1(parse, Metadata(const string&));
};

}
