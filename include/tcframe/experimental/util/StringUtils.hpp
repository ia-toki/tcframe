#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::replace;
using std::string;
using std::vector;

namespace tcframe { namespace experimental {

class StringUtils {
private:
    StringUtils() {}
public:
    static vector<string> split(string s) {
        replace(s.begin(), s.end(), ',', ' ');
        istringstream sin(s);
        string t;

        vector<string> result;

        while (sin >> t) {
            result.push_back(t);
        }

        return result;
    }
};

}}
