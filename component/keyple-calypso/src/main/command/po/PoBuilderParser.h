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

#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "CalypsoBuilderParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::calypso::command;

/**
 * The PoBuilderParser class contains the builder of a {@link
 * PoSendableInSession} command
 * <p>
 * A setter allows to associate the parser object.
 */
template <typename T>
class KEYPLECALYPSO_API PoBuilderParser
: public std::enable_shared_from_this<PoBuilderParser<T>>,
  public CalypsoBuilderParser<T, AbstractPoResponseParser> {
public:
    /**
     *
     */
    PoBuilderParser(std::shared_ptr<T> poCommandBuilder)
    : poCommandBuilder(poCommandBuilder)
    {
    }

    /**
     *
     */
    virtual ~PoBuilderParser()
    {
    }

    /**
     *
     */
    virtual std::shared_ptr<T> getCommandBuilder()
    {
        return poCommandBuilder;
    }

    /**
     *
     */
    virtual std::shared_ptr<AbstractPoResponseParser> getResponseParser()
    {
        return mPoResponseParser;
    }

    /**
     *
     */
    virtual void setResponseParser(
        std::shared_ptr<AbstractPoResponseParser> poResponseParser)
    {
        mPoResponseParser = poResponseParser;
    }

private:
    /**
     *
     */
    const std::shared_ptr<T> poCommandBuilder;

    /**
     *
     */
    std::shared_ptr<AbstractPoResponseParser> mPoResponseParser;
};

}
}
}
}
