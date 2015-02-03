#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe { namespace logger {

void logIntroduction() {
    cout << "Generating test cases..." << endl;
}

void logTestGroupHeader(int testGroupId) {
    cout << endl;

    if (testGroupId == 0) {
        cout << "[ SAMPLE TEST CASES ]" << endl;
    } else if (testGroupId != -1) {
        cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
    }
}

void logTestCaseHeader(string name) {
    cout << "  " << name << ": " << flush;
}

void logTestCaseSatisfiedResult() {
    cout << "OK" << endl;
}

void logTestCaseUnsatisfiedResult() {
    cout << "FAILED" << endl;
    // TODO: log reasons
}

} }

#endif
