#pragma once

#include <iostream>
#include <string>

#include "LoggerEngine.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe {

class SimpleLoggerEngine : public LoggerEngine {
public:
    virtual ~SimpleLoggerEngine() = default;

    void logHeading(const string& message) {
        cout << endl;
        cout << "[ " << message << " ]" << endl;
    }

    void logParagraph(int level, const string& message) {
        indent(level);
        cout << message << endl;
    }

    void logHangingParagraph(int level, const string& message) {
        indent(level);
        cout << message << flush;
    }

    void logListItem1(int level, const string& message) {
        indent(level);
        cout << "* " << message << endl;
    }

    void logListItem2(int level, const string& message) {
        indent(level);
        cout << "- " << message << endl;
    }

private:
    static void indent(int level) {
        for (int i = 0; i < level; i++) {
            cout << "  ";
        }
    }
};

}
