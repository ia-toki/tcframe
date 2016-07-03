#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ExecutionResult.hpp"
#include "OperatingSystem.hpp"

using std::ifstream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

class UnixOperatingSystem : public OperatingSystem {
public:
    istream* openForReading(const string& filename) {
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(const string& filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    void closeOpenedWritingStream(ostream* out) {
        delete out;
    }

    void forceMakeDir(const string& dirName) {
        runCommand("rm -rf " + dirName);
        runCommand("mkdir -p " + dirName);
    }

    void removeFile(const string& filename) {
        runCommand("rm -rf " + filename);
    }

    ExecutionResult execute(
            const string& command,
            const string& inputFilename,
            const string& outputFilename,
            const string& errorFilename) {

        ostringstream sout;

        sout << "{ " << command << "; }";

        if (!inputFilename.empty()) {
            sout << " < " << inputFilename;
        }
        if (outputFilename.empty()) {
            sout << " > /dev/null";
        } else {
            sout << " > " << outputFilename;
        }
        if (errorFilename.empty()) {
            sout << " 2> /dev/null";
        } else {
            sout << " 2> " << errorFilename;
        }

        int exitCode = system(sout.str().c_str());
        int exitStatus = WEXITSTATUS(exitCode);

        istream* outputStream;
        istream* errorStream;

        if (outputFilename.empty()) {
            outputStream = new istringstream();
        } else {
            outputStream = openForReading(outputFilename);
        }

        if (errorFilename.empty()) {
            errorStream = new istringstream();
        } else {
            errorStream = openForReadingAsStringStream(errorFilename);
        }

        return ExecutionResult(exitStatus, outputStream, errorStream);
    }

    void combineMultipleTestCases(const string& testCaseBaseFilename, int testCasesCount) {
        ostringstream sout;
        sout << "echo " << testCasesCount << " > " << testCaseBaseFilename << ".in";
        sout << " && touch " << testCaseBaseFilename << ".out";
        system(sout.str().c_str());

        for (int i = 1; i <= testCasesCount; i++) {
            ostringstream sout2;
            sout2 << "tail -n +2 " << testCaseBaseFilename << "_" << i << ".in >> " << testCaseBaseFilename << ".in";
            sout2 << "&& cat " << testCaseBaseFilename << "_" << i << ".out >> " << testCaseBaseFilename << ".out";
            system(sout2.str().c_str());

            ostringstream sout3;
            sout3 << "rm " << testCaseBaseFilename << "_" << i << ".in ";
            sout3 << testCaseBaseFilename << "_" << i << ".out";
            system(sout3.str().c_str());
        }
    }

private:
    istringstream* openForReadingAsStringStream(const string& filename) {
        ifstream file(filename);

        ostringstream buffer;
        buffer << file.rdbuf();

        removeFile(filename);

        return new istringstream(buffer.str());
    }

    void runCommand(const string& command) {
        system(command.c_str());
    }
};

}
