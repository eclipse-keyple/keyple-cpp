/* Example */
#include "StubMifareClassic.h"

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

const std::string StubMifareClassic::seProtocol = "PROTOCOL_MIFARE_CLASSIC";

StubMifareClassic::StubMifareClassic()
{
    /* Get data */
    addHexCommand("FFCA 000000", "112233449000");
}

std::vector<char> StubMifareClassic::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubMifareClassic::getSeProcotol()
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
