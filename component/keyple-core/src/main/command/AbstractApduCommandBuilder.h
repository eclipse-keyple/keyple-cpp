/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include <string>
#include <typeinfo>
#include <memory>

/* Common */
#include "KeypleCoreExport.h"

/* Core */
#include "ApduRequest.h"

/* Calypso */
#include "SeCommand.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;

/**
 * Generic APDU command builder.
 * <p>
 * It provides the generic getters to retrieve:
 * <ul>
 * <li>the SE command reference,</li>
 * <li>the built APDURequest,</li>
 * <li>the corresponding AbstractApduResponseParser class.</li>
 * </ul>
 */
class KEYPLECORE_API AbstractApduCommandBuilder
: public std::enable_shared_from_this<AbstractApduCommandBuilder> {

private:
    /**
     * The command name (will appear in logs)
     *
     * @deprecated use {@code reference} field instead
     */
    std::string mName;

protected:
    /**
     * The reference field is used to find the type of command concerned when
     * manipulating a list of abstract builder objects. Unfortunately, the
     * diversity of these objects does not allow the use of simple generic
     * methods.
     */
    const std::shared_ptr<SeCommand> mCommandRef;

    /**
     * The byte array APDU request
     */
    std::shared_ptr<ApduRequest> mRequest;

public:
    /**
    * the generic abstract constructor to build an APDU request with a command
    * reference and a byte array.
    *
    * @param commandRef command reference
    * @param request request
    */
    AbstractApduCommandBuilder(const std::shared_ptr<SeCommand> commandRef,
                               std::shared_ptr<ApduRequest> request);

    /**
     *
     */
    AbstractApduCommandBuilder(const std::string& name,
                               std::shared_ptr<ApduRequest> request);

    /**
    * Append a string to the current name
    *
    * @param subName the string to append
    */
    virtual void addSubName(const std::string& subName);

    /**
     * @return the current command identification
     */
    virtual std::shared_ptr<SeCommand> getCommandRef();

    /**
     * @return the name of the APDU command from the CalypsoCommands
     *         information.
     */
    virtual std::string getName() const;

    /**
     * @return the request
     */
    virtual std::shared_ptr<ApduRequest> getApduRequest() const;
};

}
}
}
