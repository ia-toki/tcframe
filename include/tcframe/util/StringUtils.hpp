#pragma once

#include <iomanip>
#include <istream>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

#include "optional.hpp"

using std::fixed;
using std::istream;
using std::istreambuf_iterator;
using std::istringstream;
using std::ostringstream;
using std::set;
using std::setprecision;
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
    static string toString(T obj, int precision) {
        ostringstream out;
        out << fixed << setprecision(2) << obj;
        return out.str();
    }

    static string streamToString(istream* in) {
        return string(istreambuf_iterator<char>(*in), istreambuf_iterator<char>());
    }

    template<typename T>
    static string setToString(const set<T>& set) {
        ostringstream out;
        out << "{";
        bool any = false;
        for (const T& val : set) {
            if (any) {
                out << ", ";
            }
            any = true;
            out << val;
        }
        out << "}";
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

    static string replace(const string& s, char c, const string& replacement) {
        string res;
        for (char t : s) {
            if (t == c) {
                res += replacement;
            } else {
                res += t;
            }
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
