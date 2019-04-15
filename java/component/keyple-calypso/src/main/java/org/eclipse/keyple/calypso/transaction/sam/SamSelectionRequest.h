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
                        class SamSelectionRequest : public SeSelectionRequest {
                        public:
                            SamSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag);

protected:
                            std::shared_ptr<SamSelectionRequest> shared_from_this() {
                                return std::static_pointer_cast<SamSelectionRequest>(SeSelectionRequest::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
