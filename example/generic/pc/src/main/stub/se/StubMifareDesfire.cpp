/* Example */
#include "StubMifareDesfire.h"

/* Core */
#include "ByteArrayUtil.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace org::eclipse::keyple::plugin::stub;
using namespace org::eclipse::keyple::core::util;

const std::string StubMifareDesfire::seProtocol = "PROTOCOL_MIFARE_DESFIRE";

StubMifareDesfire::StubMifareDesfire()
{
    /* Get data */
    addHexCommand("FFCA 000000", "223344556677889000");
}

std::vector<char> StubMifareDesfire::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubMifareDesfire::getSeProcotol()
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
