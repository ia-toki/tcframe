#ifndef CPTEST_UTIL_H
#define CPTEST_UTIL_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace org { namespace iatoki { namespace cptest {

template<typename T>
string toString(T obj) {
    ostringstream sout;
    sout << obj;
    return sout.str();
}

}}}

#endif