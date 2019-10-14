#include "StubSe2.h"
#include "ByteArrayUtil.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace org::eclipse::keyple::core::util;
using namespace org::eclipse::keyple::plugin::stub;

const std::string StubSe2::seProtocol = "PROTOCOL_ISO14443_4";

StubSe2::StubSe2()
{
}

std::vector<char> StubSe2::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}


std::string StubSe2::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}
}
}

