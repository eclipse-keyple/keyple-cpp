/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <string>
#include <typeinfo>

namespace keyple {
namespace core {
namespace command {

class SeCommand {
public:
    /**
     * Required by macOS/CLang...
     */
    SeCommand() = default;

    /**
     *
     */
    virtual ~SeCommand() = default;

    /**
     * Copy constructor
     * 
     * Required by macOS/CLang because of virtual destructor...
     */
    SeCommand(const SeCommand& sc) = default;

    /**
     *
     */
    virtual const std::string& getName() const = 0;

    /**
     *
     */
    virtual uint8_t getInstructionByte() const = 0;
};

}
}
}
