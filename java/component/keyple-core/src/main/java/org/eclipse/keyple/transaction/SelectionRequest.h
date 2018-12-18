#pragma once

#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequestSet; } } } } }

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {

                using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;

                /**
                 * Class containing the {@link SeRequestSet} used to make a default selection at the
                 * {@link org.eclipse.keyple.seproxy.event.ObservableReader} level.
                 */
                class SelectionRequest : public std::enable_shared_from_this<SelectionRequest> {
                    /** The {@link SeRequestSet} */
                private:
                    const std::shared_ptr<SeRequestSet> selectionSeRequestSet;

                public:
                    SelectionRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet);

                    virtual std::shared_ptr<SeRequestSet> getSelectionSeRequestSet();
                };

            }
        }
    }
}
