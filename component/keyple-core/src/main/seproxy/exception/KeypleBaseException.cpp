#include "KeypleBaseException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

KeypleBaseException::KeypleBaseException(const std::string &message) : Exception(message) {
}

KeypleBaseException::KeypleBaseException(const std::string &message, std::runtime_error cause) : Exception(message, cause) {
}

}
}
}
}
}
}
