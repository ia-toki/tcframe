#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include <exception>

using std::exception;

namespace tcframe {

class NotImplementedException : public exception { };

}

#endif
