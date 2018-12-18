#include "IncreaseCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using namespace org::eclipse::keyple::calypso::command::po;

                            IncreaseCmdBuild::IncreaseCmdBuild(PoClass poClass, char sfi, char counterNumber, int incValue, const std::string &extraInfo) throw(std::invalid_argument) : PoCommandBuilder(command, nullptr) {

                                // only counter number >= 1 are allowed
                                if (counterNumber < 1) {
                                    throw std::invalid_argument("Counter number out of range!");
                                }

                                // check if the incValue is in the allowed interval
                                if (incValue < 0 || incValue > 0xFFFFFF) {
                                    throw std::invalid_argument("Increment value out of range!");
                                }

                                // convert the integer value into a 3-byte buffer
                                std::vector<char> incValueBuffer(3);
                                incValueBuffer[0] = static_cast<char>((incValue >> 16) & 0xFF);
                                incValueBuffer[1] = static_cast<char>((incValue >> 8) & 0xFF);
                                incValueBuffer[2] = static_cast<char>(incValue & 0xFF);

                                char p2 = static_cast<char>(sfi * 8);

                                /* this is a case4 command, we set Le = 0 */
                                this->request = setApduRequest(poClass.getValue(), command, counterNumber, p2, incValueBuffer, static_cast<char>(0x00));
                                if (extraInfo != "") {
                                    this->addSubName(extraInfo);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
