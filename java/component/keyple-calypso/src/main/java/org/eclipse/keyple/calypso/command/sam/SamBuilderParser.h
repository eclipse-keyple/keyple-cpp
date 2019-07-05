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

#include "AbstractSamCommandBuilder.h"
#include "AbstractSamResponseParser.h"
#include "CalypsoBuilderParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace org::eclipse::keyple::calypso::command;

using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;

class SamBuilderParser
: public std::enable_shared_from_this<SamBuilderParser>,
  public CalypsoBuilderParser<AbstractSamCommandBuilder, AbstractSamResponseParser>
{
private:
    const std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder;
    std::shared_ptr<AbstractSamResponseParser> samResponseParser;

public:
    SamBuilderParser(std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder);

    virtual std::shared_ptr<AbstractSamCommandBuilder> getCommandBuilder();

    virtual std::shared_ptr<AbstractSamResponseParser> getResponseParser();

    virtual void setResponseParser(std::shared_ptr<AbstractSamResponseParser> poResponseParser);
};

}
}
}
}
}
}
