/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
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

#include <memory>
#include <type_traits>

#include "AbstractApduResponseParser_Import.h"
#include "AbstractIso7816CommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "ApduRequest.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Abstract class for all PO command builders.
 * <p>
 */
template<typename T>
class AbstractPoCommandBuilder : public AbstractIso7816CommandBuilder {
public:
    /*
    static_assert(std::is_base_of<AbstractApduResponseParser, T>::value, "T must inherit from org.eclipse.keyple.core.command.AbstractApduResponseParser");
    */

    /**
     * Constructor dedicated for the building of referenced Calypso commands
     * 
     * @param reference a command reference from the Calypso command table
     * @param request the ApduRequest (the instruction byte will be overwritten)
     */
    AbstractPoCommandBuilder(CalypsoPoCommands& reference, std::shared_ptr<ApduRequest> request)
    : AbstractIso7816CommandBuilder(reference, request)
    {
    }

    /**
     *
     */
    virtual ~AbstractPoCommandBuilder() {}

    /**
     * Create the response parser matching the builder
     *
     * @param apduResponse the response data from the SE
     * @return an {@link AbstractApduResponseParser}
     */
    virtual std::shared_ptr<T> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) = 0;

protected:
    /**
     *
     */
    std::shared_ptr<AbstractPoCommandBuilder> shared_from_this()
    {
        return std::static_pointer_cast<AbstractPoCommandBuilder>(AbstractIso7816CommandBuilder::shared_from_this());
    }
};

}
}
}
}
