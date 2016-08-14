#pragma once

#include <string>

#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

class TestCaseIdCreator {
public:
    TestCaseIdCreator() = delete;

    static string create(const string& slug, int testGroupId, int testCaseNo) {
        return createBaseId(slug, testGroupId) + "_" + StringUtils::toString(testCaseNo);
    }

    static string createBaseId(const string& slug, int testGroupId) {
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
