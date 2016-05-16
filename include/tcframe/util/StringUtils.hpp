#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::replace;
using std::string;
using std::vector;

namespace tcframe {

class StringUtils {
public:
    StringUtils() = delete;

    template<typename T>
    static string toString(T obj) {
        ostringstream out;
        out << obj;
        return out.str();
    }

    static vector<string> split(string s, char delimiter) {
        vector<string> result;

        istringstream in(s);
        string token;
        while (getline(in, token, delimiter)) {
            result.push_back(token);
        }

        return result;
    }

    static vector<string> splitAndTrimBySpace(string s) {
        vector<string> result;

        istringstream in(s);
        string token;
        while (in >> token) {
            result.push_back(token);
        }

        return result;
    }
};

}
