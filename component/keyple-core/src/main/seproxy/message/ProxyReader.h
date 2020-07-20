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
using namespace keyple::core::seproxy::message;

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
     * Transmits a Set of {@link SeRequest} (list of {@link SeRequest}) to a SE
     * application and get back the corresponding a List of {@link SeResponse}.
     * <p>
     * The usage of this method is conditioned to the presence of a SE in the
     * selected reader.
     * <p>
     * All the {@link SeRequest} are processed consecutively. The received
     * {@link SeResponse} and placed in the List of {@link SeResponse}.
     * <p>
     * If the protocol flag set in the request match the current SE protocol and
     * the keepChannelOpen flag is set to true, the transmit method returns
     * immediately with a List of {@link SeResponse}. This response contains the
     * received response from the matching SE in the last position of set. The
     * previous one are set to null, the logical channel is open.
     * <p>
     * If the protocol flag set in the request match the current SE protocol and
     * the keepChannelOpen flag is set to false, the transmission go on for the
     * next {@link SeRequest}. The channel is left closed.
     * <p>
     * This method could also fail in case of IO error or wrong card
     * currentState &rarr; some reader’s exception (SE missing, IO error, wrong
     * card currentState, timeout) have to be caught during the processing of
     * the SE request transmission.
     *
     * @param seRequests a {@link List} of application requests
     * @param multiSeRequestProcessing the multi se processing mode
     * @param channelControl indicates if the channel has to be closed at the
     *        end of the transmission
     * @return the SE response
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual std::vector<std::shared_ptr<SeResponse>> transmitSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) = 0;

    /**
     * Transmits a single {@link SeRequest} (list of {@link ApduRequest}) and
     * get back the corresponding {@link SeResponse}
     * <p>
     * The usage of this method is conditioned to the presence of a SE in the
     * selected reader.
     * <p>
     * The {@link SeRequest} is processed and the received {@link SeResponse} is
     * returned.
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
     * @param channelControl a flag to tell if the channel has to be closed at
     *        the end
     * @return SeResponse the response to the SeRequest
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual std::shared_ptr<SeResponse> transmitSeRequest(
        std::shared_ptr<SeRequest> seRequest,
        ChannelControl channelControl) = 0;
};

}
}
}
}
