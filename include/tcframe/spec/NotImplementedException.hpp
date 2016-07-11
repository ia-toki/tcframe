#pragma once

#include <exception>

using std::exception;

namespace tcframe {

struct NotImplementedException : public exception {};

}
