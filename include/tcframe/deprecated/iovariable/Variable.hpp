#pragma once

#include "tcframe/deprecated/iovariable/ParsingException.hpp"

#include <cctype>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

using std::char_traits;
using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class Variable {
public:
    virtual ~Variable() { }

    virtual void clear() = 0;

    string getName() {
        return name;
    }

protected:
    Variable(string name)
            : name(name) { }

    static void checkNoEof(istream& in, string realName) {
        if (in.peek() == char_traits<char>::eof()) {
            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: <EOF>");
        }
    }

    static void checkNoWhitespace(istream& in, string realName) {
        if (isspace(in.peek())) {
            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: <whitespace>");
        }
    }

    template<typename T>
    static void parseVariableFrom(istream& in, T* value, string realName) {
        checkNoEof(in, realName);
        checkNoWhitespace(in, realName);

        int currentPos = in.tellg();
        in >> *value;

        if (in.fail()) {
            in.clear();
            in.seekg(currentPos);
            in.width(20);
            string found;
            in >> found;

            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: '" + found + "'");
        }
    }

private:
    string name;
};

}
