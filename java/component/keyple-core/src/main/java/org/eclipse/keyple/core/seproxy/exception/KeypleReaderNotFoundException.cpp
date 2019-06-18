#include "KeypleReaderNotFoundException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

KeypleReaderNotFoundException::KeypleReaderNotFoundException(const std::string &readerName) : KeypleReaderException("Reader with name " + readerName + " was not found") {
}

}
}
}
}
}
}
