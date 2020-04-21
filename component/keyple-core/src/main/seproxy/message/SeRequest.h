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
#include <memory>

#include "stringhelper.h"

/* Core */
#include "ChannelControl.h"
#include "ApduRequest.h"
#include "SeProtocol_Import.h"

/* Common */
#include "Export.h"
#include "Serializable.h"

/* Forward declaration */
namespace keyple {
namespace core {
namespace seproxy {
class SeSelector;
}
}
}

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

/**
 * List of APDU requests that will result in a {@link SeResponse}
 *
 * @see SeResponse
 */
class EXPORT SeRequest final : public std::enable_shared_from_this<SeRequest> {
public:
    /**
     * The constructor called by a ProxyReader in order to open a logical
     * channel, to send a set of APDU commands to a SE application, or both of
     * them.
     *
     * @param seSelector the SeSelector containing the selection information to
     *        process the SE selection
     * @param apduRequests a optional list of {@link ApduRequest} to execute
     *        after a successful selection process
     * @param channelState the channel management parameter allowing to close or
     *        keep the channel open after the request execution
     */
    SeRequest(std::shared_ptr<SeSelector> seSelector,
              std::vector<std::shared_ptr<ApduRequest>>& apduRequests);

    /**
     * Constructor to be used when the SE is already selected (without {@link
     * SeSelector})
     *
     * @param apduRequests a list of ApudRequest
     * @param channelState a flag to tell if the channel has to be closed at the
     *        end
     */
    SeRequest(std::vector<std::shared_ptr<ApduRequest>>& apduRequests);

    /**
     *
     */
    virtual ~SeRequest()
    {
    }

    /**
     * Gets the SE seSelector.
     *
     * @return the current SE seSelector
     */
    std::shared_ptr<SeSelector> getSeSelector() const;

    /**
     * Gets the apdu requests.
     *
     * @return the group of APDUs to be transmitted to the SE application for
     *         this instance of SERequest.
     */
    std::vector<std::shared_ptr<ApduRequest>> getApduRequests() const;

    /**
     *¬
     */
    friend EXPORT
		std::ostream& operator<<(std::ostream& os, const SeRequest& se);

    /**
	 *
	 */
    friend EXPORT std::ostream&
        operator<<(std::ostream& os, const std::shared_ptr<SeRequest>& s);

	/**
	 *
	 */
	friend EXPORT std::ostream& operator<<(
		std::ostream& os, const std::set<std::shared_ptr<SeRequest>>& s);

private:
    /**
     * SE seSelector is either an AID or an ATR regular expression
     */
    std::shared_ptr<SeSelector> seSelector;

    /**
     * contains a group of APDUCommand to operate on the selected SE
     * application by the SE reader.
     */
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;
};

}
}
}
}
