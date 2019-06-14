#include "CloseSessionCmdBuild.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {
                                using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                                using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                                CloseSessionCmdBuild::CloseSessionCmdBuild(PoClass poClass, bool ratificationAsked, std::vector<char> &terminalSessionSignature)
                                : PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr)
                                {
                                    // The optional parameter terminalSessionSignature could contain 4 or 8
                                    // bytes.
                                    if (terminalSessionSignature.size() > 0 && terminalSessionSignature.size() != 4 && terminalSessionSignature.size() != 8) {
                                        throw std::invalid_argument("Invalid terminal sessionSignature: " + ByteArrayUtils::toHex(terminalSessionSignature));
                                    }

                                    char p1 = ratificationAsked ? static_cast<char>(0x80) : static_cast<char>(0x00);
                                    /*
                                     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
                                     * length will be processed by the lower layers.
                                     */
                                    char le = 0;

                                    request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), p1, static_cast<char>(0x00), terminalSessionSignature, le);
                                }

                                CloseSessionCmdBuild::CloseSessionCmdBuild(PoClass poClass) : PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr)
                                {
                                    std::vector<char> emptyVector;
                                    request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), static_cast<char>(0x00), static_cast<char>(0x00), emptyVector, static_cast<char>(0));
                                    /* Add "Abort session" to command name for logging purposes */
                                    this->addSubName("Abort session");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
