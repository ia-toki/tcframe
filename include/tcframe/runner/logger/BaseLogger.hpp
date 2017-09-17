#pragma once

#include <map>
#include <stdexcept>
#include <string>

#include "tcframe/runner/os.hpp"

using std::map;
using std::runtime_error;
using std::string;

namespace tcframe {

class BaseLogger {
public:
    virtual ~BaseLogger() = default;

    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(const string& testCaseName) = 0;
    virtual void logError(runtime_error* e) = 0;
};

}
