#ifndef TCFRAME_OS_H
#define TCFRAME_OS_H

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

class OperatingSystem {
public:
    virtual ~OperatingSystem() { }

    virtual void createDirectory(string directoryName) = 0;
    virtual void removeDirectory(string directoryName) = 0;
    virtual ostream* openFile(string filename) = 0;
    virtual void execute(string command, string input, string output) = 0;
};

class Unix : public OperatingSystem {
public:
    void createDirectory(string directoryName) {
        ostringstream ss;
        ss << "mkdir -p " << directoryName;
        system(ss.str().c_str());
    }

    void removeDirectory(string directoryName) {
        ostringstream ss;
        ss << "rm -rf " << directoryName;
        system(ss.str().c_str());
    }

    ostream* openFile(string filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    void execute(string program, string in, string out) {
        ostringstream ss;
        ss << program << " < " << in << " > " << out << " 2>&1";
        pclose(popen(ss.str().c_str(), "r"));
    }
};

}

#endif