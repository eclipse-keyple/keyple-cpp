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
#include <vector>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"
#include "Serializable.h"
#include "Object.h"

/* Forward class declarations */
namespace keyple {
namespace core {
namespace seproxy {
namespace message {
class SelectionStatus;
}
}
}
}
namespace keyple {
namespace core {
namespace seproxy {
namespace message {
class ApduResponse;
}
}
}
}

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::common;

/**
 * Group of SE responses received in response to a {@link SeRequest}.
 *
 * @see SeRequest
 */
class EXPORT SeResponse final : public std::enable_shared_from_this<SeResponse>,
                                public Serializable,
                                public Object {
public:
    /**
     * the constructor called by a ProxyReader during the processing of the
     * ‘transmit’ method.
     *
     * @param logicalChannelIsOpen the current channel status
     * @param channelPreviouslyOpen the channel previously open
     * @param selectionStatus the SE selection status
     * @param apduResponses the apdu responses
     */
    SeResponse(bool logicalChannelIsOpen, bool channelPreviouslyOpen,
               std::shared_ptr<SelectionStatus> selectionStatus,
               std::vector<std::shared_ptr<ApduResponse>>& apduResponses);

    /**
     * Was channel previously open.
     *
     * @return the previous currentState of the logical channel.
     */
    bool wasChannelPreviouslyOpen();

    /**
     * Get the logical channel status
     *
     * @return true if the logical channel is open
     */
    bool isLogicalChannelOpen();

    /**
     * Gets the selection status and its associated data.
     *
     * @return a {@link SelectionStatus} object.
     */
    const std::shared_ptr<SelectionStatus> getSelectionStatus() const;

    /**
     * Gets the apdu responses.
     *
     * @return the group of APDUs responses returned by the SE application for
     *         this instance of SEResponse.
     */
    std::vector<std::shared_ptr<ApduResponse>> getApduResponses();

    /**
     *
     */
    std::string toString() override;

    /**
     *
     */
    bool equals(std::shared_ptr<void> o) override;

    /**
     *
     */
    int hashCode() override;

protected:
    /**
     *
     */
    void finalize() override;

private:
    /**
     * is defined as true by the SE reader in case a logical channel was already
     * open with the target SE application.
     */
    bool channelPreviouslyOpen = false;

    /**
     * true if the channel is open
     */
    const bool logicalChannelIsOpen;

    /**
     *
     */
    const std::shared_ptr<SelectionStatus> selectionStatus;

    /**
     * could contain a group of APDUResponse returned by the selected SE
     * application on the SE reader.
     */
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
};

}
}
}
}
