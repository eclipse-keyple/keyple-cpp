/* Plugin - Stub */
#include "StubSecureElement.h"

/* Core - Seproxy - Exception */
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"

/* Core - Util */
#include "ByteArrayUtil.h"

/* Common */
#include "stringhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace plugin {
namespace stub {

using KeypleChannelStateException  = org::eclipse::keyple::core::seproxy::exception::KeypleChannelStateException;
using KeypleIOReaderException      = org::eclipse::keyple::core::seproxy::exception::KeypleIOReaderException;
using ByteArrayUtil                = org::eclipse::keyple::core::util::ByteArrayUtil;

bool StubSecureElement::isPhysicalChannelOpen() {
    return isPhysicalChannelOpen_Renamed;
}

void StubSecureElement::openPhysicalChannel()
{
    isPhysicalChannelOpen_Renamed = true;
}

void StubSecureElement::closePhysicalChannel()
{
    isPhysicalChannelOpen_Renamed = false;
}

void StubSecureElement::addHexCommand(const std::string &command, const std::string &response)
{
    if (!command.compare("") || !response.compare("")) {
        logger->debug("either command or response is empty\n");
        return;
    }

    /* Add commands without space */
    hexCommands.emplace(StringHelper::replace(command, " ", ""),
                        StringHelper::replace(response, " ", ""));
}

void StubSecureElement::removeHexCommand(const std::string &command)
{
    if (command.compare(""))
        //"command should not be null"));
        return;

    hexCommands.erase(StringHelper::trim(command));
}

std::vector<char> StubSecureElement::processApdu(std::vector<char> &apduIn)
{

    if (apduIn.empty()) {
        return apduIn;
    }

    // convert apduIn to hexa
    std::string hexApdu = ByteArrayUtil::toHex(apduIn);

    logger->debug("looking for r-apdu response to c-apdu: %s\n", hexApdu);
    logger->debug("%d combinations available\n", hexCommands.size());

    // return matching hexa response if found
    if (hexCommands.find(hexApdu) != hexCommands.end()) {
        return ByteArrayUtil::fromHex(hexCommands[hexApdu]);
    }

    // throw a KeypleIOReaderException if not found
    throw std::make_shared<KeypleIOReaderException>("No response available for this request.");
}

}
}
}
}
}
