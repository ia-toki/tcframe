#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "optional.hpp"

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

    template<typename T>
    static optional<T> toNumber(const string& s) {
        istringstream in(s);
        T res;
        in >> res;
        if (in.eof()) {
            return optional<T>(res);
        }
        return optional<T>();
    }

    static string interpolate(const string& s, int data) {
        char res[1024];
        sprintf(res, s.c_str(), data);
        return string(res);
    }

    static string escape(const string& s, const string& badChars) {
        string res;
        for (char c : s) {
            if (badChars.find_first_of(c) != string::npos) {
                res += "\\";
            }
            res += c;
        }
        return res;
    }

    static vector<string> split(const string& s, char delimiter) {
        vector<string> result;

        istringstream in(s);
        string token;
        while (getline(in, token, delimiter)) {
            result.push_back(token);
        }

        return result;
    }

    static vector<string> splitAndTrimBySpace(const string& s) {
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
