#ifndef TCFRAME_UTIL_H
#define TCFRAME_UTIL_H

#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::string;
using std::vector;

namespace tcframe {

class Util {
public:
    template<typename T>
    static string toString(T obj) {
        ostringstream sout;
        sout << obj;
        return sout.str();
    }

    static int toInt(string s) {
        istringstream sin(s);
        int i;
        sin >> i;
        return i;
    }

    static vector<string> split(string s) {
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == ',') {
                s[i] = ' ';
            }
        }

        istringstream sin(s);
        string t;

        vector<string> result;

        while (sin >> t) {
            result.push_back(t);
        }

        return result;
    }

    static string constructTestCaseBaseName(string slug, int testGroupId) {
        if (testGroupId == 0) {
            return slug + "_sample";
        } else if (testGroupId == -1) {
            return slug;
        } else {
            return slug + "_" + toString(testGroupId);
        }
    }

    static string constructTestCaseName(string slug, int testGroupId, int testCaseId) {
        return constructTestCaseBaseName(slug, testGroupId) + "_" + toString(testCaseId);
    }
};

}

#endif
