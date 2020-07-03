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

namespace keyple {
namespace core {
namespace command {

class SeCommand {
public:
    /**
     *
     */
    virtual ~SeCommand() {}

    /**
     *
     */
    virtual const std::string& getName() const = 0;

    /**
     *
     */
    virtual uint8_t getInstructionByte() const = 0;

    /**
     *
     */
    virtual const std::type_info& getCommandBuilderClass() const = 0;

    /**
     *
     */
    virtual const std::type_info& getResponseParserClass() const = 0;
};

}
}
}
