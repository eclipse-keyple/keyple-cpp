#include "KeypleRuntimeException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

KeypleRuntimeException::KeypleRuntimeException(const std::string &message) : Exception(message) {
}

KeypleRuntimeException::KeypleRuntimeException(const std::string &message, std::runtime_error cause) : Exception(message, cause) {
}

}
}
}
}
}
}
