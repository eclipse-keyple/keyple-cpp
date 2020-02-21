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

#include "AbstractApduResponseParser_Import.h"
#include "AbstractIso7816CommandBuilder.h"

namespace keyple {
namespace calypso {
namespace command {

using namespace keyple::core::command;

template <typename B, typename P> class CalypsoBuilderParser {
    /*
    static_assert(std::is_base_of>AbstractApduResponseParser, P>::value,
                  "P must inherit from org.eclipse.keyple.core.command.AbstractApduResponseParser");
    static_assert(std::is_base_of<AbstractIso7816CommandBuilder, B>::value,
                  "B must inherit from org.eclipse.keyple.core.command.AbstractIso7816CommandBuilder");
     */

public:
    /**
     *
     */
    virtual std::shared_ptr<B> getCommandBuilder() = 0;

    /**
     *
     */
    virtual std::shared_ptr<P> getResponseParser() = 0;

    /**
     *
     */
    virtual void setResponseParser(std::shared_ptr<P> parser) = 0;
};

}
}
}
