#include "StubSe1.h"
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

const std::string StubSe1::seProtocol = "PROTOCOL_ISO7816_3";

StubSe1::StubSe1() 
{
}


std::vector<char> StubSe1::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubSe1::getSeProcotol()
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

