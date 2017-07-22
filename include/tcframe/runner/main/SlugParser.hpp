#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "tcframe/util.hpp"

using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

class SlugParser {
public:
    SlugParser() = delete;

    static string parse(const string& specPath) {
        return validateSlug(getSlug(getParentDirectory(specPath)));
    }

private:
    static string getParentDirectory(const string& specPath) {
        vector<string> tokens = StringUtils::split(specPath, '/');
        if (tokens.size() < 2) {
            return "";
        }
        return tokens[tokens.size() - 2];
    }

    static string getSlug(const string& parentDirectory) {
        vector<string> tokens = StringUtils::split(parentDirectory, '_');
        if (tokens.empty()) {
            return "";
        }
        return tokens.back();
    }

    static string validateSlug(const string& slug) {
        if (slug.empty()) {
            return invalidSlug(slug);
        }
        for (char c : slug) {
            if (!valid(c)) {
                return invalidSlug(slug);
            }
        }
        return slug;
    }

    static string invalidSlug(const string& slug) {
        throw runtime_error("Slug must match [a-zA-Z0-9\\-]+, but found: '" + slug + "'");
    }

    static bool valid(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-';
    }
};

}
