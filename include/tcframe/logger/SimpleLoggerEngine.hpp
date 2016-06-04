#pragma once

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe {

class SimpleLoggerEngine : public LoggerEngine {
public:
    void logHeading(string message) {
        cout << endl;
        cout << "[ " << message << " ]" << endl;
    }

    void logParagraph(int level, string message) {
        indent(level);
        cout << message << endl;
    }

    void logHangingParagraph(int level, string message) {
        indent(level);
        cout << message << flush;
    }

    void logListItem1(int level, string message) {
        indent(level);
        cout << "* " << message << endl;
    }

    void logListItem2(int level, string message) {
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
