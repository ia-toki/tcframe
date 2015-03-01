#ifndef TCFRAME_UTIL_H
#define TCFRAME_UTIL_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe {

class Util {
public:
    template<typename T>
    static string toString(T obj) {
        ostringstream sout;
        sout << obj;
        return sout.str();
    }

    static string constructTestCaseName(string slug, int testGroupId, int testCaseId) {
        if (testGroupId != -1) {
            return slug + "_" + toString(testGroupId) + "_" + toString(testCaseId);
        } else {
            return slug + "_" + toString(testCaseId);
        }
    }
};

}

#endif
