#ifndef TCFRAME_UTIL_H
#define TCFRAME_UTIL_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe {

template<typename T>
string toString(T obj) {
    ostringstream sout;
    sout << obj;
    return sout.str();
}

}

#endif