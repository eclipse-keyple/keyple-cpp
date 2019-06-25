#pragma once

#include "../../AbstractSamCommandBuilder.h"
#include "../../CalypsoSamCommands.h"
#include "../../SamRevision.h"
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { class KeyReference; } } } }

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
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

                                /**
                                 * Builder for the SAM Give Random APDU command.
                                 */
                                class CardGenerateKeyCmdBuild : public AbstractSamCommandBuilder {
                                    /** The command reference. */
                                private:
                                    static constexpr CalypsoSamCommands command = CalypsoSamCommands::CARD_GENERATE_KEY;

                                    /**
                                     * Instantiates a new DigestUpdateCmdBuild and generate the ciphered data for a key ciphered by
                                     * another.
                                     *
                                     * <p>
                                     * If the provided ciphering key reference is null, the source key is ciphered with the null
                                     * key.
                                     * 
                                     * @param revision of the SAM
                                     * @param cipheringKey the key used to ciphering the source key (the null key is used if this
                                     *        reference is null)
                                     * @param sourceKey the reference of the key to be loaded
                                     */
                                public:
                                    CardGenerateKeyCmdBuild(SamRevision revision, std::shared_ptr<KeyReference> cipheringKey, std::shared_ptr<KeyReference> sourceKey);

protected:
                                    std::shared_ptr<CardGenerateKeyCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<CardGenerateKeyCmdBuild>(org.eclipse.keyple.calypso.command.sam.AbstractSamCommandBuilder::shared_from_this());
                                    }
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
