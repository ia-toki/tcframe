#pragma once

#include <string>

#include "tcframe/spec/testcase.hpp"

using std::string;

namespace tcframe {

class TestUtils {
public:
    static TestCase createFakeTestCase(string name) {
        return createFakeTestCase(name, {-1});
    }

    static TestCase createFakeTestCase(string name, set<int> subtaskIds) {
        return TestCaseBuilder().setName(name).setSubtaskIds(subtaskIds).build();
    }
};

}
