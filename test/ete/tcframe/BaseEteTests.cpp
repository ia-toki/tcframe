#include "gmock/gmock.h"

#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using ::testing::Test;

using std::istringstream;
using std::shared_ptr;
using std::string;
using std::vector;

namespace tcframe {

class BaseEteTests : public Test {
protected:
    int execStatus(const string& command) {
        return system(command.c_str());
    }

    string exec(const string& command) {
        char buf[128];
        string res;
        shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        while (!feof(pipe.get())) {
            if (fgets(buf, 128, pipe.get()) != nullptr) {
                res += buf;
            }
        }
        return res;
    }

    vector<string> ls(const string& dir) {
        istringstream sin(exec("ls -1 " + dir));
        vector<string> files;
        string file;
        while (sin >> file) {
            files.push_back(file);
        }
        return files;
    }

    string readFile(const string& path) {
        return exec("cat " + path);
    }
};

}
