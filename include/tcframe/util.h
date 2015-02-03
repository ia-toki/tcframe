#ifndef TCFRAME_UTIL_H
#define TCFRAME_UTIL_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe { namespace util {

template<typename T>
string toString(T obj) {
    ostringstream sout;
    sout << obj;
    return sout.str();
}

string constructTestCaseFilename(string slug, int testGroupId, int testCaseId) {
    if (testCaseId) {
        return slug + "_" + toString(testGroupId) + "_" + toString(testCaseId);
    } else {
        return slug + "_" + toString(testCaseId);
    }
}

} }

#endif
