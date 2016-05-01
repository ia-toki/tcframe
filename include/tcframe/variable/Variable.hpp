#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

using std::char_traits;
using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::tie;

namespace tcframe {

enum VariableType {
    SCALAR,
    VECTOR,
    MATRIX
};

class Variable {
private:
    string name_;
    VariableType type_;

public:
    virtual ~Variable() {}

    virtual const string& name() const {
        return name_;
    }

    virtual VariableType type() const {
        return type_;
    }

    bool equals(Variable* o) const {
        return tie(name_, type_) == tie(o->name_, o->type_);
    }

protected:
    Variable(const string& name, VariableType type)
            : name_(name)
            , type_(type)
    {}

    template<typename T>
    static void parseValue(istream* in, T& val, const string& context) {
        ensureNoEof(in, context);
        ensureNoWhitespace(in, context);

        long long currentPos = in->tellg();
        *in >> val;

        if (in->fail()) {
            in->clear();
            in->seekg(currentPos);
            in->width(20);
            string found;
            *in >> found;

            throw runtime_error("Cannot parse for '" + context + "`. Found: '" + found + "'");
        }
    }

private:
    static void ensureNoEof(istream* in, const string& context) {
        if (in->peek() == char_traits<char>::eof()) {
            throw runtime_error("Cannot parse for '" + context + "'. Found: <EOF>");
        }
    }

    static void ensureNoWhitespace(istream* in, const string& context) {
        if (isspace(in->peek())) {
            throw runtime_error("Cannot parse for '" + context + "'. Found: <whitespace>");
        }
    }
};

}
