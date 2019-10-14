/* Core */
#include "KeypleReaderException.h"

/* Calypso */
#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace org::eclipse::keyple::core::seproxy::exception;

KeypleCalypsoSecureSessionUnauthorizedKvcException
::KeypleCalypsoSecureSessionUnauthorizedKvcException(const std::string &message)
: KeypleReaderException(message)
{
}

}
}
}
}
}
}
