#include "CardGenerateKeyCmdBuild.h"
#include "../../../../KeyReference.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            namespace security {
                                using KeyReference = org::eclipse::keyple::calypso::KeyReference;
                                using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                CardGenerateKeyCmdBuild::CardGenerateKeyCmdBuild(SamRevision revision, std::shared_ptr<KeyReference> cipheringKey, std::shared_ptr<KeyReference> sourceKey) : org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder(command, nullptr) {
                                    if (revision != nullptr) {
                                        this->defaultRevision = revision;
                                    }
                                    if (sourceKey == nullptr) {
                                        throw std::invalid_argument("The source key reference can't be null.");
                                    }

                                    char cla = this->defaultRevision.getClassByte();

                                    char p1, p2;
                                    std::vector<char> data;

                                    if (cipheringKey == nullptr) {
                                        // case where the source key is ciphered by the null key
                                        p1 = static_cast<char>(0xFF);
                                        p2 = static_cast<char>(0x00);

                                        data = std::vector<char>(3);
                                        data[0] = sourceKey->getKif();
                                        data[1] = sourceKey->getKvc();
                                        data[2] = static_cast<char>(0x90);
                                    }
                                    else {
                                        p1 = static_cast<char>(0xFF);
                                        p2 = static_cast<char>(0xFF);

                                        data = std::vector<char>(5);
                                        data[0] = cipheringKey->getKif();
                                        data[1] = cipheringKey->getKvc();
                                        data[2] = sourceKey->getKif();
                                        data[3] = sourceKey->getKvc();
                                        data[4] = static_cast<char>(0x90);
                                    }

                                    request = setApduRequest(cla, command, p1, p2, data, nullptr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
