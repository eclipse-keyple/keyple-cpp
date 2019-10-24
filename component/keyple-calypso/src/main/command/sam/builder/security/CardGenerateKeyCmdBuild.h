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

#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

/* Calypso */
#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommands.h"
#include "KeyReference.h"
#include "SamRevision_Import.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso;
using namespace keyple::calypso::command::sam;

/**
 * Builder for the SAM Give Random APDU command.
 */
class CardGenerateKeyCmdBuild : public AbstractSamCommandBuilder {
public:
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
    CardGenerateKeyCmdBuild(SamRevision revision, std::shared_ptr<KeyReference> cipheringKey, std::shared_ptr<KeyReference> sourceKey);

protected:
    /**
     *
     */
    std::shared_ptr<CardGenerateKeyCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<CardGenerateKeyCmdBuild>(AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command reference
     */
    CalypsoSamCommands& command = CalypsoSamCommands::CARD_GENERATE_KEY;
};

}
}
}
}
}
}
