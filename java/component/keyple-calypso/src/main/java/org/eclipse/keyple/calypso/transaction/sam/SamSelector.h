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
#include <stdexcept>
#include <memory>

/* Core */
#include "SeSelector.h"

/* Calypso */
#include "SamRevision.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace sam {

                        using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;
                        using SeSelector = org::eclipse::keyple::seproxy::SeSelector;

                        /**
                         * The {@link SamSelector} class extends {@link SeSelector} to handle specific Calypso SAM needs
                         * such as model identification.
                         */
                        class SamSelector : public SeSelector {
                            /**
                             * Create a SeSelector to perform the SAM selection
                             * <p>
                             * Two optional parameters
                             *
                             * @param samRevision the expected SAM revision (subtype)
                             * @param serialNumber the expected serial number as an hex string (padded with 0 on the left).
                             *        Can be a sub regex (e.g. "AEC0....")
                             * @param extraInfo information string (to be printed in logs)
                             */
                        public:
                            SamSelector(SamRevision samRevision, const std::string &serialNumber, const std::string &extraInfo);

protected:
                            std::shared_ptr<SamSelector> shared_from_this() {
                                return std::static_pointer_cast<SamSelector>(SeSelector::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
