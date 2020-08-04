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

#include <vector>
#include <memory>

#include "ChannelControl.h"

/* Common */
#include "KeypleCoreExport.h"
#include "IllegalArgumentException.h"

/* Core */
#include "AbstractApduCommandBuilder.h"
#include "AbstractMatchingSe.h"
#include "KeypleException.h"
#include "SeRequest.h"
#include "SeSelector.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

/**
 * The AbstractSeSelectionRequest class combines a SeSelector with additional
 * helper methods useful to the selection process done in {@link SeSelection}.
 * <p>
 * This class may also be extended to add particular features specific to a SE
 * family.
 */
template<class T>
class KEYPLECORE_API AbstractSeSelectionRequest {
public:
    /**
     *
     */
    const std::shared_ptr<SeSelector> mSeSelector;

    /**
     *
     */
    AbstractSeSelectionRequest<T>(std::shared_ptr<SeSelector> seSelector)
    : mSeSelector(seSelector) {}

    /**
     *
     */
    virtual ~AbstractSeSelectionRequest<T>() {}

    /**
     * Returns a selection SeRequest built from the information provided in the
     * constructor and possibly completed with the commandBuilders list
     *
     * @return the selection SeRequest
     */
    virtual std::shared_ptr<SeRequest> getSelectionRequest()
    {
        std::vector<std::shared_ptr<ApduRequest>> seSelectionrApduRequests;

        for (const auto& commandBuilder : mCommandBuilders) {

            /* C++ specific. Java uses a template with "extends" keyword */
            std::shared_ptr<AbstractApduCommandBuilder> builder =
                std::static_pointer_cast<AbstractApduCommandBuilder>(
                    commandBuilder);
            if (!builder)
                throw IllegalArgumentException(
                          "template class does not extend" \
                          "AbstractApduCommandBuilder");

            seSelectionrApduRequests.push_back(
                commandBuilder->getApduRequest());
        }

        return std::make_shared<SeRequest>(mSeSelector,
                                           seSelectionrApduRequests);
    }

    /**
     *
     */
    virtual std::shared_ptr<SeSelector> getSeSelector()
    {
        return mSeSelector;
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os,
                                    const AbstractSeSelectionRequest<T>& asr)
    {
        os << "ABSTRACTSELECTIONREQUEST: {"
        << asr.mSeSelector
        << "}";

        return os;
    }

    /**
     * Virtual parse method
     *
     * @param seResponse the SE response received
     * @return a {@link AbstractMatchingSe}
     * @throws KeypleException if an error occurs while parsing the SE response
     *
     * Note: protected in Java, triggers error when used in SeSelection.cpp
     */
    virtual const std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) = 0;

    /**
     * Add an additional {@link AbstractApduCommandBuilder} for the command to
     * be executed after the selection process if it succeeds.
     * <p>
     * If more than one {@link AbstractApduCommandBuilder} is added, all will be
     * executed in the order in which they were added.
     *
     * @param commandBuilder an {@link AbstractApduCommandBuilder}
     *
     * /!\ C++ vs. Java: this function is protected in Java
     */
    void addCommandBuilder(std::shared_ptr<T> commandBuilder)
    {
        mCommandBuilders.push_back(commandBuilder);
    }

    /**
     * @return the current command builder list
     *
     * /!\ C++ vs. Java: this function is protected in Java
     */
    const std::vector<std::shared_ptr<T>> getCommandBuilders() const
    {
        return mCommandBuilders;
    }

private:
    /**
     * Optional command nuilber list of command to be executed following the
     * selection process
     */
    std::vector<std::shared_ptr<T>> mCommandBuilders;
};

/* Explicit template instanciation */
template class AbstractSeSelectionRequest<AbstractApduCommandBuilder>;

}
}
}
