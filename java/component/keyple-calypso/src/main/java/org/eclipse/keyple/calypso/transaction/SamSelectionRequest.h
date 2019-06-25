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

/* Calypso */
#include "SamSelectionRequest.h"

/* Core */
#include "SeSelectionRequest.h"
#include "ChannelState.h"
#include "SeProtocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace sam {

                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeSelectionRequest = org::eclipse::keyple::transaction::SeSelectionRequest;

                        /**
                         * Specialized selection request to manage the specific characteristics of Calypso SAMs
                         */
                    class SamSelectionRequest : public AbstractSeSelectionRequest {
                        /**
                         * Create a {@link SamSelectionRequest}
                         * 
                         * @param samSelector the SAM selector
                         * @param channelState the action to take after (close or keep open)
                         */
                        public:
                        SamSelectionRequest(std::shared_ptr<SamSelector> samSelector, ChannelState channelState);

                        /**
                         * Create a CalypsoSam object containing the selection data received from the plugin
                         *
                         * @param seResponse the SE response received
                         * @return a {@link CalypsoSam}
                         */
                    protected:
                        std::shared_ptr<CalypsoSam> parse(std::shared_ptr<SeResponse> seResponse) override;

                    public:
                        std::shared_ptr<AbstractSamResponseParser> getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex) override;

protected:
                            std::shared_ptr<SamSelectionRequest> shared_from_this() {
                            return std::static_pointer_cast<SamSelectionRequest>(AbstractSeSelectionRequest::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
