/* Example */
#include "StubMifareUL.h"

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

const std::string StubMifareUL::seProtocol = "PROTOCOL_MIFARE_UL";

StubMifareUL::StubMifareUL()
{
    /* Get data */
    addHexCommand("FFCA 000000", "223344556677889000");
}

std::vector<char> StubMifareUL::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubMifareUL::getSeProcotol()
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
