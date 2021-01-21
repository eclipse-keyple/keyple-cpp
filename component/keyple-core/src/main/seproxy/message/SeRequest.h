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

/* Core */
#include "ChannelControl.h"
#include "ApduRequest.h"
#include "KeypleCoreExport.h"

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

/**
 * List of APDU requests that will result in a keyple::core::seproxy::message::SeResponse
 *
 * @see SeResponse
 */
class KEYPLECORE_API SeRequest final
: public std::enable_shared_from_this<SeRequest> {
public:
    /**
     * The constructor called by a ProxyReader in order to open a logical channel, to send a set of
     * APDU commands to a SE application, or both of them.
     *
     * @param seSelector the SeSelector containing the selection information to process the SE
     *        selection
     * @param apduRequests a optional list of keyple::core::seproxy::message::ApduRequest to execute
     *        after a successful selection process
     */
    SeRequest(const std::shared_ptr<SeSelector> seSelector,
              const std::vector<std::shared_ptr<ApduRequest>>& apduRequests);

    /**
     * Constructor to be used when the SE is already selected (without
     * keyple::core::seproxy::SeSelector)
     *
     * @param apduRequests a list of ApudRequest
     */
    SeRequest(const std::vector<std::shared_ptr<ApduRequest>>& apduRequests);

    /**
     * Gets the SE seSelector.
     *
     * @return the current SE seSelector
     */
    const std::shared_ptr<SeSelector> getSeSelector() const;

    /**
     * Gets the apdu requests.
     *
     * @return the group of APDUs to be transmitted to the SE application for this instance of
     *         SERequest.
     */
    const std::vector<std::shared_ptr<ApduRequest>>& getApduRequests() const;

    /**
     *¬
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const SeRequest& se);

    /**
	 *
	 */
    friend KEYPLECORE_API std::ostream&
        operator<<(std::ostream& os, const std::shared_ptr<SeRequest>& s);

	/**
	 *
	 */
    friend KEYPLECORE_API std::ostream& operator<<(
		std::ostream& os, const std::set<std::shared_ptr<SeRequest>>& s);

    /**
	 *
	 */
    friend KEYPLECORE_API std::ostream& operator<<(
		std::ostream& os,const std::vector<std::shared_ptr<SeRequest>>& s);

private:
    /**
     * SE seSelector is either an AID or an ATR regular expression
     */
    const std::shared_ptr<SeSelector> seSelector;

    /**
     * contains a group of APDUCommand to operate on the selected SE
     * application by the SE reader.
     */
    const std::vector<std::shared_ptr<ApduRequest>> apduRequests;
};

}
}
}
}
