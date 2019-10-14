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

#include <string>
#include <memory>

/* Common */
#include "Export.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace selection { class AbstractMatchingSe; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace selection { class AbstractSeSelectionRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeResponse; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace command { class AbstractApduResponseParser; } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using SeResponse = org::eclipse::keyple::core::seproxy::message::SeResponse;

/**
 * The MatchingSelection class holds the result of a single selection case.
 */
class EXPORT MatchingSelection final : public std::enable_shared_from_this<MatchingSelection> {
private:
    /**
     *
     */
    const std::shared_ptr<AbstractMatchingSe> matchingSe;

    /**
     *
     */
    const std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;

    /**
     *
     */
    const std::shared_ptr<SeResponse> selectionSeResponse;

    /**
     *
     */
    const int selectionIndex;

    /**
     * Constructor
     *
     * @param selectionIndex the selection index
     * @param seSelectionRequest the selection request
     * @param matchingSe the matching SE
     * @param selectionSeResponse the selection SeResponse
     */
public:
    MatchingSelection(int selectionIndex, std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest,
                      std::shared_ptr<AbstractMatchingSe> matchingSe, std::shared_ptr<SeResponse> selectionSeResponse);

    /**
     * @return the AbstractMatchingSe
     */
    std::shared_ptr<AbstractMatchingSe> getMatchingSe();

    /**
     * Get the parser for the targeted response.
     *
     * @param commandIndex the command index
     * @return a parser object
     */
    std::shared_ptr<AbstractApduResponseParser> getResponseParser(int commandIndex);

    /**
     * @return the info string provided with the Selector
     */
    std::string getExtraInfo();

    /**
     * @return the index of the selection (order in the prepareSelection command). 0 is the first
     *         selection.
     */
    int getSelectionIndex();
};

}
}
}
}
}
