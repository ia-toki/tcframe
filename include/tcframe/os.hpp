#ifndef TCFRAME_OS_H
#define TCFRAME_OS_H

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

using std::ifstream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

struct ExecutionResult {
    int exitCode;
    istream* outputStream;
    istream* errorStream;
};

class OperatingSystem {
public:
    virtual ~OperatingSystem() { }

    virtual void setBaseDirectory(string baseDirectoryName) = 0;
    virtual istream* openForReading(string name) = 0;
    virtual ostream* openForWriting(string name) = 0;
    virtual void remove(string name) = 0;
    virtual ExecutionResult execute(string command, string inputName, string outputName) = 0;
};

class UnixOperatingSystem : public OperatingSystem {
public:
    UnixOperatingSystem()
            : baseDirectoryName(".") { }

    void setBaseDirectory(string baseDirectoryName) override {
        this->baseDirectoryName = baseDirectoryName;

        system(("rm -rf " + baseDirectoryName).c_str());
        system(("mkdir -p " + baseDirectoryName).c_str());
    }

    istream* openForReading(string name) override {
        string filename = baseDirectoryName + "/" + name;
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(string name) override {
        string filename = baseDirectoryName + "/" + name;
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    ExecutionResult execute(string command, string inputName, string outputName) override {
        string inputFilename = baseDirectoryName + "/" + inputName;
        string outputFilename = baseDirectoryName + "/" + outputName;
        string errorFilename = baseDirectoryName + "/_error.out";

        ExecutionResult result;
        result.exitCode = system((command + " < " + inputFilename + " > " + outputFilename + " 2> " + errorFilename).c_str());
        result.outputStream = openForReading(outputFilename);
        result.errorStream = openForReadingAsStringStream(errorFilename);

        return result;
    }

    void remove(string name) override {
        string filename = baseDirectoryName + "/" + name;
        removeFile(filename);
    }

private:
    string baseDirectoryName;

    void removeFile(string filename) {
        system(("rm -f " + filename).c_str());
    }

    istringstream* openForReadingAsStringStream(string filename) {
        ifstream file(filename);

        ostringstream buffer;
        buffer << file.rdbuf();

        removeFile(filename);

        return new istringstream(buffer.str());
    }
};

}

#endif
