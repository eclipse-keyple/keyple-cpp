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

#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "CalypsoBuilderParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace org::eclipse::keyple::calypso::command;;

/**
    * The PoBuilderParser class contains the builder of a {@link PoSendableInSession} command
    * <p>
    * A setter allows to associate the parser object.
    */
template<typename T>
class PoBuilderParser
: public std::enable_shared_from_this<PoBuilderParser<T>>,
  public CalypsoBuilderParser<AbstractPoCommandBuilder<T>, AbstractPoResponseParser> {
private:
    const std::shared_ptr<AbstractPoCommandBuilder<T>> poCommandBuilder;
    std::shared_ptr<AbstractPoResponseParser> poResponseParser;

public:
    PoBuilderParser(std::shared_ptr<AbstractPoCommandBuilder<T>> poCommandBuilder)
    : poCommandBuilder(poCommandBuilder)
    {
    }

    virtual std::shared_ptr<AbstractPoCommandBuilder<T>> getCommandBuilder() {
        return poCommandBuilder;
    }

    virtual std::shared_ptr<AbstractPoResponseParser> getResponseParser() {
        return poResponseParser;
    }

    virtual void setResponseParser(std::shared_ptr<AbstractPoResponseParser> poResponseParser) {
        this->poResponseParser = poResponseParser;
    }
};

}
}
}
}
}
}
