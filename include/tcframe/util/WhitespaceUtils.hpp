#pragma once

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

using std::char_traits;
using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;

namespace tcframe {

class WhitespaceUtils {
public:
    static void parseSpace(istream* in, const string& context) {
        if (in->get() != ' ') {
            throw runtime_error("Expected: <space> after " + context);
        }
    }

    static void parseSpaceAfterMissingNewline(istream *in, const string &context) {
        if (in->get() != ' ') {
            throw runtime_error("Expected: <space> or <newline> after " + context);
        }
    }

    static bool canParseNewline(istream* in) {
        return in->peek() == '\n';
    }

    static void parseNewline(istream* in, const string& context) {
        if (in->get() != '\n') {
            throw runtime_error("Expected: <newline> after " + context);
        }
    }

    static void ensureEof(istream* in) {
        if (in->peek() != char_traits<char>::eof()) {
            throw runtime_error("Expected: <EOF>");
        }
    }

    static void ensureNoEof(istream* in, const string& context) {
        if (in->peek() == char_traits<char>::eof()) {
            throw runtime_error("Cannot parse for " + context + ". Found: <EOF>");
        }
    }

    static void ensureNoWhitespace(istream* in, const string& context) {
        if (isspace(in->peek())) {
            throw runtime_error("Cannot parse for " + context + ". Found: <whitespace>");
        }
    }
};

}
