#pragma once

#include <string>

#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

class TestCaseNameCreator {
public:
    TestCaseNameCreator() = delete;

    static string create(const string& slug, int testGroupId, int testCaseId) {
        return createBaseName(slug, testGroupId) + "_" + StringUtils::toString(testCaseId);
    }

    static string createBaseName(const string& slug, int testGroupId) {
        if (testGroupId == 0) {
            return slug + "_sample";
        }
        if (testGroupId != -1) {
            return slug + "_" + StringUtils::toString(testGroupId);
        }
        return slug;
    }
};

}
