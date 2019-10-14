#include "NoStackTraceThrowable.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

NoStackTraceThrowable::NoStackTraceThrowable() : std::runtime_error("")
{

}

NoStackTraceThrowable::NoStackTraceThrowable(const std::string &message) : std::runtime_error(message)
{

}

std::runtime_error NoStackTraceThrowable::fillInStackTrace() {
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
//                        synchronized(shared_from_this()) {
//                            return shared_from_this();
//                        }
    return std::runtime_error("NoStackTraceThrowable");
}

}
}
}
}
}
}
