/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <memory>

#include "ChannelControl.h"
#include "MultiSeRequestProcessing.h"
#include "SeReader.h"
#include "SeRequest.h"
#include "SeResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy;

/**
 * ProxyReader interface
 * <ul>
 * <li>To operate the transmission of Set of SeRequest, a specific local reader
 * processes the sorted list of SeRequest.</li>
 * <li>According to SeRequest protocolFlag and to the current status of the
 * reader (RF protocol involved / current ATR) the processing of a specific
 * SeRequest could be skipped.</li>
 * <li>When processing a SeRequest</li>
 * <li>if necessary a new logical channel is open (for a specific AID if
 * defined)</li>
 * <li>and ApduRequest are transmited one by one</li>
 * </ul>
 * This interface should be implemented by any specific reader plugin.
 */
class ProxyReader : public virtual SeReader {
public:
    /**
     *
     */
    virtual ~ProxyReader() {}

    /**
     * Transmits a Set of keyple::core::seproxy::message::SeRequest (list of
     * keyple::core::seproxy::message::SeRequest) to a SE application and get back the corresponding
     * a List of keyple::core::seproxy::message::SeResponse.
     * <p>
     * The usage of this method is conditioned to the presence of a SE in the selected reader.
     * <p>
     * All the keyple::core::seproxy::message::SeRequest are processed consecutively. The received
     * keyple::core::seproxy::message::SeResponse and placed in the List of
     * keyple::core::seproxy::message::SeResponse.
     * <p>
     * If the protocol flag set in the request match the current SE protocol and the keepChannelOpen
     * flag is set to true, the transmit method returns immediately with a List of
     * keyple::core::seproxy::message::SeResponse. This response contains the received response from
     * the matching SE in the last position of set. The previous one are set to null, the logical
     * channel is open.
     * <p>
     * If the protocol flag set in the request match the current SE protocol and the keepChannelOpen
     * flag is set to false, the transmission go on for the next
     * keyple::core::seproxy::message::SeRequest. The channel is left closed.
     * <p>
     * This method could also fail in case of IO error or wrong card currentState &rarr; some
     * reader’s exception (SE missing, IO error, wrong card currentState, timeout) have to be caught
     * during the processing of the SE request transmission.
     *
     * @param seRequests a list of application requests
     * @param multiSeRequestProcessing the multi se processing mode
     * @param channelControl indicates if the channel has to be closed at the end of the
     *        transmission
     * @return the SE response
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    virtual std::vector<std::shared_ptr<SeResponse>> transmitSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) = 0;

    /**
     * Transmits a single keyple::core::seproxy::message::SeRequest (list of
     * keyple::core::seproxy::message::ApduRequest) and get back the corresponding
     * keyple::core::seproxy::message::SeResponse
     * <p>
     * The usage of this method is conditioned to the presence of a SE in the selected reader.
     * <p>
     * The keyple::core::seproxy::message::SeRequest is processed and the received
     * keyple::core::seproxy::message::SeResponse is returned.
     * <p>
     * The logical channel is set according to the keepChannelOpen flag.
     *
     * <p>
     * This method could also fail in case of IO error or wrong card
     * currentState &rarr; some reader’s exception (SE missing, IO error, wrong
     * card currentState, timeout) have to be caught during the processing of
     * the SE request transmission. *
     *
     * @param seRequest the SeRequest to transmit
     * @param channelControl a flag to tell if the channel has to be closed at the end
     * @return SeResponse the response to the SeRequest
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    virtual std::shared_ptr<SeResponse> transmitSeRequest(std::shared_ptr<SeRequest> seRequest,
                                                          const ChannelControl& channelControl) = 0;
};

}
}
}
}
