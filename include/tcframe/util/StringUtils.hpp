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

    static string trim(const string& s) {
        static const string delimiters = " \f\n\r\t\v";
        size_t begin = s.find_first_not_of(delimiters);
        size_t end = s.find_last_not_of(delimiters);

        if (begin == string::npos) {
            return "";
        }
        return s.substr(begin, end - begin + 1);
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
