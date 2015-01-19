#ifndef CPTEST_EXCEPTION_H
#define CPTEST_EXCEPTION_H

#include <exception>

using std::exception;

namespace org { namespace iatoki { namespace cptest {

class NotImplementedException : public exception { };

}}}

#endif