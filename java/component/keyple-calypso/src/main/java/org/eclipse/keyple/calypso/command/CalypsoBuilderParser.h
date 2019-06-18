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

#include <type_traits>

#include "AbstractApduResponseParser.h"
#include "AbstractIso7816CommandBuilder.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {

using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
using AbstractIso7816CommandBuilder = org::eclipse::keyple::command::AbstractIso7816CommandBuilder;

template<typename B, typename P>
class CalypsoBuilderParser {
    static_assert(std::is_base_of<org.eclipse.keyple.core.command.AbstractApduResponseParser, P>::value, "P must inherit from org.eclipse.keyple.core.command.AbstractApduResponseParser");
    static_assert(std::is_base_of<org.eclipse.keyple.core.command.AbstractIso7816CommandBuilder, B>::value, "B must inherit from org.eclipse.keyple.core.command.AbstractIso7816CommandBuilder");

public:
    virtual B getCommandBuilder() = 0;

    virtual P getResponseParser() = 0;

    virtual void setResponseParser(P parser) = 0;
};

}
}
}
}
}
