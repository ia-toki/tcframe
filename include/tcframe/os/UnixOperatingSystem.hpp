#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>

#include "ExecutionResult.hpp"
#include "OperatingSystem.hpp"
#include "tcframe/spec/testcase/TestCaseIdCreator.hpp"

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

    ExecutionResult execute(const ExecutionRequest& request) {
        ostringstream sout;

        sout << "{ ";

        if (request.timeLimit()) {
            sout << "ulimit -S -t " << request.timeLimit().value() << "; ";
        }

        // Note: this has no effect on OS X. No known workaround unfortunately.
        if (request.memoryLimit()) {
            sout << "ulimit -S -v " << request.memoryLimit().value() * 1024 << "; ";
        }

        sout << request.command() << "; }";

        if (request.inputFilename()) {
            sout << " < " << request.inputFilename().value();
        }
        if (request.outputFilename()) {
            sout << " > " << request.outputFilename().value();
        } else {
            sout << " > /dev/null";
        }
        if (request.errorFilename()) {
            sout << " 2> " << request.errorFilename().value();
        } else {
            sout << " 2> /dev/null";
        }

        ExecutionInfoBuilder info;

        int exitValue = system(sout.str().c_str());
        int exitStatus = WEXITSTATUS(exitValue);

        if (WIFSIGNALED(exitStatus)) {
            int signal = WTERMSIG(exitStatus);
            info.setExitSignal(strsignal(signal));
            if (signal == SIGXCPU) {
                info.setExceededCpuLimits(true);
            }
        } else {
            info.setExitCode(exitStatus);
        }

        istream* outputStream;
        istream* errorStream;

        if (request.outputFilename()) {
            outputStream = openForReading(request.outputFilename().value());
        } else {
            outputStream = new istringstream();
        }

        if (request.errorFilename()) {
            errorStream = openForReadingAsStringStream(request.errorFilename().value());
        } else {
            errorStream = new istringstream();
        }

        return ExecutionResult(info.build(), outputStream, errorStream);
    }

    void combineMultipleTestCases(
            const string& slug,
            int testGroupId,
            int testCasesCount,
            const string& outputDir,
            const optional<string>& outputPrefix) {

        string baseId = TestCaseIdCreator::createBaseId(slug, testGroupId);
        string baseIn = outputDir + "/" + baseId + ".in";
        string baseOut = outputDir + "/" + baseId + ".out";

        ostringstream sout;

        sout << "echo " << testCasesCount << " > " << baseId << ".in && ";
        sout << "touch " << baseId << ".out";
        system(sout.str().c_str());

        for (int i = 1; i <= testCasesCount; i++) {
            string id = TestCaseIdCreator::create(slug, testGroupId, i);
            string in = outputDir + "/" + id + ".in";
            string out = outputDir + "/" + id + ".out";

            ostringstream sout2;
            sout2 << "tail -n +2 " << in << " >> " << baseIn << " && ";

            if (i > 1 && outputPrefix) {
                // Replace the prefix for the first tc, with the correct prefix for this tc
                string firstPrefix = StringUtils::interpolate(outputPrefix.value(), 1);
                string correctPrefix = StringUtils::interpolate(outputPrefix.value(), i);
                sout2 << "printf \"" << correctPrefix << "\" >> " << baseOut << " && ";
                sout2 << "cut -c " << (firstPrefix.size() + 1) << "- " << out << " >> " << baseOut;
            } else {
                sout2 << "cat " << out << " >> " << baseOut;
            }

            system(sout2.str().c_str());

            ostringstream sout3;
            sout3 << "rm " << in << " " << out;
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
