#pragma once

#include <string>

#include "tcframe/spec/testcase.hpp"

using std::string;

namespace tcframe {

class TestUtils {
public:
    static TestCase createFakeTestCase(string id) {
        return TestCaseBuilder().setId(id).build();
    }

    static TestCase createFakeTestCase(string id, set<int> subtaskIds) {
        return TestCaseBuilder().setId(id).setSubtaskIds(subtaskIds).build();
    }
};

}
