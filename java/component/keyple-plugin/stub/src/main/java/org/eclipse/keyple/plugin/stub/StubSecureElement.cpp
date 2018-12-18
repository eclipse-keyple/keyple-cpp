#include "StubSecureElement.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleChannelStateException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    bool StubSecureElement::isPhysicalChannelOpen() {
                        return isPhysicalChannelOpen_Renamed;
                    }

                    void StubSecureElement::openPhysicalChannel() throw(KeypleChannelStateException) {
                        isPhysicalChannelOpen_Renamed = true;
                    }

                    void StubSecureElement::closePhysicalChannel() throw(KeypleChannelStateException) {
                        isPhysicalChannelOpen_Renamed = false;
                    }

                    void StubSecureElement::addHexCommand(const std::string &command, const std::string &response) {
                        assert((command != "" && response != "", "command and response should not be null"));
                        // add commands without space
                        hexCommands.emplace(StringHelper::replace(command, " ", ""), StringHelper::replace(response, " ", ""));
                    }

                    void StubSecureElement::removeHexCommand(const std::string &command) {
                        assert((command != "", "command should not be null"));
                        hexCommands.erase(StringHelper::trim(command));
                    }

                    std::vector<char> StubSecureElement::processApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) {

                        if (apduIn.empty()) {
                            return nullptr;
                        }

                        // convert apduIn to hexa
                        std::string hexApdu = ByteArrayUtils::toHex(apduIn);

                        // return matching hexa response if found
                        if (hexCommands.find(hexApdu) != hexCommands.end()) {
                            return ByteArrayUtils::fromHex(hexCommands[hexApdu]);
                        }

                        // throw a KeypleIOReaderException if not found
                        throw std::make_shared<KeypleIOReaderException>("No response available for this request.");
                    }
                }
            }
        }
    }
}
