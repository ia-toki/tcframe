#pragma once

#include <map>
#include <string>
#include <vector>

#include "tcframe/util.hpp"

using std::map;
using std::string;
using std::vector;

namespace tcframe {

class SimpleYamlParser {
public:
    SimpleYamlParser() = delete;

    static map<string, string> parse(const string& s) {
        map<string, string> res;

        for (string line : StringUtils::split(s, '\n')) {
            string cleanedLine = stripComment(line);
            vector<string> tokens = StringUtils::split(cleanedLine, ':');
            if (tokens.size() != 2) {
                continue;
            }

            string cleanedKey = StringUtils::trim(tokens[0]);
            string cleanedValue = StringUtils::trim(tokens[1]);
            if (cleanedKey.empty()) {
                continue;
            }

            res[cleanedKey] = cleanedValue;
        }

        return res;
    }

private:
    static string stripComment(const string& s) {
        size_t hashPos = s.find_last_of('#');
        if (hashPos != string::npos) {
            return s.substr(0, hashPos);
        }
        return s;
    }
};

}
