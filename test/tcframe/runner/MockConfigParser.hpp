#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/ConfigParser.hpp"

namespace tcframe {

class MockConfigParser : public ConfigParser {
public:
    MockConfigParser()
            : ConfigParser(nullptr) {}

    MOCK_METHOD1(parse, Config(const string&));
};

}
