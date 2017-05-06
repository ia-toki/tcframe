#pragma once

#include <string>

#include "tcframe/spec/testcase.hpp"

using std::string;

namespace tcframe {

class TestUtils {
public:
    static TestCase newSampleTestCase(string name) {
        return newSampleTestCase(name, {});
    }

    static TestCase newSampleTestCase(string name, set<int> subtaskIds) {
        return TestCaseBuilder().setName(name).setSubtaskIds(subtaskIds).build();
    }

    static TestCase newTestCase(string name) {
        return newTestCase(name, {Subtask::MAIN_ID});
    }

    static TestCase newTestCase(string name, set<int> subtaskIds) {
        return TestCaseBuilder().setName(name).setSubtaskIds(subtaskIds).build();
    }
};

}
