#include "KeypleIOReaderException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

KeypleIOReaderException::KeypleIOReaderException(const std::string &message) : KeypleReaderException(message) {
}

KeypleIOReaderException::KeypleIOReaderException(const std::string &message, std::runtime_error cause) : KeypleReaderException(message, cause) {
}

}
}
}
}
}
}
