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
    virtual ostream* createFile(string baseDirectoryName, string filename) = 0;
    virtual void execute(string command, string input, string output) = 0;
};

class Unix : public OperatingSystem {
public:
    void createDirectory(string directoryName) {
        ostringstream ss;
        ss << "rm -rf " << directoryName;
        system(ss.str().c_str());

        ostringstream ss2;
        ss2 << "mkdir -p " << directoryName;
        system(ss2.str().c_str());
    }

    ostream* createFile(string baseDirectoryName, string filename) {
        ofstream* file = new ofstream();
        file->open(baseDirectoryName + "/" + filename);
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
