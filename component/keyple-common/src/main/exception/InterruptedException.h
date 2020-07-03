#pragma once

#include "Exception.h"

namespace keyple {
namespace common {

class InterruptedException : public Exception {
public:
    /**
     *
     */
    InterruptedException(const std::string& name) : Exception(name) {}

    /**
     *
     */
    InterruptedException(const std::string& name, const std::exception& cause)
    : Exception(name, cause) {}
};

}
}
